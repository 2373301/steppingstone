
#ifndef REMOTE_LOGGER_HPP
#define REMOTE_LOGGER_HPP

#include "typedef.h"
#include <fstream>
#include <ace/Task.h>
#include "Logger.h"
#include "LoggerSession.h"
#include "ManageLoggerInput.h"
#include "ManageLoggerOutput.h"

class RemoteLogger : public Logger, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	RemoteLogger();

	~RemoteLogger();

public:
	int svc();

public:
	int init(const string & remote_addr);

	int stop();

	// Logger function
	virtual void doLogging(const char * log_buffer);

	void setRemoteLoggerLevel(int log_level);

	void setLogIdentify(const string & log_identify);

	void returnBuffer(char * log_buffer);

	void outputError();

	void inputError();

protected:
	int connectRemoteServer();

	void reduceReferenceNo();
private:
	LoggerSession * m_logger_session;

	bool m_remote_logger_enable;

	int m_remote_session_reference_no;

	int m_logger_level;

	ACE_Thread_Mutex m_remote_session_reference_no_mutex;

	string m_remote_addr;

	string m_log_identify;

	ManageLoggerInput m_manage_logger_input;

	ManageLoggerOutput m_manage_logger_output;

	bool m_is_stop;
};

#endif