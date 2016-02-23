
#include <ace/Select_Reactor.h>
#include "ManageGSStream.h"
#include "GateLogger.h"
#include "HandleGSInputStream.h"
#include "HandleGSOutputStream.h"
#include "ManageConfig.h"
#include "ManageRoleGsState.h"

//class StopHandler : public ACE_Event_Handler
//{
//public:
//	StopHandler(ManageGSStream * manage_gs_stream)
//	: m_timer_id (0)
//	, m_manage_gs_stream(manage_gs_stream)
//	{
//	}
//
//	virtual int  handle_timeout (const ACE_Time_Value &current_time, const void *act=0)
//	{
//		if (m_manage_gs_stream->isWait())
//		{
//			this->reactor()->cancel_timer(m_timer_id);
//			this->reactor()->end_reactor_event_loop();
//		}
//		return 0;
//	}
//
//	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
//	{
//		delete this;
//		return 0;
//	}
//
//	void setTimerID(long timer_id)
//	{
//		m_timer_id = timer_id;
//	}
//private:
//	long m_timer_id;
//
//	ManageGSStream * m_manage_gs_stream;
//};

ManageGSStream::ManageGSStream()
: m_stop(false)
, m_wait(true)
, m_free_thread(0)
{

}
ManageGSStream::~ManageGSStream()
{
	for (HandleGSStreamList::iterator it = m_gs_stream_list.begin(); it != m_gs_stream_list.end(); ++it)
	{
		delete *it;
	}
}

int ManageGSStream::svc(void)
{
	FUNCTIONTRACE(ManageGSStream::svc);
	REPORT_THREAD_INFO("ManageGSStream::svc", ACE_OS::thr_self());
//	collectFreeThread(-1);
	int result = 0;
	HandleGSStream * gs_stream = NULL;
//	try
	{
		while (!m_stop)
		{
			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_mutex, 1);
				if (m_gs_stream_list.size() > 0)
				{
					if (NULL == gs_stream)
					{
						if (m_gs_stream_list.size() > 0)
						{
							gs_stream = *m_gs_stream_list.begin();
							m_gs_stream_list.pop_front();
						}
					}
				}
			}

			if (NULL == gs_stream)
			{
				ACE_OS::sleep(ACE_Time_Value(0, 1000));
				continue;
			}

			GATE_LOG_DEBUG(ACE_TEXT("start to do gs socket\n"));
			while (!m_stop)
			{
				int r = gs_stream->doing();
				if (-1 == r)
				{
					GATE_LOG_ERROR(ACE_TEXT("Failed to execute gs doing\n"));
					ManageRoleGsState::instance()->cleanAllPlayerStatus();
					gs_stream = NULL;
					break;
				}
				else
				{
					ACE_OS::sleep(ACE_Time_Value(0, 1000));
				}
			}
			collectFreeThread(1);
		}
	}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in ManageGSStream::svc, last error is <%d>\n"), ACE_OS::last_error());
	//	result = 2;
	//}

	if (NULL != gs_stream)
	{
		delete gs_stream;
	}
	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageGSStream::svc, last error is <%d>\n"), ACE_OS::last_error());

	m_wait = false;

	return result;
}

int ManageGSStream::init(int argc, ACE_TCHAR * argv[])
{
	m_wait = false;
	return 0;
	//return this->activeThread(sMsgCfg->getGSCfg().handle_stream_thread_pool_number);
	///do nothing£¬ start a thread while a gs connection is coming
	//if (this->activate(THR_JOINABLE, sMsgCfg->getGSCfg().handle_stream_thread_pool_number) == -1)
	//{
	//	GATELogp(LM_ERROR, ACE_TEXT("Failed to call activate in ManageGSStream::init, last error is <%d>\n"), ACE_OS::last_error());
	//	return -1;
	//}
	//else
	//{
	//	GATELog(LM_INFO, ACE_TEXT("Success to init ManageGSStream\n"));
	//	return 0;
	//}
}

int ManageGSStream::fini()
{
	m_stop = true;
	return 0;
}

int ManageGSStream::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageGSStream\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

void ManageGSStream::handleStream(GSSession * gs_session, ACE_Reactor_Mask mask)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_mutex, );
	if (mask & ACE_Event_Handler::READ_MASK)
	{
		m_gs_stream_list.push_back(new HandleGSInputStream(gs_session));
	}
	else if (mask & ACE_Event_Handler::WRITE_MASK)
	{
		m_gs_stream_list.push_back(new HandleGSOutputStream(gs_session));
	}
	else
	{
		/// error
		GATE_LOG_ERROR(ACE_TEXT("Failed to get unknown mask in ManageGSStream::handleStream, the mask value is <%d>"), mask);
	}
}

bool ManageGSStream::isStop()
{
	return m_stop;
}

bool ManageGSStream::isWait()
{
	return m_wait;
}

int ManageGSStream::activeThread(int thread_no)
{
	int create_thread_no = 0;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_thread_number_mutex, -1);
		if (m_free_thread >= thread_no)
		{
			m_free_thread -= thread_no;
			return 0;
		}
		else
		{
			create_thread_no = thread_no - m_free_thread;
			m_free_thread = 0;
		}
	}

	if (this->activate(THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, create_thread_no, true) == -1)
	{
		//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_thread_number_mutex, -1);
		//m_free_thread -= create_thread_no;
		GATE_LOG_ERROR(ACE_TEXT("Failed to create thread in ManageGSStream::activeThread, thread no is <%d>, last error is <%d>\n"), create_thread_no, ACE_OS::last_error());
		return -1;
	}
	else
	{
		return 0;
	}
}

void ManageGSStream::collectFreeThread(int thread_no)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_thread_number_mutex, );
	m_free_thread += thread_no;
}