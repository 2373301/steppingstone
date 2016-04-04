
#include <ace/os_include/netinet/os_tcp.h>
#include "GateSession.h"
#include "Logger.h"

#define MSG_BLOCK_LEN	1024000

GateSession::GateSession()
: m_handle_input(NULL)
, m_read_timeout_value(0, 1000)
{
	m_input_msg_block.init(MSG_BLOCK_LEN);
	m_output_msg_block.init(MSG_BLOCK_LEN);
}

GateSession::~GateSession()
{
}

int GateSession::open(void * p)
{
	this->peer().enable(ACE_NONBLOCK);

	int flag = 1;
	this->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

	return 0;
}

int GateSession::handle_input(ACE_HANDLE  fd)
{
	return 0;
}

int GateSession::handle_output(ACE_HANDLE  fd)
{
	return 0;
}

int GateSession::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	return 0;
}

void GateSession::setHandleInput(GateHandleInput * handle_input)
{
	m_handle_input = handle_input;
}

int GateSession::processRead()
{
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
			DEF_LOG_ERROR("Remote connection closed in GateSession::rd_stream, last error is <%d>\n", ACE_OS::last_error());
			return 2;
		}
	}
}

int GateSession::processWrite(PacketQue_t & output_packet_que)
{
	m_output_msg_block.crunch();

	Head gate_head;
	Packet * packet = NULL;
	while (output_packet_que.size() > 0)
	{
		packet = output_packet_que.front();
		size_t msg_len = m_output_msg_block.space();
		if (packet->stream_size() <= msg_len)
		{
			gate_head.opcode = packet->opcode();
			gate_head.size = packet->body_size();

			m_output_msg_block.copy((char *)&gate_head, sizeof(gate_head));
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

int GateSession::parseInputPacket()
{
	Packet * packet = NULL;
	Head gate_header;

	while(true)
	{
		memset(&gate_header, 0, sizeof(gate_header));
		if (m_input_msg_block.length() < sizeof(gate_header))
		{
			break;
		}

		memcpy(&gate_header, m_input_msg_block.rd_ptr(), sizeof(gate_header));

		if (gate_header.size > 10240 * 4)
		{
            DEF_LOG_ERROR(ACE_TEXT("opcode:%d, gate packet body size is too big : <%d>\n"), gate_header.opcode, gate_header.size);
			return 2;
		}

		if (m_input_msg_block.length() >= sizeof(gate_header) + gate_header.size)
		{
			packet = new Packet();
			packet->setOwner(this);
			memcpy(packet->ch_head(), m_input_msg_block.rd_ptr(), sizeof(gate_header));

			m_input_msg_block.rd_ptr(sizeof(gate_header));
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
				DEF_LOG_ERROR("the input handle is NULL\n");
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
