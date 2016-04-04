
#include <ace/OS.h>
#include <ace/os_include/netinet/os_tcp.h>
#include "Logger.h"
#include "MemberSession.h"
#include "ManageMemberRegister.h"
#include "ManageMemberSessionCooler.h"
#include "Logger.h"

#define MSG_BLOCK_LEN	1024000

MemberSession::MemberSession()
: m_handle_input(NULL)
, m_read_timeout_value(0, 1000)
{
	m_input_msg_block.init(MSG_BLOCK_LEN);
	m_output_msg_block.init(MSG_BLOCK_LEN);
}

MemberSession::~MemberSession()
{

}

int MemberSession::open(void * p)
{
	this->peer().enable(ACE_NONBLOCK);

	int flag = 1;
	this->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

	ACE_TCHAR peer_name[MAXHOSTNAMELEN] = {0};
	ACE_INET_Addr peer_addr;
	if (this->peer().get_remote_addr(peer_addr) == 0 &&
		peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN) == 0)
	{
		DEF_LOG_INFO("Get new member session connection, the ip is : %s\n", peer_name);
	}

	m_remote_ip = peer_name;

	this->reactor()->remove_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

	ManageMemberRegister::instance()->handleMemberSession(this);

	return 0;
}

int MemberSession::handle_input(ACE_HANDLE fd)
{
	int r = processRead();
	if (2 == r)
	{
		// lost connection
		ManageMemberSessionCooler::instance()->lostConnection(this);
		return -1;
	}

	return 0;
}

int MemberSession::handle_output(ACE_HANDLE fd)
{
	return 0;
}

int MemberSession::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	return 0;
}

void MemberSession::setHandleInput(MemberHandleInput * handle_input)
{
	m_handle_input = handle_input;
}

int MemberSession::processRead()
{
	// 
	int recv_n = this->peer().recv(m_input_msg_block.wr_ptr(), m_input_msg_block.space(), &m_read_timeout_value);
	if (recv_n > 0)
	{
		m_input_msg_block.wr_ptr(recv_n);
		return parseInputPacket();
	}
	else if (0 == recv_n)
	{
		return 2;
	}
	else
	{
		int last_error = ACE_OS::last_error();
		if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error) || (ETIME == last_error))
		{
			// it's ok
			return 1;
		}
		else
		{
			//// socket error
			DEF_LOG_ERROR("Remote connection closed in MemberSession::rd_stream, last error is <%d>\n", ACE_OS::last_error());
			return 2;
		}
	}
}

int MemberSession::processWrite(PacketQue_t & output_packet_que)
{
	m_output_msg_block.crunch();

	MemberHead member_head;
	Packet * packet = NULL;
	while (output_packet_que.size() > 0)
	{
		packet = output_packet_que.front();
		if (packet->stream_size() <= m_output_msg_block.space())
		{
			member_head.opcode = packet->opcode();
			member_head.size = packet->body_size();

			m_output_msg_block.copy((char *)&member_head, sizeof(member_head));
			m_output_msg_block.copy(packet->ch_body(), packet->body_size());

			delete packet;
			packet = NULL;
			output_packet_que.pop();
		}
		else
		{
			break;
		}
	}

	if (m_output_msg_block.length() == 0)
	{
		return 1;
	}

	while (m_output_msg_block.length() > 0)
	{
		int send_n = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
		if (send_n > 0)
		{
			m_output_msg_block.rd_ptr(send_n);
		}
		else
		{
			int last_error = ACE_OS::last_error();
			if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error))
			{
				// ok
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}

	return 0;
}

int MemberSession::processWrite(Packet * packet)
{
	int send_time = 0;

	//char buffer[1024] = {0};
    MemberHead member_head;
    char *buffer = new char[packet->body_size() + sizeof(member_head)];
	member_head.opcode = packet->opcode();
	member_head.size = packet->body_size();
	memcpy(buffer, &member_head, sizeof(member_head));
	memcpy(buffer + sizeof(member_head), packet->ch_body(), packet->body_size());

	ACE_Time_Value timeout_value(1, 0);

	int already_send_byte = 0;

	while (++send_time <= 3)
	{
		int send_n = this->peer().send_n(buffer + already_send_byte, sizeof(member_head) + member_head.size - already_send_byte, 0, &timeout_value);
		if (send_n == (sizeof(member_head) + member_head.size - already_send_byte))
		{
			// ok sucess
			break;
		}
		else
		{
			if (send_n > 0)
			{
				already_send_byte += send_n;
			}

			int last_error = ACE_OS::last_error();
			if ((EINTR == last_error) || (EAGAIN == last_error))
			{
				continue;
			}
			else
			{
				DEF_LOG_ERROR("failed to send the packet, opcode is <%d>, size is <%d>, last error is <%d>\n", packet->opcode(), packet->body_size(), last_error);
				break;
			}
		}
	}

	delete packet;
    
    delete buffer;
    buffer = NULL;
	return 0;
}

int MemberSession::parseInputPacket()
{
	Packet * packet = NULL;
	MemberHead member_header;

	while(true)
	{
		memset(&member_header, 0, sizeof(member_header));
		if (m_input_msg_block.length() < sizeof(member_header))
		{
			break;
		}

		memcpy(&member_header, m_input_msg_block.rd_ptr(), sizeof(member_header));

		if (member_header.size > 1024 * 4)
		{
            DEF_LOG_ERROR(ACE_TEXT("opcode:%d, member packet body size is too big : <%d>\n"), member_header.opcode, member_header.size);
			return 2;
		}

		if (m_input_msg_block.length() >= sizeof(member_header) + member_header.size)
		{
			packet = new Packet();
			packet->setOwner(this);
			Head head;
			head.opcode = member_header.opcode;
			head.size = member_header.size;
			head.guid = 0;
			memcpy(packet->ch_head(), &head, sizeof(head));

			m_input_msg_block.rd_ptr(sizeof(member_header));
			if (packet->body_size() > 0)
			{
				memcpy(packet->ch_body(), m_input_msg_block.rd_ptr(), packet->body_size());
				m_input_msg_block.rd_ptr(packet->body_size());
			}
			else
			{
				// do nothing
			}

			if (NULL != m_handle_input)
			{
				m_handle_input->input(packet);
			}
			else
			{
				DEF_LOG_ERROR("the input handle is NULL in membersession\n");
				delete packet;
				packet = NULL;
			}
		}
		else
		{
			delete packet;
			packet = NULL;
			break;
		}
	}

	m_input_msg_block.crunch();

	return 0;
}
