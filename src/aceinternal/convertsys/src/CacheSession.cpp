
#include "CacheSession.h"

CacheSession::CacheSession()
{
}

CacheSession::~CacheSession()
{
}

void CacheSession::output(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_mutex, );
	m_output_packet.push(packet);
}

int CacheSession::wt_stream()
{
	int result = 0;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_mutex, -1);
		if (m_output_packet.size() == 0)
		{
			return 1;
		}

		while (m_output_packet.size() > 0)
		{
			Packet * ps = m_output_packet.front();
			if (ps->stream_size() < m_outputs.space())
			{
				m_outputs.copy(ps->stream(), ps->stream_size());
				m_output_packet.pop();
				delete ps;
			}
			else
			{
				break;
			}
		}
	}

	if (m_outputs.length() > 0)
	{
		int send_n = (int)this->peer().send(m_outputs.rd_ptr(), m_outputs.length());
		if (send_n <= 0)
		{
			int last_error = ACE_OS::last_error();
			if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error))
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			m_outputs.rd_ptr(send_n);
		}
	}

	if ((m_outputs.length() < 100) || (m_outputs.space() < 1000))
	{
		m_outputs.crunch();
	}

	if (m_outputs.length() == 0)
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_mutex, -1);
		if (m_output_packet.size() == 0)
		{
			//this->reactor()->remove_handler(this, ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
			result = -1;
		}
	}

	return result;
}
