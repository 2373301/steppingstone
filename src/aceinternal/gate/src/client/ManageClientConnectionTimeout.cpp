
#include "ManageClientConnectionTimeout.h"
#include "ManageConfig.h"
#include "GateLogger.h"
#include "ClientStrategyCenter.h"

ManageClientConnectionTimeout::ManageClientConnectionTimeout()
: m_stop(false)
, m_wait(true)
{

}

ManageClientConnectionTimeout::~ManageClientConnectionTimeout()
{

}

int ManageClientConnectionTimeout::svc(void)
{
	FUNCTIONTRACE(ManageClientConnectionTimeout::svc);
	REPORT_THREAD_INFO("ManageClientConnectionTimeout::svc", ACE_OS::thr_self());

	int client_connection_timeout = sMsgCfg->getClientCfg().client_connection_timeout;

	ClientSessionDeque session_deque;
	ClientSessionDeque session_deque_check_delete;

	ClientSessionSet avilable_session_set;

//	ClientSessionList remove_session_list;

	while (!m_stop)
	{
		ACE_OS::sleep(3);
		// todo
		continue;
		//break;
		{
			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_mutex, -1);
				std::copy(m_client_session_deque.begin(), m_client_session_deque.end(), back_inserter(session_deque));
				m_client_session_deque.clear();
			}

			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_remove_client_session_set_mutex, false);
				std::copy(m_remove_client_session_set.begin(), m_remove_client_session_set.end(), std::inserter(m_remove_client_session_set_copy, m_remove_client_session_set_copy.end()));
				m_remove_client_session_set.clear();
			}

			ACE_Time_Value current_time = ACE_OS::gettimeofday();

			ACE_Time_Value diff_time;

			session_deque_check_delete.clear();
			std::copy(session_deque.begin(), session_deque.end(), back_inserter(session_deque_check_delete));
			session_deque.clear();

			avilable_session_set.clear();

			for(ClientSessionDeque::iterator it = session_deque_check_delete.begin(); it != session_deque_check_delete.end(); ++it)
			{
				ClientSession * client_session = *it;
				if (isRemovedSession(client_session, true))
				{
					GATE_LOG_INFO(ACE_TEXT("remove client session in ManageClientConnectionTimeout::svc, removed, pointer is <%x>\n"), client_session);
				}
				else
				{
					session_deque.push_back(client_session);
					avilable_session_set.insert(client_session);
				}
			}

			checkDeleteSession(avilable_session_set);

			while (! session_deque.empty())
			{
				ClientSession * client_session = session_deque.front();
				//if (isRemovedSession(client_session, true))
				//{
				//	GATE_LOG_INFO(ACE_TEXT("remove client session in ManageClientConnectionTimeout::svc, removed, pointer is <%x>\n"), client_session);
				//	session_deque.pop_front();
				//}
				////else if (client_session->getValidation())
				////{
				////	GATE_LOG_INFO(ACE_TEXT("remove client session in ManageClientConnectionTimeout::svc, validation, pointer is <%x>\n"), client_session);
				////	session_deque.pop_front();
				////}
				//else
				{
					diff_time = current_time - client_session->getConnectionTime();
					if (diff_time.sec() > client_connection_timeout)
					{
						// timeout todo
						GATE_LOG_INFO(ACE_TEXT("remove client session in ManageClientConnectionTimeout::svc, timeout, pointer is <%x>\n"), client_session);
						clientSessionTimeout(client_session);
						session_deque.pop_front(); 
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	m_wait = false;
	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageClientConnectionTimeout::svc, last error <%d>\n"), ACE_OS::last_error());
	return 0;
}

int ManageClientConnectionTimeout::init(int argc, ACE_TCHAR * argv[])
{
	if (this->activate() == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call activate in ManageClientConnectionTimeout::init, last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init ManageClientConnectionTimeout\n"));
		return 0;
	}
}

int ManageClientConnectionTimeout::fini()
{
	m_stop = true;
	return 0;
}

int ManageClientConnectionTimeout::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageClientConnectionTimeout\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

void ManageClientConnectionTimeout::putClientSession(ClientSession * client_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_mutex, );
	m_client_session_deque.push_back(client_session);
}

void ManageClientConnectionTimeout::removeClientSession(ClientSession * client_session)
{
	//if (client_session->getConnectionTimeout())
	//{
	//	return ;
	//}
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_remove_client_session_set_mutex, );
	m_remove_client_session_set.insert(client_session);
	GATE_LOG_INFO(ACE_TEXT("remove client session in ManageClientConnectionTimeout::removeClientSession, pointer is <%x>\n"), client_session);
}

void ManageClientConnectionTimeout::clientSessionTimeout(ClientSession * client_session)
{
	ClientStrategyCenter::instance()->clientConnectionTimeout(client_session);
}

bool ManageClientConnectionTimeout::isRemovedSession(ClientSession * client_session, bool remove_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_remove_client_session_set_mutex, false);
	ClientSessionSet::iterator it = m_remove_client_session_set_copy.find(client_session);
	if (it != m_remove_client_session_set_copy.end())
	{
		if (remove_session)
		{
			m_remove_client_session_set_copy.erase(it);
		}
		return true;
	}
	
	return false;
}

void ManageClientConnectionTimeout::checkDeleteSession(ClientSessionSet & avilable_session_set)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_remove_client_session_set_mutex, );
	for (ClientSessionSet::iterator it = m_remove_client_session_set_copy.begin(); it != m_remove_client_session_set_copy.end(); )
	{
		if (avilable_session_set.find(*it) == avilable_session_set.end())
		{
			m_remove_client_session_set_copy.erase(it++);
		}
		else
		{
			++it;
		}
	}
}