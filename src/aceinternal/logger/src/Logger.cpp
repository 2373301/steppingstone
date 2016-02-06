
#include <stdio.h>
#include <stdarg.h>
#include "typedef.h"
#include <ace/OS.h>
#include <ace/Date_Time.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Logger.h"
#include "ManageLogger.h"

#define STR_FATAL	("LL_FATAL   ")
#define STR_ERROR	("LL_ERROR   ")
#define STR_TRACE	("LL_TRACE ")
#define STR_WARNING	("LL_WARNING ")
#define STR_INFO	("LL_INFO    ")
#define STR_DEBUG	("LL_DEBUG   ")
#define STR_UNKNOWN	("LL_UNKNOWN ")

int32	Logger::m_filter_mask = LL_DEBUG;
int		Logger::m_buffer_length = 1024;

LOGGER_LEVEL extractLogLevel(const string & log_level_str)
{
	LOGGER_LEVEL log_level = LL_DEBUG;

	if (log_level_str == "LL_DEBUG")
	{
		log_level = LL_DEBUG;
	}
	else if (log_level_str == "LL_INFO")
	{
		log_level = LL_INFO;
	}
	else if (log_level_str == "LL_WARNING")
	{
		log_level = LL_WARNING;
	}
	else if (log_level_str == "LL_ERROR")
	{
		log_level = LL_ERROR;
	}
	else if (log_level_str == "LL_FATAL")
	{
		log_level = LL_FATAL;
	}

	return log_level;
}

Logger::Logger()
: m_synchronization(false)
//, m_module_name("UNDEF")
//, m_class_name("UNDEF")
{
}

Logger::~Logger()
{
}

void Logger::setSynchronization(bool sync)
{
	m_synchronization = sync;
}

bool Logger::getSynchronization()
{
	return m_synchronization;
}

void Logger::setName(const string & logger_name)
{
	m_logger_name = logger_name;
}

const string & Logger::getName()
{
	return m_logger_name;
}

void Logger::setFilter(uint32 filter_mask)
{
	m_filter_mask = filter_mask;
}

void Logger::setBufferLength(int buffer_length)
{
	m_buffer_length = buffer_length;
}

int Logger::getBufferLength()
{
	return m_buffer_length;
}

void Logger::log(LOGGER_LEVEL log_level, const char * log_format, ...)
{
	if (log_level < m_filter_mask)
	{
		return;
	}

	char * log_buffer = ManageLogger::instance()->getBuffer();
	if (NULL == log_buffer)
	{
		return ;
	}

	size_t buf_pos = 0;
	//boost::posix_time::ptime curr_time = boost::posix_time::second_clock::local_time();
	//string str_date = boost::posix_time::to_simple_string(curr_time);

	string str_date;
	str_date.resize(100, 0);
	ACE_Date_Time tv(ACE_OS::gettimeofday());
	sprintf((char *)str_date.data(), "%4d-%02d-%02dT%02d:%02d:%02d", tv.year(), tv.month(), tv.day(), tv.hour(), tv.minute(), tv.second());


	strcpy(log_buffer + buf_pos, str_date.c_str());
	buf_pos += strlen(str_date.c_str());

	log_buffer[buf_pos] = ' ';
	buf_pos += 1;

	switch(log_level)
	{
	case LL_FATAL:
		strcpy(log_buffer + buf_pos, STR_FATAL);
		buf_pos += strlen(STR_FATAL);
		break;
	case LL_ERROR:
		strcpy(log_buffer + buf_pos, STR_ERROR);
		buf_pos += strlen(STR_ERROR);
		break;
	case LL_TRACE:
		strcpy(log_buffer + buf_pos, STR_TRACE);
		buf_pos += strlen(STR_TRACE);
		break;
	case LL_WARNING:
		strcpy(log_buffer + buf_pos, STR_WARNING);
		buf_pos += strlen(STR_WARNING);
		break;
	case LL_INFO:
		strcpy(log_buffer + buf_pos, STR_INFO);
		buf_pos += strlen(STR_INFO);
		break;
	case LL_DEBUG:
		strcpy(log_buffer + buf_pos, STR_DEBUG);
		buf_pos += strlen(STR_DEBUG);
		break;
	default:
		strcpy(log_buffer + buf_pos, STR_UNKNOWN);
		buf_pos += strlen(STR_UNKNOWN);
		break;
	}

	//strcpy(log_buffer+ buf_pos, m_module_name.c_str());
	//buf_pos += m_module_name.length();
	//log_buffer[buf_pos] = ' ';
	//buf_pos += 1;

	//strcpy(log_buffer+ buf_pos, m_class_name.c_str());
	//buf_pos += m_class_name.length();
	//log_buffer[buf_pos] = ' ';
	//buf_pos += 1;

	bool success_formated = false;
	try
	{
		va_list args;
		va_start(args, log_format);

		success_formated = -1 != vsnprintf(log_buffer + buf_pos, m_buffer_length - buf_pos, log_format, args);
		va_end(args);
	}
	catch (...)
	{
		//printf("Occur error while calling _vsntprintf\n");
	}
	
	if (success_formated)
	{
		if (m_synchronization)
		{
			syncLog(log_buffer);
		} 
		else
		{
			ManageLogger::instance()->log(this, log_buffer);
		}
	}
	else
	{
		//printf("error to format the log string\n");
		ManageLogger::instance()->returnBuffer(log_buffer);
	}
}

void Logger::syncLog(char * log_buffer)
{
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_sync_logger_mutex, );
		doLogging(log_buffer);
	}
	ManageLogger::instance()->returnBuffer(log_buffer);
}

void Logger::flush()
{
	// do nothing
}