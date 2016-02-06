
#ifndef FILE_LOGGER_HPP
#define FILE_LOGGER_HPP

#include "typedef.h"
#include <fstream>
#include "Logger.h"

class FileLogger : public Logger
{
public:
	FileLogger();

	~FileLogger();

public:
	// Logger function
	virtual void doLogging(const char * log_buffer);

	virtual void flush();
public:
	bool setFilePath(const string & file_path);

private:
	string makeLogFileName(const string & file_path);

	bool openFileStream();
private:
	int    m_write_count;

	int	   m_file_max_record;

	int	   m_file_number_index;

	string m_file_path;

	fstream m_log_stream;
};

#endif