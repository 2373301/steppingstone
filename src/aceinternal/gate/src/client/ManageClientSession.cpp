
#include <ace/OS.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "GateLogger.h"
#include "ManageClientSession.h"
#include "ClientSession.h"
#include "ManageConfig.h"

ManageClientSession::ManageClientSession()
: m_stop(false)
, m_wait(true)
, m_timer_id(0)
{}

ManageClientSession::~ManageClientSession()
{
	if (NULL != m_reactor)
	{
		delete m_reactor;
		m_reactor = NULL;
	}
}

int ManageClientSession::svc(void)
{
	/*
		�������ֻ�������һ�Σ��������д�����տͻ��������Reactor��
		��������£�ManageClientSessionֻ�ᱻ��ʼ��1���̡߳�
	*/

	REPORT_THREAD_INFO("ManageClientSession::svc", ACE_OS::thr_self());

	ACE_GUARD_RETURN(ACE_Thread_Mutex, mutex_guard, m_mutex, 1);

#ifdef WIN32
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	m_reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor((size_t)sMsgCfg->getClientCfg().max_socket), true);
#endif

	int reactor_result = 0;
//	try
	{
		ClientAcceptor client_acceptor(m_reactor);
		ACE_INET_Addr port_to_listen(sMsgCfg->getClientCfg().listen_port, sMsgCfg->getClientCfg().host.c_str());
		if (client_acceptor.open(port_to_listen, m_reactor) == -1)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to call client_acceptor.open, host is <%s>, the port is <%d>, last error is <%d>\n"), sMsgCfg->getClientCfg().host.c_str(), sMsgCfg->getClientCfg().listen_port, ACE_OS::last_error());
			return 1;
		}

		//ע��һ��timer�����ڼ���Ƿ���Ҫ�˳�
		m_timer_id = m_reactor->schedule_timer(this, NULL, ACE_Time_Value(3, 0), ACE_Time_Value(3, 0));
		if (m_timer_id < 0)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to register timer in ManageClientSession::svc, the last error is <%d>"), ACE_OS::last_error());
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
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in ManageClientSession::svc, last error is <%d>\n"), ACE_OS::last_error());
	//	return 2;
	//}

	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageClientSession::svc, reactor result is <%d>, last error is <%d>\n"), reactor_result, ACE_OS::last_error());
	m_wait = false;
	return 0;
}

int ManageClientSession::handle_timeout(const ACE_Time_Value &current_time, const void *act)
{
	if (m_stop)
	{
		if (this->reactor() != NULL)
		{
			this->reactor()->cancel_timer(m_timer_id);
			this->reactor()->end_reactor_event_loop();
		}
	}
	return 0;
}

int ManageClientSession::init(int argc, ACE_TCHAR * argv[])
{
	int r = this->activate(THR_JOINABLE, 1);
	if (r < 0)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call ManageClientSession::init, the last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init ManageClientSession.\n"));
		return 0;
	}
}

int ManageClientSession::fini()
{
	m_stop = true;
	m_reactor->end_reactor_event_loop();
	return 0;
}

int ManageClientSession::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageClientSession\n");
		ACE_OS::sleep(1);
	}

	return 0;
}