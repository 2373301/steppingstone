
#include "CellSession.h"
#include "ManageNetEventNotify.h"

namespace netstream
{

CellSession::CellSession()
: m_guid(0)
{
	m_outpu_buffer.init(m_socket_buffer_length);
}

CellSession::~CellSession()
{

}

int CellSession::open(void * p)
{
	int result = Session::open(p);
	if (-1 != result)
	{
		//notifySessionPool(this, SS_CONNECTED);
		ManageNetEventNotify::instance()->handleNewSession(this);
	}

	//m_save_output_pack_info.init(true, "output_file");

	return result;
}

int CellSession::rd_stream()
{
	int result = Session::rd_stream();
	if (-1 == result)
	{
		// net close
		ManageNetEventNotify::instance()->handleSessionClose(this, ACE_Event_Handler::READ_MASK);
	}

	return result;
}

bool CellSession::output(char * buffer, int buff_size)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_outpu_buffer_mutex, false);
	//m_output_packet.push(packet);
	if (m_outpu_buffer.space() >= buff_size)
	{
		m_outpu_buffer.copy(buffer, buff_size);
		return true;
	}
	else
	{
		return false;
	}
}

int CellSession::wt_stream()
{
	int result = 0;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_outpu_buffer_mutex, -1);
		if ((m_outpu_buffer.length() == 0) && (m_output_msg_block.length() == 0))
		{
			// no data to send
			return 1;
		}

		if ((m_output_msg_block.space() > 0) && (m_outpu_buffer.length() > 0))
		{
			auto copy_size = m_outpu_buffer.length() < m_output_msg_block.space() ? m_outpu_buffer.length() : m_output_msg_block.space();
			m_output_msg_block.copy(m_outpu_buffer.rd_ptr(), copy_size);
			m_outpu_buffer.rd_ptr(copy_size);
			m_outpu_buffer.crunch();
		}
	}

	if (m_output_msg_block.length() > 0)
	{
		auto send_n = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
		if (send_n <= 0)
		{
			int last_error = ACE_OS::last_error();
			if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error))
			{
				result = 0;
			}
			else
			{
				this->shutdown();
				result = -1;
			}
		}
		else
		{
			//m_save_output_pack_info.save(m_output_msg_block.rd_ptr(), send_n);

			m_output_msg_block.rd_ptr(send_n);

			if (m_output_msg_block.length() == 0)
			{
				result = 0;
			}
			else
			{
				result = 2;
			}
		}
	}

	if (m_output_msg_block.length() < 500)
	{
		m_output_msg_block.crunch();
	}

	//if (m_output_msg_block.length() == 0)
	//{
	//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_mutex, -1);
	//	if (m_output_packet.size() == 0)
	//	{
	//		//this->reactor()->remove_handler(this, ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
	//		result = -1;
	//	}
	//}

	if (-1 == result)
	{
		ManageNetEventNotify::instance()->handleSessionClose(this, ACE_Event_Handler::WRITE_MASK);
	}

	return result;
}

uint64 CellSession::getGUID()
{
	return m_guid;
}

void CellSession::setGUID(uint64 guid)
{
	m_guid = guid;
}

}
