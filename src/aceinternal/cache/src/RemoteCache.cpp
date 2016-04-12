
#include "RemoteCache.h"
#include "Logger.h"

RemoteCache::RemoteCache()
: m_is_writing(false)
{

}

RemoteCache::~RemoteCache()
{
	cleanPacketQue(m_output_packet);
}

int RemoteCache::session_on_read()
{
	int result = Session::session_on_read();
	if (-1 == result)
	{
		DEF_LOG_ERROR("occur error in RemoteCache::handle_input, rd_stream return value is -1, last error is <%d>\n", ACE_OS::last_error());
		return -1;
	}
	else
	{
		return 0;
	}
}

void RemoteCache::output(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_mutex, );
	m_output_packet.push(packet);
}

int RemoteCache::session_write()
{
	int result = 0;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_mutex, -1);
		if ((m_output_packet.size() == 0) && (m_output_msg_block.length() == 0))
		{
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
		int send_n = (int)this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
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
			m_output_msg_block.rd_ptr(send_n);
		}
	}

	if (m_output_msg_block.length() < 500)
	{
		m_output_msg_block.crunch();
	}

	return result;
}

void RemoteCache::session_recvError(int recv_value, int last_error)
{
	DEF_LOG_ERROR("RemoteCache recv error, recv value is <%d>, last error is <%d>\n", recv_value, last_error);
}

int RemoteCache::finit()
{
	return 0;
}

int RemoteCache::initing()
{
	return 0;
}
