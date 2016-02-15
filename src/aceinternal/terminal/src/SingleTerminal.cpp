
#include "SingleTerminal.h"
#include "Logger.h"
#include "ManageSingleTerminal.h"
#include "opcode.h"

SingleTerminal::SingleTerminal()
: m_player_guid(0)
{
}

bool SingleTerminal::output(char * buffer, int buff_size)
{
	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_msg_mutex, );
	//if (m_output_msg_block.space() < packet->stream_size())
	//{
	//	if (m_output_msg_block.length() * 2 > m_output_msg_block.capacity())
	//	{
	//		delete packet;
	//		return ;
	//	}
	//	int msg_length = m_output_msg_block.length();
	//	memcpy(m_output_msg_block.base(), m_output_msg_block.rd_ptr(), msg_length);
	//	m_output_msg_block.rd_ptr(m_output_msg_block.base());
	//	m_output_msg_block.wr_ptr(m_output_msg_block.base() + msg_length);
	//}

	//m_output_msg_block.copy(packet->stream(), packet->stream_size());
	//this->reactor()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
	//delete packet;

	return false;
}

void SingleTerminal::outputPacket(Packet * packet)
{
	
}

int SingleTerminal::wt_stream()
{
	int result = 0;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_msg_mutex, -1);
	if (m_output_msg_block.length() == 0)
	{
		if (SS_CLOSE == m_session_state)
		{
			return -1;
		}
		return 2;
	}

	int send_n = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
	if (send_n <= 0)
	{
		if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
		{
			result = 1;
		}
		else
		{
			result = -1;
			DEF_LOG_WARNING(ACE_TEXT("error to send data in GSSession::wt_stream, send return value is <%d>, last error is <%d>\n"), send_n, ACE_OS::last_error());
		}
	}
	else
	{
		m_output_msg_block.rd_ptr(send_n);
		result = 0;
	}

	if (m_output_msg_block.length() == 0)
	{
		this->reactor()->remove_handler(this, ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
	}
	return result;
}


int SingleTerminal::finit()
{
	return 0;
}

uint64 SingleTerminal::getPlayerGuid()
{
	return m_player_guid;
}

const ACE_Time_Value & SingleTerminal::getCloseTime()
{
	return m_close_time;
}

int SingleTerminal::initing()
{
	return 0;
}

void SingleTerminal::parseInputPacket()
{
	while (true)
	{
		if (m_input_msg_block.length() < Packet::head_size())
		{
			return;
		}

		Packet * ps = new Packet();
		ps->setOwner(this);
		memcpy(ps->ch_head(), m_input_msg_block.rd_ptr(), ps->head_size());
		m_player_guid = ps->guid();
		if (m_input_msg_block.length() >= ps->head_size() + ps->body_size())
		{
			m_input_msg_block.rd_ptr(ps->head_size());
			memcpy(ps->ch_body(), m_input_msg_block.rd_ptr(), ps->body_size());
			m_input_msg_block.rd_ptr(ps->body_size());
			if (NULL != m_handle_input)
			{
				// todo
				//m_handle_input->input(this, ps);
			}
			else
			{
				// fatal error
			}
		}
		else
		{
			delete ps;
			break;
		}
	}
}

int SingleTerminal::net_connected()
{
	Terminal::net_connected();

	ManageSingleTerminal::instance()->registerSingleTerminal(this);
	return 0;
}

int SingleTerminal::net_closed()
{
	Terminal::net_closed();

	m_close_time = ACE_OS::gettimeofday();

	MAKE_NEW_PACKET(ps, CMSG_PLAYER_LEAVE_MAP, m_player_guid, "");
//	m_handle_input->input(ps);

	ManageSingleTerminal::instance()->removeSingleTerminal(this);
	return 0;
}
