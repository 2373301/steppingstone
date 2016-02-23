
#include <ace/Select_Reactor.h>
#include "ManageGSSession.h"
#include "GateLogger.h"
#include "ManageConfig.h"

ManageGSSession::ManageGSSession()
: m_stop(false)
, m_wait(true)
, m_timer_id(0)
, m_reactor(NULL)
{
}

ManageGSSession::~ManageGSSession()
{
	delete m_reactor;
	m_reactor = NULL;
}

int ManageGSSession::svc(void)
{
	FUNCTIONTRACE(ManageGSSession::svc);
	REPORT_THREAD_INFO("ManageGSSession::svc", ACE_OS::thr_self());
	/*
		这个函数只允许进入一次，用于运行处理接收GS请求的Reactor。
		正常情况下，ManageGSSession只会被初始化1个线程。
	*/
	ACE_GUARD_RETURN(ACE_Thread_Mutex, mutex_guard, m_mutex, 1);
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);

//	try
	{
		GSAcceptorInstance gs_acceptor(m_reactor);
		ACE_INET_Addr port_to_listen(sMsgCfg->getGSCfg().listen_port, sMsgCfg->getGSCfg().host.c_str());
		if (gs_acceptor.open(port_to_listen, m_reactor) == -1)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to call client_acceptor.open, the port is <%d>, last error is <%d>\n"), sMsgCfg->getClientCfg().listen_port, ACE_OS::last_error());
			return 1;
		}

		//注册一个timer，用于检查是否需要退出
		m_timer_id = m_reactor->schedule_timer(this, NULL, ACE_Time_Value(3, 0), ACE_Time_Value(3, 0));
		if (m_timer_id < 0)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to register timer in ManageClientSession::svc, the last error is <%d>"), ACE_OS::last_error());
			return 1;
		}

		//m_reactor->run_reactor_event_loop();
		while (true)
		{
			m_reactor->run_reactor_event_loop();
#ifdef WIN32
			break;
#else
			if (ACE_OS::last_error() == EINTR)
			{
				continue;
			}
			else
			{
				break;
			}
#endif
		}

	}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in ManageGSSession::svc, last error is <%d>\n"), ACE_OS::last_error());
	//	return 2;
	//}

	m_wait = false;
	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageGSSession::svc, last error is <%d>\n"), ACE_OS::last_error());
	return 0;
}

int ManageGSSession::handle_timeout(const ACE_Time_Value &current_time, const void *act)
{
	if (m_stop)
	{
		this->reactor()->cancel_timer(m_timer_id);
		this->reactor()->end_reactor_event_loop();
	}
	return 0;
}

int ManageGSSession::init(int argc, ACE_TCHAR * argv[])
{
	int r = this->activate(THR_JOINABLE, 1);
	if (r < 0)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call ManageGSSession::init, the last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init ManageGSSession.\n"));
		return 0;
	}
}

int ManageGSSession::fini()
{
	m_stop = true;
	m_reactor->end_reactor_event_loop();
	return 0;
}

int ManageGSSession::wait()
{
	closeGSSession();
	super::wait();
	if (NULL != m_reactor)
	{
		delete m_reactor;
		m_reactor = NULL;
	}
	return 0;
}

void ManageGSSession::addGSSession(GSSession * gs_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gs_session_mutex, );
	m_gs_session_list.push_back(gs_session);
}

void ManageGSSession::removeGSSession(GSSession * gs_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gs_session_mutex, );
	GSSessionList::iterator it = std::find(m_gs_session_list.begin(), m_gs_session_list.end(), gs_session);
	if (it != m_gs_session_list.end())
	{
		m_gs_session_list.erase(it);
	}
}

void ManageGSSession::closeGSSession()
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gs_session_mutex, );
	for (GSSessionList::iterator it = m_gs_session_list.begin(); it != m_gs_session_list.end(); ++it)
	{
		(*it)->peer().close();
	}
}