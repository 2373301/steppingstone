
#include <ace/OS.h>
#include <ace/Date_Time.h>
#include <boost/date_time.hpp>
#include "FileLogger.h"
#include "ManageLogger.h"

FileLogger::FileLogger()
: m_write_count(0)
, m_file_max_record(1000000)
, m_file_number_index(0)
{
	m_file_max_record = ManageLogger::instance()->getFileMaxRecord();
	ManageLogger::instance()->registerLogger(this);
}

FileLogger::~FileLogger()
{
	m_log_stream.flush();
	m_log_stream.close();
}

void FileLogger::doLogging(const char * log_buffer)
{
	m_log_stream.rdbuf()->sputn(log_buffer, strlen(log_buffer));

	++m_write_count;

	if (m_write_count >= m_file_max_record)
	{
		m_write_count = 0;
		++m_file_number_index;
		m_log_stream.flush();
		m_log_stream.close();
		openFileStream();
	}

	//m_write_count = ++m_write_count % 10;
	//if (0 == m_write_count)
	//{
	//	m_log_stream.flush();
	//}
}

void FileLogger::flush()
{
	m_log_stream.flush();
}

bool FileLogger::setFilePath(const string & file_path)
{
	m_file_path = makeLogFileName(file_path);
	return openFileStream();
}

string FileLogger::makeLogFileName(const string & file_path)
{
	//boost::posix_time::ptime curr_time = boost::posix_time::second_clock::local_time();
	//string str_time = boost::posix_time::to_simple_string(curr_time);
	string result;
	result.resize(250, 0);
	ACE_Date_Time tv(ACE_OS::gettimeofday());
	sprintf((char *)result.data(), "%s_%4d-%02d-%02dT%02d_%02d_%02d", file_path.c_str(), tv.year(), tv.month(), tv.day(), tv.hour(), tv.minute(), tv.second());

	return result;
}

bool FileLogger::openFileStream()
{
	string file_name;
	file_name.resize(250, 0);
	sprintf((char *)file_name.data(), "%s_%d.log", m_file_path.c_str(), m_file_number_index);
	m_log_stream.open(file_name.c_str(), ios_base::out | ios_base::app);
	return !m_log_stream.fail();
}