
#include <ace/OS.h>
#include <boost/filesystem.hpp>
#include "ManageLogger.h"
#include "LoggerFactory.h"
#include "SimpleXML.h"
#include "RemoteLogger.h"

Logger * g_logger_instance = NULL;

uint32 ManageLogger::m_log_dest_mask = LD_SCREEN | LD_TARGET;

ManageLogger::ManageLogger()
: m_buffer_number(100)
, m_remote_logger(NULL)
, m_need_flush(false)
{
	m_log_dir = "logs";
	m_remote_logger = new RemoteLogger();
}

ManageLogger::~ManageLogger()
{
	for (LoggerCtn_t::iterator it = m_logger_ctn.begin(); it != m_logger_ctn.end(); ++it)
	{
		free(it->second);
	}

	while (m_buffer_ctn.size() > 0)
	{
		free(m_buffer_ctn.front());
		m_buffer_ctn.pop();
	}

	if (NULL != g_logger_instance)
	{
		delete g_logger_instance;
	}

	if (NULL != m_remote_logger)
	{
		delete m_remote_logger;
		m_remote_logger = NULL;
	}
}

int ManageLogger::svc()
{
	int wait_index = 0;
	ACE_Time_Value sleep_time(0, 1 * 1000);
	LoggerCtn_t logger_ctn;
	LoggerSet_t logger_flush_set;

	while (!m_stop)
	{
		logger_ctn.clear();

		// get the copy of logger content
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_logger_mutex, -1);
			if (!m_logger_ctn.empty())
			{
				std::copy(m_logger_ctn.begin(), m_logger_ctn.end(), std::back_inserter(logger_ctn));
				m_logger_ctn.clear();
			}
		}

		// if logger is empty, continue
		if (logger_ctn.empty())
		{
			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_logger_flush_set_mutex, -1);
				if (m_logger_flush_set.size() > 0)
				{
					for (LoggerSet_t::iterator it = m_logger_flush_set.begin(); it != m_logger_flush_set.end(); ++it)
					{
						logger_flush_set.insert(*it);
					}
				}
			}

			if (m_need_flush)
			{
				m_need_flush = false;
				flushLogger(logger_flush_set);
			}
			else
			{
				ACE_OS::sleep(sleep_time);
			}
			continue;
		}

		m_need_flush = true;

		for (LoggerCtn_t::iterator it = logger_ctn.begin(); it != logger_ctn.end(); ++it)
		{
			doLogging(it->first, it->second);
		}
	}

	m_wait = false;

	return 0;
}

void ManageLogger::setLogDestination(uint32 log_destination_mask)
{
	m_log_dest_mask = log_destination_mask;
}

uint32 ManageLogger::getLogDestination()
{
	return m_log_dest_mask;
}

char * ManageLogger::getBuffer()
{
	char * buf = doGetBuffer();
	if (NULL == buf)
	{
		buf = (char *)malloc(Logger::getBufferLength());
		memset(buf, 0, Logger::getBufferLength());
	}

	return buf;
}

void ManageLogger::returnBuffer(char * log_buffer)
{
	if (!doReturnBuffer(log_buffer))
	{
		free(log_buffer);
	}
}

void ManageLogger::log(Logger * logger, char * log_buffer)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_logger_mutex, );
	m_logger_ctn.push_back(std::make_pair(logger, log_buffer));
}

int ManageLogger::getFileMaxRecord()
{
	return m_logger_cfg.file_max_record;
}

void ManageLogger::registerLogger(Logger * logger)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_logger_flush_set_mutex, );
	m_logger_flush_set.insert(logger);
}

void ManageLogger::setLogDir(const string & log_dir)
{
	if (!log_dir.empty())
		m_log_dir = log_dir;
}

char * ManageLogger::doGetBuffer()
{
	char * buf = NULL;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_buffer_mutex, NULL);
		if (!m_buffer_ctn.empty())
		{
			buf = m_buffer_ctn.front();
			m_buffer_ctn.pop();
			return buf;
		}
	}

	buf = (char *)malloc(Logger::getBufferLength());
	memset(buf, 0, Logger::getBufferLength());

	return buf;
}

bool ManageLogger::doReturnBuffer(char * log_buffer)
{
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_buffer_mutex, false);
		if (m_buffer_ctn.size() < m_buffer_number)
		{
			memset(log_buffer, 0, Logger::getBufferLength());
			m_buffer_ctn.push(log_buffer);

			return true;
		}
	}

	// queue is full, free it
	free(log_buffer);
	return true;
}

