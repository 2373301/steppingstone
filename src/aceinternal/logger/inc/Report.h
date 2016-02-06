
#ifndef REPORT_HPP
#define REPORT_HPP

#include "logger_def.h"
#include "typedef.h"

#define REPORT_THREAD_INFO()	\
	ThreadReport thread_report(__FUNCTION__, __FILE__, __LINE__);

class LOGGER_EXOPRT ThreadReport
{
public:
	ThreadReport(const string & fun_name, const string & file_name, int file_line);
	~ThreadReport();
protected:
private:
	int m_file_line;
	string m_fun_name;
	string m_file_name;
};

#endif