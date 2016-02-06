
#include "CellSession.h"
#include "SessionPoolAssistent.h"

namespace netcore
{

CellSession::CellSession()
: m_guid(0)
{
}

CellSession::~CellSession()
{

}

int CellSession::open(void * p)
{
	int result = Session::open(p);
	if (-1 != result)
	{
		notifySessionPool(this, SS_CONNECTED);
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
		notifySessionPool(this, SS_CLOSE);
	}

	return result;
}

void CellSession::output(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_mutex, );
	m_output_packet.push(packet);
}

int CellSession::wt_stream()
{
	int result = 0;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_mutex, -1);
		if ((m_output_packet.size() == 0) && (m_output_msg_block.length() == 0))
		{
			// no data to send
			return 1;
		}

		while (m_output_packet.size() > 0)
		{
			Packet * ps = m_output_packet.front();
			if (ps->stream_size() < m_output_msg_block.space())
			{
				m_output_msg_block.copy(ps->stream(), ps->stream_size());
				m_output_packet.pop();
				delete ps;
			}
			else
			{
				break;
			}
		}
	}

	if (m_output_msg_block.length() > 0)
	{
		ssize_t send_n = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
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
