
#ifndef MANAGE_LOGGER_HPP
#define MANAGE_LOGGER_HPP

#include "typedef.h"
#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "Singleton.h"
#include "logger_def.h"
#include "SingletonInit.h"
#include "Logger.h"

class RemoteLogger;

enum Logger_Destination
{
	LD_SCREEN = 0x1,	// print to screen
	LD_TARGET = 0x2,	// output to the target, for example, the file
	LD_REMOTE = 0x4		// the remote destination
};

struct LoggerCfg 
{
	LoggerCfg()
	: log_level(LL_DEBUG)
	, is_print(true)
	, enable_remote(false)
	, remote_log_level(LL_DEBUG)
	, file_max_record(1000000)
	{}

	int log_level;
	bool is_print;
	bool enable_remote;
	int remote_log_level;
	int file_max_record;
	string remote_addr;
	string log_dir;
	string log_identify;
};

class LOGGER_EXOPRT ManageLogger : public SingletonInit, public ACE_Task<ACE_NULL_SYNCH>
{
	friend class Singleton<ManageLogger>;
public:
	ManageLogger();

protected:
	~ManageLogger();

public:
	static ManageLogger * instance()
	{
		return Singleton<ManageLogger>::instance();
	}
public:
	// ACE_Task's svc
	virtual int svc();

public:
	static void setLogDestination(uint32 log_destination_mask);

	static uint32 getLogDestination();

public:
	char * getBuffer();

	void returnBuffer(char * log_buffer);

	void log(Logger * logger, char * log_buffer);

	int getFileMaxRecord();

	// register for calling of flush
	void registerLogger(Logger * logger);

public:
	// SingletonInit function
	virtual int init();

	virtual int stop();

	virtual int finit();

private:
	typedef define_unordered_set<Logger *> LoggerSet_t;

	bool loadLogCfg();

	void initBuffer();

	void doLogging(Logger * logger, char * log_buffer);

	bool initGlobalLogger();

	bool initLogDir();

	void setLogDir(const string & log_dir);
private:
	static uint32 m_log_dest_mask;

	char * doGetBuffer();

	bool doReturnBuffer(char * log_buffer);

	void flushLogger(LoggerSet_t & logger_flush_set);

private:
	typedef queue<char *> BufferCtn_t;

	typedef list<std::pair<Logger *, char *> > LoggerCtn_t;

	uint32 m_buffer_number;

	string m_log_dir;

	LoggerCfg m_logger_cfg;

	RemoteLogger * m_remote_logger;

	bool m_need_flush;

	ACE_Thread_Mutex m_buffer_mutex;

	BufferCtn_t m_buffer_ctn;

	ACE_Thread_Mutex m_logger_mutex;

	LoggerCtn_t m_logger_ctn;

	LoggerSet_t m_logger_flush_set;

	ACE_Thread_Mutex m_logger_flush_set_mutex;
private:
};

#endif
