
#include "SingleTerminal.h"
#include "Logger.h"
#include "ManageSingleTerminal.h"
#include "opcode.h"

SingleTerminal::SingleTerminal()
: m_player_guid(0)
{
}

bool SingleTerminal::IStream_output(char * buffer, int buff_size)
{
	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_msg_mutex, );
	//if (out_buf_.space() < packet->stream_size())
	//{
	//	if (out_buf_.length() * 2 > out_buf_.capacity())
	//	{
	//		delete packet;
	//		return ;
	//	}
	//	int msg_length = out_buf_.length();
	//	memcpy(out_buf_.base(), out_buf_.rd_ptr(), msg_length);
	//	out_buf_.rd_ptr(out_buf_.base());
	//	out_buf_.wr_ptr(out_buf_.base() + msg_length);
	//}

	//out_buf_.copy(packet->stream(), packet->stream_size());
	//this->reactor()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
	//delete packet;

	return false;
}

void SingleTerminal::outputPacket(Packet * packet)
{
	
}

int SingleTerminal::session_write()
{
	int result = 0;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_msg_mutex, -1);
	if (out_buf_.length() == 0)
	{
		if (SS_CLOSED == session_state_)
		{
			return -1;
		}
		return 2;
	}

	int send_n = this->peer().send(out_buf_.rd_ptr(), out_buf_.length());
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
		out_buf_.rd_ptr(send_n);
		result = 0;
	}

	if (out_buf_.length() == 0)
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
		if (in_buf_.length() < Packet::head_size())
		{
			return;
		}

		Packet * ps = new Packet();
		ps->setOwner(this);
		memcpy(ps->ch_head(), in_buf_.rd_ptr(), ps->head_size());
		m_player_guid = ps->guid();
		if (in_buf_.length() >= ps->head_size() + ps->body_size())
		{
			in_buf_.rd_ptr(ps->head_size());
			memcpy(ps->ch_body(), in_buf_.rd_ptr(), ps->body_size());
			in_buf_.rd_ptr(ps->body_size());
			if (NULL != handle_input_)
			{
				// todo
				//handle_input_->input(this, ps);
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

int SingleTerminal::session_on_connected()
{
	Terminal::session_on_connected();

	ManageSingleTerminal::instance()->registerSingleTerminal(this);
	return 0;
}

int SingleTerminal::session_on_closed()
{
	Terminal::session_on_closed();

	m_close_time = ACE_OS::gettimeofday();

	MAKE_NEW_PACKET(ps, CMSG_PLAYER_LEAVE_MAP, m_player_guid, "");
//	handle_input_->input(ps);

	ManageSingleTerminal::instance()->removeSingleTerminal(this);
	return 0;
}
