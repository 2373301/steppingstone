
#include <ace/OS.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "InputSessionPool.h"
#include "Logger.h"
#include "SessionPoolAssistent.h"

namespace netcore
{

InputSessionPool::InputSessionPool()
: m_session_thread_info_index(0)
, m_stop(false)
, m_wait(true)
, m_actived(false)
, m_session_pool(NULL)
, m_handle_session_event(NULL)
{

}

InputSessionPool::~InputSessionPool()
{
	for (InputSessionThreadInfoVec_t::iterator it = m_session_thread_info.begin(); it != m_session_thread_info.end(); ++it)
	{
		InputSessionThreadInfo * ist = *it;
		delete ist;
	}
}

int InputSessionPool::init(int thread_no, SessionPoolx * session_pool, HandleSessionOpenClosed * handle_session_event)
{
	if (this->activate(THR_NEW_LWP, thread_no) == -1)
	{
		return -1;
	}

	// 等待svc 运行起来
	while (!m_actived)
	{
		ACE_OS::sleep(1);
	}

	m_session_pool = session_pool;
	m_handle_session_event = handle_session_event;

	return 0;
}

void InputSessionPool::stop()
{
	m_stop = true;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_thread_info_mutex, );
		for (InputSessionThreadInfoVec_t::iterator it = m_session_thread_info.begin(); it != m_session_thread_info.end(); ++it)
		{
			InputSessionThreadInfo * ist = *it;
			ist->reactor->end_reactor_event_loop();
		}
	}
}

void InputSessionPool::finit()
{
	while(m_wait)
	{
		//std::cout << "wait InputSessionPool" << std::endl;
		ACE_OS::sleep(1);
	}

	ACE_OS::sleep(2);

	//std::cout << "exit InputSessionPool" << std::endl;

}

int InputSessionPool::svc()
{
	REPORT_THREAD_INFO();

	try
	{
		InputSessionThreadInfo * session_thread_info = new InputSessionThreadInfo();

#ifdef WIN32
		session_thread_info->reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
		session_thread_info->reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(100000), true);
#endif

		collectSessionPool(this, session_thread_info->reactor);

		registerSessionThreadInfo(session_thread_info);

		m_actived = true;

		int reactor_result = 0;

		while (true)
		{
			reactor_result = session_thread_info->reactor->run_reactor_event_loop();
#ifdef WIN32
			break;
#else
			int last_error = ACE_OS::last_error();
			if ((last_error == EWOULDBLOCK) || (last_error == EINTR) || (last_error == EAGAIN))
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
	catch (...)
	{
		DEF_LOG_ERROR("unknown exception in InputSessionPool::svc, last error is <%d>\n", ACE_OS::last_error());
	}

	m_wait = false;
	return 0;
}

void InputSessionPool::handleSession(CellSessionx * cell_session)
{
	InputSessionThreadInfo * thr_info = NULL;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_thread_info_mutex, );
		if (m_session_thread_info.size() == 0)
		{
			// error , should never happen
			return ;
		}

		int index = m_session_thread_info_index++ % m_session_thread_info.size();
		thr_info = m_session_thread_info[index];
		m_cell_session_map[cell_session] = thr_info;
	}

	thr_info->reactor->register_handler(cell_session, ACE_Event_Handler::READ_MASK);
}

void InputSessionPool::registerSessionThreadInfo(InputSessionThreadInfo * stinfo)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_thread_info_mutex, );
	m_session_thread_info.push_back(stinfo);
}

void InputSessionPool::sessionOpen(Sessionx * session)
{
	// do nothing
}

void InputSessionPool::sessionClosed(Sessionx * session)
{
	if (NULL != m_handle_session_event)
	{
		m_handle_session_event->sessionClosed(session);
	}
}


}
