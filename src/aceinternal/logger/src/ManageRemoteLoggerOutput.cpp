
#include "ManageLogger.h"
#include "ManageRemoteLoggerOutput.h"
#include "RemoteLogger.h"
#include "ace/OS_NS_unistd.h"

ManageRemoteLoggerOutput::ManageRemoteLoggerOutput()
: m_logger_session(NULL)
, m_remote_logger(NULL)
, m_inited(false)
, m_read_error(false)
, m_is_stop(false)
{
	
}

ManageRemoteLoggerOutput::~ManageRemoteLoggerOutput()
{
	for (LogBuffer_t::iterator it = m_log_buffer_vec.begin(); it != m_log_buffer_vec.end(); ++it)
	{
		free(*it);
	}
}

int ManageRemoteLoggerOutput::init()
{
	if ((NULL == m_logger_session) || (NULL == m_remote_logger))
	{
		return -1;
	}

	if (m_inited)
	{
		return 0;
	}
	
	m_inited = true;

	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

int ManageRemoteLoggerOutput::stop()
{
	m_is_stop = true;
	return 0;
}

int ManageRemoteLoggerOutput::svc()
{
	ACE_Time_Value sleep_time(0, 10 * 1000);
	LogBuffer_t log_buffer_vec;
	int handle_result = 0;
	while (!m_is_stop)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_log_buffer_mutex, -1);
			std::copy(m_log_buffer_vec.begin(), m_log_buffer_vec.end(), std::back_inserter(log_buffer_vec));
			m_log_buffer_vec.clear();
		}

		if (m_read_error)
		{
			m_read_error = false;
			m_logger_session = NULL;
			m_remote_logger->outputError();
		}

		if (log_buffer_vec.size() == 0)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		if (NULL == m_logger_session)
		{
			clearBuffer(log_buffer_vec);
			continue;
		}

		handle_result = m_logger_session->handleLogBuffer(log_buffer_vec);

		clearBuffer(log_buffer_vec);

		if (-1 == handle_result)
		{
			m_logger_session = NULL;
			m_remote_logger->outputError();
		}
	}
	return 0;
}

void ManageRemoteLoggerOutput::handleLogBuffer(const char * log_buffer)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_log_buffer_mutex, );
	m_log_buffer_vec.push_back((char *)log_buffer);
}

void ManageRemoteLoggerOutput::setLoggerSession(LoggerSession * logger_session, RemoteLogger * remote_logger)
{
	m_logger_session = logger_session;

	m_remote_logger = remote_logger;

	m_read_error = false;
}

void ManageRemoteLoggerOutput::clearBuffer(LogBuffer_t & log_buffer_vec)
{
	for (LogBuffer_t::iterator it = log_buffer_vec.begin(); it != log_buffer_vec.end(); ++it)
	{
		m_remote_logger->returnBuffer(*it);
	}
	log_buffer_vec.clear();
}

void ManageRemoteLoggerOutput::syncReadError()
{
	m_read_error = true;
}