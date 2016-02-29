
#include <ace/OS.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "InputSessionPool.h"
#include "Logger.h"
#include "Report.h"
#include "SessionPoolImp.h"
#include "ManageNetEventNotify.h"

namespace netstream
{

InputSessionPool::InputSessionPool()
: m_session_thread_info_index(0)
, m_stop(false)
, m_wait(true)
, m_actived(false)
, m_session_pool(NULL)
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

int InputSessionPool::init(int thread_no, SessionPool * session_pool)
{
	m_session_pool = session_pool;

	if (this->activate(THR_NEW_LWP, thread_no) == -1)
	{
		return -1;
	}

	while (!m_actived)
	{
		ACE_OS::sleep(1);
	}

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
		ACE_OS::sleep(1);
	}
}

int InputSessionPool::svc()
{
	REPORT_THREAD_INFO();

	InputSessionThreadInfo * session_thread_info = new InputSessionThreadInfo();

#ifdef WIN32
	session_thread_info->reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	session_thread_info->reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(100000), true);
#endif

	ManageNetEventNotify::instance()->collectSessionPoolReactor((SessionPoolImp *)m_session_pool, session_thread_info->reactor);
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

	m_wait = false;
	return 0;
}

void InputSessionPool::handleSession(CellSession * cell_session)
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

void InputSessionPool::removeSession(CellSession * cell_session)
{
	InputSessionThreadInfo * thr_info = NULL;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_thread_info_mutex, );
		CellSessionMap_t::iterator it = m_cell_session_map.find(cell_session);
		if (it != m_cell_session_map.end())
		{
			thr_info = it->second;
			m_cell_session_map.erase(it);
		}
	}

	if (NULL != thr_info)
	{
		thr_info->reactor->remove_handler(cell_session, ACE_Event_Handler::ALL_EVENTS_MASK);
	}
}

void InputSessionPool::registerSessionThreadInfo(InputSessionThreadInfo * stinfo)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_thread_info_mutex, );
	m_session_thread_info.push_back(stinfo);
}

}
