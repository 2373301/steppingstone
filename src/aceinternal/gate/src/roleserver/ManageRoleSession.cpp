#include <ace/OS.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "GateLogger.h"
#include "ManageConfig.h"
#include "ManageRoleSession.h"
#include "RoleSession.h"

ManageRoleSession::ManageRoleSession()
: m_stop(false)
, m_wait(true)
, m_timer_id(0)
, m_reactor(0)
{
}

ManageRoleSession::~ManageRoleSession()
{
	if (NULL != m_reactor)
	{
		delete m_reactor;
		m_reactor = NULL;
	}
}

int ManageRoleSession::svc (void)
{
	/*
		这个函数只允许进入一次，用于运行处理接收客户端请求的Reactor。
		正常情况下，ManageClientSession只会被初始化1个线程。
	*/

	REPORT_THREAD_INFO("ManageRoleSession::svc", ACE_OS::thr_self());

	ACE_GUARD_RETURN(ACE_Thread_Mutex, mutex_guard, m_mutex, 1);

#ifdef WIN32
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	m_reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(100000), true);
#endif

	int reactor_result = 0;
//	try
	{
		RoleAcceptor role_acceptor(m_reactor);
		ACE_INET_Addr port_to_listen(sMsgCfg->getRoleCfg().listen_port, sMsgCfg->getRoleCfg().host.c_str());
		if (role_acceptor.open(port_to_listen, m_reactor) == -1)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to call role_acceptor.open, host is <%s>, the port is <%d>, last error is <%d>\n"), sMsgCfg->getClientCfg().host.c_str(), sMsgCfg->getClientCfg().listen_port, ACE_OS::last_error());
			return 1;
		}

		//注册一个timer，用于检查是否需要退出
		m_timer_id = m_reactor->schedule_timer(this, NULL, ACE_Time_Value(3, 0), ACE_Time_Value(3, 0));
		if (m_timer_id < 0)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to register timer in ManageRoleSession::svc, the last error is <%d>"), ACE_OS::last_error());
			return 1;
		}

		//reactor_result = m_reactor->run_reactor_event_loop();
		while (!m_stop)
		{
			reactor_result = m_reactor->run_reactor_event_loop();
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
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in ManageRoleSession::svc, last error is <%d>\n"), ACE_OS::last_error());
	//	return 2;
	//}

	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageRoleSession::svc, reactor result is <%d>, last error is <%d>\n"), reactor_result, ACE_OS::last_error());
	m_wait = false;
	return 0;

}

int ManageRoleSession::handle_timeout(const ACE_Time_Value &current_time, const void *act)
{
	if (m_stop)
	{
		this->reactor()->cancel_timer(m_timer_id);
		this->reactor()->end_reactor_event_loop();
	}
	return 0;
}

int ManageRoleSession::init(int argc, ACE_TCHAR * argv[])
{
	int r = this->activate(THR_JOINABLE, 1);
	if (r < 0)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call ManageRoleSession::init, the last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init ManageRoleSession.\n"));
		return 0;
	}
}

int ManageRoleSession::fini()
{
	m_stop = true;
	m_reactor->end_reactor_event_loop();
	return 0;
}

int ManageRoleSession::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageRoleSession\n");
		ACE_OS::sleep(1);
	}
	return 0;
}
