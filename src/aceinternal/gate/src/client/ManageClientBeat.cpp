
#include "ManageClientBeat.h"
#include "GateLogger.h"
#include "ManageConfig.h"
#include "opcode.h"
#include "ClientStrategyCenter.h"

ManageClientBeat::ManageClientBeat()
: m_stop(false)
, m_wait(true)
, m_check_interval(30)
, m_max_lost_beat(5)
, m_beat_interval(30)
{
}

ManageClientBeat::~ManageClientBeat()
{
}

int ManageClientBeat::svc(void)
{
	FUNCTIONTRACE(ManageClientBeat::svc);
	REPORT_THREAD_INFO("ManageClientBeat::svc", ACE_OS::thr_self());
	ClientSessionList put_list;
	ClientSessionList remove_list;
	ACE_Time_Value start_to_check_time = ACE_OS::gettimeofday();
	ACE_Time_Value diff_time;
	int sleep_time;
	while (!m_stop)
	{
		diff_time = (ACE_OS::gettimeofday() - start_to_check_time);
		sleep_time = m_check_interval - diff_time.sec();
		if (sleep_time > 0)
		{
			ACE_OS::sleep(sleep_time);
		}
		
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_copy_mutex, -1);
			start_to_check_time = ACE_OS::gettimeofday();
			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_beat_put_mutex, -1);
				put_list = m_client_session_list_put;
				remove_list = m_client_session_list_remove;
				m_client_session_list_put.clear();
				m_client_session_list_remove.clear();
			}

//			std::copy(put_list.begin(), put_list.end(), std::back_inserter(m_client_session_list));
			for (ClientSessionList::iterator it = put_list.begin(); it != put_list.end(); ++it)
			{
				ClientSessionList::iterator add_it = std::find(m_client_session_list.begin(), m_client_session_list.end(), *it);
				if (m_client_session_list.end() == add_it)
				{
					m_client_session_list.push_back(*it);
				}
			}

			for (ClientSessionList::iterator it = remove_list.begin(); it != remove_list.end(); ++it)
			{
				ClientSessionList::iterator remove_it = std::find(m_client_session_list.begin(), m_client_session_list.end(), *it);
				if (remove_it != m_client_session_list.end())
				{
					m_client_session_list.erase(remove_it);
				}
			}

			ACE_Time_Value current_time = ACE_OS::gettimeofday();
			for (ClientSessionList::iterator it = m_client_session_list.begin(); it != m_client_session_list.end(); )
			{
				if (lastBeatTimeout(current_time, *it))
				{
					// todo
					ClientSession * client_session = *it;
					PacketStream * ps = new PacketStream(GMSG_CLIENT_BEAT_TIMEOUT, client_session->getRoleGuid(), "");
					client_session->putInputPacket(ps);
					it = m_client_session_list.erase(it);
				}
				else
				{
					++it;
				}
			}
			put_list.clear();
			remove_list.clear();
			m_client_session_list_copy.clear();
			std::copy(m_client_session_list.begin(), m_client_session_list.end(), std::back_inserter(m_client_session_list_copy));
		}

	}

	m_wait = false;
	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageClientBeat::svc, last error is <%d>\n"), ACE_OS::last_error());
	return 0;
}

int ManageClientBeat::init(int argc, ACE_TCHAR * argv[])
{
	m_check_interval = ManageConfig::instance()->getClientCfg().beat_check_interval;
	m_max_lost_beat = ManageConfig::instance()->getClientCfg().lost_beat_time;
	m_beat_interval = ManageConfig::instance()->getClientCfg().beat_interval;
	m_max_timeout_second = m_beat_interval * m_max_lost_beat;

	if (this->activate() == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to init ManageClientBeat, last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init ManageClientBeat\n"));
		return 0;
	}
}

int ManageClientBeat::fini()
{
	m_stop = true;
	return 0;
}

int ManageClientBeat::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageClientBeat\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

void ManageClientBeat::addClient(ClientSession * client_session)
{
	GATE_LOG_INFO(ACE_TEXT("enter ManageClientBeat::addClient, the pointer is <%x>\n"), (void *)client_session);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_beat_put_mutex, );
	m_client_session_list_put.push_back(client_session);
}

void ManageClientBeat::removeClient(ClientSession * client_session)
{
	GATE_LOG_INFO(ACE_TEXT("enter ManageClientBeat::removeClient, the pointer is <%x>\n"), (void *)client_session);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_beat_put_mutex, );
	m_client_session_list_remove.push_back(client_session);
}

void ManageClientBeat::getOnlineClient(ClientSessionList & online_client_list)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_copy_mutex,  );
	std::copy(m_client_session_list_copy.begin(), m_client_session_list_copy.end(), std::back_inserter(online_client_list));
}

bool ManageClientBeat::lastBeatTimeout(const ACE_Time_Value & current_time, ClientSession * client_session)
{
	return (current_time.sec() - client_session->getLastBeatTime()) > m_max_timeout_second;
}