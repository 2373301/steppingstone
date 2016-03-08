
#include "ManageRemoteLoggerInput.h"
#include "RemoteLogger.h"
#include "ace/OS_NS_unistd.h"

ManageRemoteLoggerInput::ManageRemoteLoggerInput()
: m_logger_session(NULL)
, m_remote_logger(NULL)
, m_inited(false)
, m_is_stop(false)
{
	m_input_block.init(1024 * 100);
}

ManageRemoteLoggerInput::~ManageRemoteLoggerInput()
{

}

int ManageRemoteLoggerInput::init()
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

int ManageRemoteLoggerInput::stop()
{
	m_is_stop = true;
	return 0;
}

int ManageRemoteLoggerInput::svc()
{
	int read_r = 0;
	ACE_Time_Value sleep_time(0, 10 * 1000);
	while (!m_is_stop)
	{
		if (NULL == m_logger_session)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		read_r = m_logger_session->recv_data(m_input_block);
		if (-1 == read_r)
		{
			m_logger_session = NULL;
			m_remote_logger->inputError();
		}
		else if (0 == read_r)
		{
			ACE_OS::sleep(sleep_time);
		}
	}
	return 0;
}

void ManageRemoteLoggerInput::setLoggerSession(LoggerSession * logger_session, RemoteLogger * remote_logger)
{
	m_logger_session = logger_session;
	m_remote_logger = remote_logger;
}
