
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "ManageConfig.h"
#include "ManageTranscriptStream.h"

ManageTranscriptStream::ManageTranscriptStream()
: m_reactor(NULL)
, m_stop(false)
{

}

ManageTranscriptStream::~ManageTranscriptStream()
{
	if (NULL != m_reactor)
	{
		delete m_reactor;
	}
}


int ManageTranscriptStream::svc()
{
	FUNCTIONTRACE(ManageTranscriptStream::svc);
	REPORT_THREAD_INFO("ManageTranscriptStream::svc", ACE_OS::thr_self());
#ifdef WIN32
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	m_reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor((size_t)sMsgCfg->getClientCfg().max_socket), true);
#endif

//	try
	{
		while (!m_stop)
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
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in ManageTranscriptStream::svc, last error is <%d>\n"), ACE_OS::last_error());
	//}
	GATE_LOG_INFO(ACE_TEXT("Exit thread ManageTranscriptStream::svc, last error is <%d>\n"), ACE_OS::last_error());
	return 0;
}

int ManageTranscriptStream::init(int argc, ACE_TCHAR * argv[])
{
	//if (this->activate(THR_JOINABLE, sMsgCfg->getClientCfg().handle_input_stream_thread_pool_number) == -1)
	if (this->activate(THR_JOINABLE, 1) == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to init ManageTranscriptStream, last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		return 0;
	}
}

int ManageTranscriptStream::fini()
{
	m_stop = true;
	m_reactor->end_reactor_event_loop();
	return 0;
}

int ManageTranscriptStream::wait()
{
	return 0;
}

bool ManageTranscriptStream::handleTranscriptStream(GSSession * gs_session)
{
	if (m_reactor->register_handler(gs_session, ACE_Event_Handler::READ_MASK) == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to register transcript session, last error is <%d>\n"), ACE_OS::last_error());
		return false;
	}
	return true;
}