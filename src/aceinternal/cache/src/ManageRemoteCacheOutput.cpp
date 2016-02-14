
#include <ace/OS.h>
#include "Logger.h"
#include "ManageRemoteCacheOutput.h"

ManageRemoteCacheOutput::ManageRemoteCacheOutput()
: m_is_stop(false)
{
}

ManageRemoteCacheOutput::~ManageRemoteCacheOutput()
{}

int ManageRemoteCacheOutput::init(int thread_no)
{
	if (this->activate(THR_NEW_LWP, thread_no) == -1)
	{
		DEF_LOG_ERROR("failed to activate ManageRemoteCacheOutput, thread no is <%d>, last error is <%d>\n", thread_no, ACE_OS::last_error());
		return -1;
	}

	return 0;
}

int ManageRemoteCacheOutput::stop()
{
	m_is_stop = true;
	return 0;
}

int ManageRemoteCacheOutput::svc()
{
	REPORT_THREAD_INFO();

	RemoteCache * remote_cache = NULL;

	while(!m_is_stop)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_remote_cache_qeque_mutex, -1);
			if (m_remote_cache_qeque.size() > 0)
			{
				remote_cache = m_remote_cache_qeque.front();
				m_remote_cache_qeque.pop_front();
				break;
			}

		}
		ACE_OS::sleep(1);
	}

	ACE_Time_Value sleep_time(0, 1 * 1000);

	int process_result = 0;
	while (!m_is_stop)
	{
		process_result = remote_cache->wt_stream();

		if (1 == process_result)
		{
			ACE_OS::sleep(sleep_time);
		}
	}
	return 0;
}

void ManageRemoteCacheOutput::handleRemoteCache(RemoteCache * remote_cache)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_remote_cache_qeque_mutex, );
	m_remote_cache_qeque.push_back(remote_cache);
}
