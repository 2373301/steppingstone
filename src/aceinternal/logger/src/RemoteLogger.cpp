
#include <ace/os_include/netinet/os_tcp.h>
#include <ace/SOCK_Connector.h>
#include "RemoteLogger.h"
#include "ManageLogger.h"
#include "ace/OS_NS_unistd.h"

RemoteLogger::RemoteLogger()
: m_logger_session(NULL)
, m_remote_logger_enable(false)
, m_remote_session_reference_no(2)
, m_logger_level(LL_DEBUG)
, m_is_stop(false)
{}

RemoteLogger::~RemoteLogger()
{
	if (NULL != m_logger_session)
	{
		m_logger_session->shutdown();
		m_logger_session->close();
		delete m_logger_session;
	}
}

int RemoteLogger::svc()
{
	while (!m_is_stop)
	{
		ACE_OS::sleep(3);
		if (!m_remote_logger_enable)
		{
			if (NULL != m_logger_session)
			{
				m_logger_session->reactor(NULL);
				delete m_logger_session;
				m_logger_session = NULL;
			}

			connectRemoteServer();
		}
	}
	return 0;
}

int RemoteLogger::init(const string & remote_addr)
{
	m_remote_addr = remote_addr;

	if (connectRemoteServer() == -1)
	{
		return -1;
	}

	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;

}

int RemoteLogger::stop()
{
	m_is_stop = true;
	m_manage_logger_input.stop();
	m_manage_logger_output.stop();
	return 0;
}

void RemoteLogger::doLogging(const char * log_buffer)
{
	if (m_remote_logger_enable)
	{
		m_manage_logger_output.handleLogBuffer(log_buffer);
	}
	else
	{
		ManageLogger::instance()->returnBuffer((char *)log_buffer);
	}
}

void RemoteLogger::setRemoteLoggerLevel(int log_level)
{
	m_logger_level = log_level;
}

void RemoteLogger::setLogIdentify(const string & log_identify)
{
	m_log_identify = log_identify;
}

void RemoteLogger::returnBuffer(char * log_buffer)
{
	ManageLogger::instance()->returnBuffer(log_buffer);
}

void RemoteLogger::outputError()
{
	char * log_disconnect = ManageLogger::instance()->getBuffer();
	strcpy(log_disconnect, "remote logger server disconnected.\n");
	m_manage_logger_output.handleLogBuffer(log_disconnect);
	reduceReferenceNo();
}

void RemoteLogger::inputError()
{
	m_manage_logger_output.syncReadError();
	reduceReferenceNo();
}

int RemoteLogger::connectRemoteServer()
{
	ACE_SOCK_Connector connector;
	ACE_INET_Addr addr(m_remote_addr.c_str());
	m_logger_session = new LoggerSession();
	ACE_Time_Value conn_time_out(1, 0);
	if (connector.connect(m_logger_session->peer(), addr, &conn_time_out) == -1)
	{
		m_remote_logger_enable = false;
		return -1;
	}
	else
	{
		m_logger_session->reactor(NULL);
		m_logger_session->open(NULL);

		m_manage_logger_input.setLoggerSession(m_logger_session, this);
		m_manage_logger_output.setLoggerSession(m_logger_session, this);

		if (m_manage_logger_input.init() == -1)
		{
			return -1;
		}

		if (m_manage_logger_output.init() == -1)
		{
			return -1;
		}

		m_remote_session_reference_no = 2;

		char * first_log = ManageLogger::instance()->getBuffer();
		strcpy(first_log, m_log_identify.c_str());
		memcpy(first_log + m_log_identify.length(), "\n", 1);
		m_manage_logger_output.handleLogBuffer(first_log);

		m_remote_logger_enable = true;

		return 0;
	}
}

void RemoteLogger::reduceReferenceNo()
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_remote_session_reference_no_mutex, );
	if (0 == m_remote_session_reference_no)
	{
		return ;
	}


	--m_remote_session_reference_no;
	if (0 == m_remote_session_reference_no)
	{
		m_remote_logger_enable = false;
	}
}