void ManageLogger::flushLogger(LoggerSet_t & logger_flush_set)
{
	for (LoggerSet_t::iterator it = logger_flush_set.begin(); it != logger_flush_set.end(); ++it)
	{
		Logger * logger = *it;
		logger->flush();
	}
}

int ManageLogger::init()
{
	static bool first_enter = true;
	if (!first_enter)
	{
		return 0;
	}

	first_enter = false;

	if (!loadLogCfg())
	{
		return -1;
	}

	if (m_logger_cfg.enable_remote)
	{
		m_remote_logger->setLogIdentify(m_logger_cfg.log_identify);

		if (m_remote_logger->init(m_logger_cfg.remote_addr) == -1)
		{
			return -1;
		}
	}

	setLogDir(m_logger_cfg.log_dir);

	Logger::setFilter(m_logger_cfg.log_level);

	if (m_logger_cfg.is_print)
	{
		m_log_dest_mask = m_log_dest_mask | LD_SCREEN;
	}
	else
	{
		m_log_dest_mask = m_log_dest_mask & (~LD_SCREEN);
	}

	if (m_logger_cfg.enable_remote)
	{
		m_log_dest_mask = m_log_dest_mask | LD_REMOTE;
	}
	else
	{
		m_log_dest_mask = m_log_dest_mask & (~LD_REMOTE);
	}

	if (!initLogDir())
	{
		std::cout << "failed to init log dir" << std::endl;
		return -1;
	}

	if (!initGlobalLogger())
	{
		std::cout << "failed to init global logger" << std::endl;
		return -1;
	}

	initBuffer();
	if (this->activate() == -1)
	{
		std::cout << "failed to active ManageLogger thread, last error is " << ACE_OS::last_error() << std::endl;
		return -1;
	}
	else
	{
		return 0;
	}
}

int ManageLogger::stop()
{
	m_stop = true;
	m_remote_logger->stop();
	return 0;
}

int ManageLogger::finit()
{
	SingletonInit::finit();
	return 0;
}

bool ManageLogger::loadLogCfg()
{
	bool result = false;
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		return false;
	}

	if (!auto_doc->load_file("logger_cfg.xml"))
	{
		return false;
	}

	Element * log_cfg_ele = auto_doc->get_root()->get_element("logcfg");
	if (NULL == log_cfg_ele)
	{
		return false;
	}

	Attributes attrs = log_cfg_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "level")
		{
			m_logger_cfg.log_level = extractLogLevel(attr->get_value());
		}
		else if (attr->get_name() == "dir")
		{
			m_logger_cfg.log_dir = attr->get_value();
		}
		else if (attr->get_name() == "print")
		{
			m_logger_cfg.is_print = attr->as_bool();
		}
		else if (attr->get_name() == "log_identify")
		{
			m_logger_cfg.log_identify = attr->get_value();
		}
		else if (attr->get_name() == "enable_remote")
		{
			m_logger_cfg.enable_remote = attr->as_bool();
		}
		else if (attr->get_name() == "remote_addr")
		{
			m_logger_cfg.remote_addr = attr->get_value();
		}
		else if (attr->get_name() == "remote_level")
		{
			m_logger_cfg.remote_log_level = extractLogLevel(attr->get_value());
		}
		else if (attr->get_name() == "file_max_record")
		{
			m_logger_cfg.file_max_record = attr->as_int(100000);
		}
	}

	return true;
}

void ManageLogger::initBuffer()
{
	for (uint32 i = 0; i < m_buffer_number; ++i)
	{
		char * buf = (char *)malloc(Logger::getBufferLength());
		memset(buf, 0, Logger::getBufferLength());
		m_buffer_ctn.push(buf);
	}
}

void ManageLogger::doLogging(Logger * logger, char * log_buffer)
{
	if (m_log_dest_mask & LD_SCREEN)
	{
		printf(log_buffer);
	}

	if (m_log_dest_mask & LD_TARGET)
	{
		if (!logger->getSynchronization())
		{
			logger->doLogging(log_buffer);
		}
	}

	if (m_log_dest_mask & LD_REMOTE)
	{
		// todo
		m_remote_logger->doLogging(log_buffer);
	}
	else
	{
		returnBuffer(log_buffer);
	}
}

bool ManageLogger::initGlobalLogger()
{
	g_logger_instance = LoggerFactory::createFileLogger(m_log_dir + "/default");
	return NULL != g_logger_instance;
}

bool ManageLogger::initLogDir()
{
	boost::filesystem::path path(m_log_dir);
	if (!boost::filesystem::exists(path))
	{
		return boost::filesystem::create_directory(path);
	}
	else
	{
		return true;
	}
}
