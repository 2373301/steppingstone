
#include "ManageClientSessionCooler.h"
#include "ManageConfig.h"

ManageClientSessionCooler::ManageClientSessionCooler()
: m_stop(false)
, m_wait(true)
, m_cooler_time(180)
{
}

ManageClientSessionCooler::~ManageClientSessionCooler()
{
	for (ClientSessionList::iterator it = m_client_session_list.begin(); it != m_client_session_list.end(); ++it)
	{
		delete *it;
	}
	m_client_session_list.clear();

	for (ClientSessionList::iterator it = m_cleaned_client_session_list.begin(); it != m_cleaned_client_session_list.end(); ++it)
	{
		delete *it;
	}
	m_cleaned_client_session_list.clear();
}

int ManageClientSessionCooler::svc(void)
{
	FUNCTIONTRACE(ManageClientSessionCooler::svc);
	REPORT_THREAD_INFO("ManageClientSessionCooler::svc", ACE_OS::thr_self());
	ACE_Time_Value current_time;
	ACE_Time_Value last_check_time;
	ACE_Time_Value deff_time;
	ClientSession * client_session;
	ClientSessionList should_clean_client;
	ACE_Time_Value sleep_time(0, 10 * 1000);
	while (!m_stop)
	{
		{
			ACE_OS::sleep(sleep_time);

			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_mutex, 1);

			for (ClientSessionList::iterator it = m_client_session_list.begin(); it != m_client_session_list.end(); ++it)
			{
				if (!isInCleanList(*it))
				{
					should_clean_client.push_back(*it);
				}
			}
			m_client_session_list.clear();
		}

		for (ClientSessionList::iterator it = should_clean_client.begin(); it != should_clean_client.end(); ++it)
		{
			if (!isInCleanList(*it))
			{
				cleanClientSession(*it);
				putInCleanList(*it);
			}
		}
		should_clean_client.clear();

		current_time = ACE_OS::gettimeofday();
		if (current_time.sec() - last_check_time.sec() < 1)
		{
			continue;
		}

		if (m_cleaned_client_session_list.size() == 0)
		{
			continue;
		}

		// check cooler timeout client session
		last_check_time = current_time;
		for (ClientSessionList::iterator it = m_cleaned_client_session_list.begin(); it != m_cleaned_client_session_list.end(); )
		{
			client_session = *it;
			deff_time = current_time - client_session->getHandleCloseTime();
			if (deff_time.sec() > m_cooler_time)
			{
				it = m_cleaned_client_session_list.erase(it);
				client_session->closeHandle();
				GATE_LOG_INFO("start to delete client session <%x>\n", client_session);
				delete client_session;
				client_session = NULL;
			}
			else
			{
				++it;
			}
		}
	}

	m_wait = false;
	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageClientSessionCooler::svc, last error is <%d>\n"), ACE_OS::last_error());
	return 0;
}

int ManageClientSessionCooler::init(int argc, ACE_TCHAR * argv[])
{
	m_cooler_time = ManageConfig::instance()->getClientCfg().cooler_time_interval;
	if (this->activate() == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call activate in ManageClientSessionCooler::init, the last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init ManageClientSessionCooler\n"));
		return 0;
	}
}

int ManageClientSessionCooler::fini()
{
	m_stop = true;
	return 0;
}

int ManageClientSessionCooler::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageClientSessionCooler\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

void ManageClientSessionCooler::putClientSession(ClientSession * client_session)
{
	GATE_LOG_INFO(ACE_TEXT("enter ManageClientSessionCooler::putClientSession, 111, pointer is <%x>, guid is <%llu>\n"), (void *)client_session, client_session->getRoleGuid());

	if (client_session->alreadyInCooler())
	{
		return ;
	}

	client_session->putInCooler();

	GATE_LOG_INFO(ACE_TEXT("enter ManageClientSessionCooler::putClientSession, pointer is <%x>, guid is <%llu>\n"), (void *)client_session, client_session->getRoleGuid());
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_mutex, );
	ClientSessionList::iterator it = std::find(m_client_session_list.begin(), m_client_session_list.end(), client_session);
	if (m_client_session_list.end() == it)
	{
		m_client_session_list.push_back(client_session);
	}
}

void ManageClientSessionCooler::putInCleanList(ClientSession * client_session)
{
	m_cleaned_client_session_list.push_back(client_session);
}

bool ManageClientSessionCooler::isInCleanList(ClientSession * client_session)
{
	ClientSessionList::iterator it = std::find(m_cleaned_client_session_list.begin(), m_cleaned_client_session_list.end(), client_session);
	return m_cleaned_client_session_list.end() != it ? true : false;
}

void ManageClientSessionCooler::cleanClientSession(ClientSession * client_session)
{
	GATE_LOG_INFO(ACE_TEXT("Start to call handle_close of client session, the pointer is <%x>, account id is <%s>\n"), (void *)client_session, client_session->getAccountID().c_str());
	client_session->handle_close(NULL, 0);
}