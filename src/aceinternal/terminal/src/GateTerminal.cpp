
#include <ace/os_include/netinet/os_tcp.h>
#include "GateTerminal.h"
#include "Logger.h"
#include "opcode.h"

#define SOCK_BUFFER_LEN 1024000

GateTerminal::GateTerminal()
: m_is_writing(false)
, m_msg_block_index(0)
, m_msg_block_pos(0)
, m_current_msg_block(NULL)
{

}

GateTerminal::~GateTerminal()
{
	for (MsgBlockQueMap_t::iterator it = m_msg_block_que_map.begin(); it != m_msg_block_que_map.end(); ++it)
	{
		MsgBlockQue_t & msg_bq = it->second;
		while (msg_bq.size() > 0)
		{
			delete msg_bq.front();
			msg_bq.pop_front();
		}
	}
}

int GateTerminal::session_on_read()
{
	int result = Session::session_on_read();
	if (-1 == result)
	{
		DEF_LOG_ERROR("occur error in GateTerminal::handle_input, rd_stream return value is -1, last error is <%d>\n", ACE_OS::last_error());
		geteLostConnection();
		return -1;
	}
	else
	{
		return 0;
	}
}

void GateTerminal::outputBuffer(uint32 msg_index, MsgBlockQue_t & msg_block_que)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_msg_block_que_map_mutex, );
	//MsgBlockQueMap_t::iterator it = m_msg_block_que_map.find(msg_index);
	//if (it != m_msg_block_que_map.end())
	//{
	//	DEF_LOG_ERROR("get redumplicate msg index <%d>\n", msg_index);
	//}
	m_msg_block_que_map.insert(std::make_pair(msg_index, msg_block_que));
}

uint32 GateTerminal::getMessageIndex(uint32 increase_no)
{
	uint32 result = m_msg_block_index;
	m_msg_block_index = increaseNumber(m_msg_block_index, increase_no);

	return result;
}


void GateTerminal::output(Packet * packet)
{
	m_packet_serialize_assistant.handlePacket(packet);
}

void GateTerminal::outputPacket(Packet * packet)
{
	m_packet_serialize_assistant.handlePacket(packet);
}

int GateTerminal::session_write()
{
	int result = 0;
	if (NULL == m_current_msg_block)
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_msg_block_que_map_mutex, -1);
		if (m_msg_block_que_map.size() == 0)
		{
			return 1;
		}
		else
		{
			MsgBlockQueMap_t::iterator it = m_msg_block_que_map.find(m_msg_block_pos);
			if (it != m_msg_block_que_map.end())
			{
				MsgBlockQue_t & msg_block_que = it->second;
				m_current_msg_block = msg_block_que.front();
				msg_block_que.pop_front();
				if (msg_block_que.size() == 0)
				{
					m_msg_block_pos = increaseNumber(m_msg_block_pos, 1);
					m_msg_block_que_map.erase(it);
				}
			}
			else
			{
				return 0;
			}
		}
	}

	if (NULL != m_current_msg_block)
	{
		if (m_current_msg_block->length() > 0)
		{
			int send_n = this->peer().send(m_current_msg_block->rd_ptr(), m_current_msg_block->length());
			if (send_n <= 0)
			{
				int last_error = ACE_OS::last_error();
				if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error))
				{
					result = 0;
				}
				else
				{
					result = -1;
				}
			}
			else
			{
				m_current_msg_block->rd_ptr(send_n);
			}
		}

		if (m_current_msg_block->length() == 0)
		{
			m_packet_serialize_assistant.returnMsgBlock(m_current_msg_block);
			m_current_msg_block = NULL;
		}
	}

	if (-1 == result)
	{
		geteLostConnection();
	}

	return result;
}

//int GateTerminal::wt_stream()
//{
//	int result = 0;
//
//	{
//		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_mutex, -1);
//		if ((m_output_packet.size() == 0) && (sync_out_buf_.length() == 0))
//		{
//			return 1;
//		}
//
//		while (m_output_packet.size() > 0)
//		{
//			if (m_output_packet.size() > 3000)
//			{
//				DEF_LOG_INFO("getting gate queue size is lagrger : <%d>   ^^^^^^^^^\n", m_output_packet.size());
//			}
//
//			Packet * ps = m_output_packet.front();
//			if (ps->stream_size() < sync_out_buf_.space())
//			{
//				sync_out_buf_.copy(ps->stream(), ps->stream_size());
//				m_output_packet.pop();
//				delete ps;
//			}
//			else
//			{
//				break;
//			}
//		}
//	}
//
//	if (sync_out_buf_.length() > 0)
//	{
//		int send_n = this->peer().send(sync_out_buf_.rd_ptr(), sync_out_buf_.length());
//		if (send_n <= 0)
//		{
//			int last_error = ACE_OS::last_error();
//			if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error))
//			{
//				result = 0;
//			}
//			else
//			{
//				result = -1;
//			}
//		}
//		else
//		{
//			 sync_out_buf_.rd_ptr(send_n);
//		}
//	}
//
//	if (sync_out_buf_.length() < 500)
//	{
//		sync_out_buf_.crunch();
//	}
//
//	return result;
//}

void GateTerminal::recvError(int recv_value, int last_error)
{
	DEF_LOG_ERROR("GateTerminal recv error, recv value is <%d>, last error is <%d>\n", recv_value, last_error);
}

int GateTerminal::finit()
{
	m_packet_serialize_assistant.stop();
	return 0;
}

int GateTerminal::initing()
{
	sync_in_buf_.init(SOCK_BUFFER_LEN);
	sync_out_buf_.init(SOCK_BUFFER_LEN);

	this->peer().enable(ACE_NONBLOCK);

	int flag = 1;
	this->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

	int buf_len = 1024000;
	this->peer().set_option(IPPROTO_TCP, SO_RCVBUF, &buf_len, sizeof(buf_len));
	this->peer().set_option(IPPROTO_TCP, SO_SNDBUF, &buf_len, sizeof(buf_len));

	m_packet_serialize_assistant.init(20, this, 102400);

	return 0;
}

uint32 GateTerminal::increaseNumber(uint32 src, uint32 increase_no)
{
	return (src + increase_no) % 0xFFFFFFFF;
}

void GateTerminal::geteLostConnection()
{
	MAKE_NEW_PACKET(ps, SMSG_LOST_GATE_CONNECTION, 0, "");
	if (NULL != handle_input_)
	{
		// todo
		sync_in_buf_.rd_ptr(sync_in_buf_.base());
		sync_in_buf_.wr_ptr(sync_in_buf_.base());
		sync_in_buf_.copy(ps->stream(), ps->stream_size());
		handle_input_->IStreamIn_read(this, sync_in_buf_);
		delete ps;
	}
}
