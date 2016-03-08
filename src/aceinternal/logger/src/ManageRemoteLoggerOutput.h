
#ifndef MANAGE_LOGGER_OUTPUT_HPP
#define MANAGE_LOGGER_OUTPUT_HPP

#include <ace/Task.h>
#include "LoggerSession.h"

class RemoteLogger;

class ManageRemoteLoggerOutput : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageRemoteLoggerOutput();

	~ManageRemoteLoggerOutput();

	int init();

	int stop();

	int svc();

	void handleLogBuffer(const char * log_buffer);

	void setLoggerSession(LoggerSession * logger_session, RemoteLogger * remote_logger);

	void syncReadError();

protected:
	typedef vector<char *> LogBuffer_t;

	void clearBuffer(LogBuffer_t & log_buffer_vec);

private:
	LoggerSession * m_logger_session;

	RemoteLogger * m_remote_logger;

	bool m_inited;

	LogBuffer_t m_log_buffer_vec;

	ACE_Thread_Mutex m_log_buffer_mutex;

	bool m_read_error;

	bool m_is_stop;
};
#endif