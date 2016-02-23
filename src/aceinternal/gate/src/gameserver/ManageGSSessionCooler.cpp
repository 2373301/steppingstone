
#include "ManageGSSessionCooler.h"

ManageGSSessionCooler::ManageGSSessionCooler()
: m_stop(false)
, m_wait(true)
, m_cooler_time(60)
{
}

ManageGSSessionCooler::~ManageGSSessionCooler()
{
	for (GSSessionList::iterator it = m_gs_session_list.begin(); it != m_gs_session_list.end(); ++it)
	{
		delete *it;
	}
	m_gs_session_list.clear();

	for (GSSessionList::iterator it = m_gs_cleaned_session_list.begin(); it != m_gs_cleaned_session_list.end(); ++it)
	{
		delete *it;
	}
	m_gs_cleaned_session_list.clear();
}

int ManageGSSessionCooler::svc(void)
{
	FUNCTIONTRACE(ManageGSSessionCooler::svc);
	REPORT_THREAD_INFO("ManageGSSessionCooler::svc", ACE_OS::thr_self());
	ACE_Time_Value current_time;
	ACE_Time_Value deff_time;
	
	while (!m_stop)
	{
		while (!m_stop)
		{
			ACE_OS::sleep(1);
			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gs_session_mutex, 1);
				if ((m_gs_session_list.size() == 0) && (m_gs_cleaned_session_list.size() == 0))
				{
					break;
				}
			}

			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gs_session_mutex, 1);
				//gs_session = *m_gs_session_list.begin();
				//m_gs_session_list.pop_front();
				for (GSSessionList::iterator it = m_gs_session_list.begin(); it != m_gs_session_list.end(); ++it)
				{
					GSSessionList::iterator it_find = std::find(m_gs_cleaned_session_list.begin(), m_gs_cleaned_session_list.end(), *it);
					if (m_gs_cleaned_session_list.end() == it_find)
					{
						(*it)->handle_close(0, 0);
						m_gs_cleaned_session_list.push_back(*it);
					}
				}
				m_gs_session_list.clear();
			}

			//gs_session->handle_close(0, 0);
			current_time = ACE_OS::gettimeofday();
			for (GSSessionList::iterator it = m_gs_cleaned_session_list.begin(); it != m_gs_cleaned_session_list.end(); )
			{
				deff_time = current_time - (*it)->getHandleCloseTime();
				if (deff_time.sec() > m_cooler_time)
				{
					delete *it;
					it = m_gs_cleaned_session_list.erase(it);
				}
				else
				{
					++it;
				}
			}
			break;
		}
	}

	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageGSSessionCooler::svc, last error is <%d>\n"), ACE_OS::last_error());

	m_wait = false;
	return 0;
}

int ManageGSSessionCooler::init(int argc, ACE_TCHAR * argv[])
{
	if (this->activate() == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call activate in ManageGSSessionCooler::init, last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init ManageGSSessionCooler\n"));
		return 0;
	}
}

int ManageGSSessionCooler::fini()
{
	m_stop = true;
	return 0;
}

int ManageGSSessionCooler::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageGSSessionCooler\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

void ManageGSSessionCooler::putGSSession(GSSession * gs_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gs_session_mutex, );
	m_gs_session_list.push_back(gs_session);
}
