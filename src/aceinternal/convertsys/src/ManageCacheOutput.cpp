
#include <ace/OS.h>
#include "ManageCacheOutput.h"
#include "Logger.h"

int ManageCacheOutput::init(int thread_no)
{
	if (this->activate(THR_NEW_LWP, thread_no) == -1)
	{
		DEF_LOG_ERROR("failed to activate ManageCacheOutput, thread no is <%d>, last error is <%d>\n", thread_no, ACE_OS::last_error());
		return -1;
	}

	return 0;
}

int ManageCacheOutput::svc()
{
	CacheSession * cache_session = NULL;

	while(true)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cache_session_qeque_mutex, -1);
			if (m_cache_session_qeque.size() > 0)
			{
				cache_session = m_cache_session_qeque.front();
				m_cache_session_qeque.pop_front();
				break;
			}

		}
		ACE_OS::sleep(1);
	}

	ACE_Time_Value sleep_time(0, 5 * 1000);

	int process_result = 0;
	while (true)
	{
		process_result = cache_session->wt_stream();

		if (1 == process_result)
		{
			ACE_OS::sleep(sleep_time);
		}
	}
	return 0;
}

void ManageCacheOutput::handleCache(CacheSession * cache_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cache_session_qeque_mutex, );
	m_cache_session_qeque.push_back(cache_session);
}
