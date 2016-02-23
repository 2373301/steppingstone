
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "GateLogger.h"
#include "ManageConfig.h"
#include "ManageClientStreamEx.h"

ManageClientStreamEx::ManageClientStreamEx()
: m_stop(false)
, m_wait(true)
, m_reactor_index(0)
{

}

ManageClientStreamEx::~ManageClientStreamEx()
{
	for (ReactorList::iterator it = m_reactor_list.begin(); it != m_reactor_list.end(); ++it)
	{
		delete *it;
	}
}

int ManageClientStreamEx::svc(void)
{
	FUNCTIONTRACE(ManageClientStreamEx::svc);
	REPORT_THREAD_INFO("ManageClientStreamEx::svc", ACE_OS::thr_self());
	ACE_Reactor * reactor = NULL;
#ifdef WIN32
	reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor((size_t)sMsgCfg->getClientCfg().max_socket), true);
#endif

//	try
	{
		registerReactor(reactor);

		while (!m_stop)
		{
			reactor->run_reactor_event_loop();
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
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in ManageClientStreamEx::svc, last error is <%d>\n"), ACE_OS::last_error());
	//}
	unregistorReactor(reactor);
	delete reactor;
	reactor = NULL;
	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageClientStreamEx::svc, last error is <%d>\n"), ACE_OS::last_error());
	m_wait = false;
	return 0;
}

int ManageClientStreamEx::init(int argc, ACE_TCHAR * argv[])
{
	if (this->activate(THR_JOINABLE, sMsgCfg->getClientCfg().handle_input_stream_thread_pool_number) == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to init ManageClientStreamEx, last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		return 0;
	}
}

int ManageClientStreamEx::fini()
{
	m_stop = true;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_reactor_mutex, -1);
	{
		for (ReactorList::iterator it = m_reactor_list.begin(); it != m_reactor_list.end(); ++it)
		{
			(*it)->end_reactor_event_loop();
		}
	}
	return 0;
}

int ManageClientStreamEx::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageClientStreamEx\n");
		ACE_OS::sleep(1);
	}

	ACE_OS::sleep(1);

	return 0;
}

void ManageClientStreamEx::registerReactor(ACE_Reactor * reactor)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_reactor_mutex, );
	m_reactor_list.push_back(reactor);
}

void ManageClientStreamEx::unregistorReactor(ACE_Reactor * reactor)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_reactor_mutex, );
	ReactorList::iterator it = std::find(m_reactor_list.begin(), m_reactor_list.end(), reactor);
	if (it != m_reactor_list.end())
	{
		m_reactor_list.erase(it);
	}
}

bool ManageClientStreamEx::handleClientStream(ClientSession * client_stream)
{
	bool result = false;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_reactor_mutex, false);
	if (m_reactor_list.size() > 0)
	{
		ACE_Reactor * reactor = m_reactor_list[m_reactor_index];
		if ((reactor->register_handler(client_stream, ACE_Event_Handler::READ_MASK) == -1))
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to register read event in ManageClientStreamEx::handleClientStream, last error is <%d>\n"), ACE_OS::last_error());
			result = false;
		}
		else
		{
			result = true;
		}
		m_reactor_index = ++m_reactor_index % m_reactor_list.size();
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("the reactor count is 0, ManageClientStreamEx::handleClientStream\n"));
	}
	return result;
}