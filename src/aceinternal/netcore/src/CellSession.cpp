
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
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_deferred_outputs_mutex, );
	m_deferred_outputs.push(packet);
}

int CellSession::wt_stream()
{
	int result = 0;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_deferred_outputs_mutex, -1);
		if ((m_deferred_outputs.size() == 0) && (m_outputs.length() == 0))
		{
			// no data to send
			return 1;
		}

		while (m_deferred_outputs.size() > 0)
		{
			Packet * ps = m_deferred_outputs.front();
			if (ps->stream_size() < m_outputs.space())
			{
				m_outputs.copy(ps->stream(), ps->stream_size());
				m_deferred_outputs.pop();
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
		ssize_t send_n = this->peer().send(m_outputs.rd_ptr(), m_outputs.length());
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
			//m_save_output_pack_info.save(m_outputs.rd_ptr(), send_n);

			m_outputs.rd_ptr(send_n);
		}
	}

	if (m_outputs.length() < 500)
	{
		m_outputs.crunch();
	}

	//if (m_outputs.length() == 0)
	//{
	//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_deferred_outputs_mutex, -1);
	//	if (m_deferred_outputs.size() == 0)
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
