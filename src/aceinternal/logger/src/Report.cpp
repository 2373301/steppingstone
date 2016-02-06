
#include "Report.h"
#include "Logger.h"

#ifndef WIN32
#include <sys/types.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)
#endif

ThreadReport::ThreadReport(const string & fun_name, const string & file_name, int file_line)
: m_file_line(file_line)
, m_fun_name(fun_name)
, m_file_name(file_name)
{
#ifndef WIN32
	DEF_LOG_INFO("stat. enter thread, thread id is <%d> <%x>, fun is <%s>, file is <%s>, line is <%d>\n", gettid(), ACE_OS::thr_self(), m_fun_name.c_str(), m_file_name.c_str(), m_file_line);
#else
	DEF_LOG_INFO("stat. enter thread, thread id is <%d> <%x>, fun is <%s>, file is <%s>, line is <%d>\n", ACE_OS::thr_self(), ACE_OS::thr_self(), m_fun_name.c_str(), m_file_name.c_str(), m_file_line);
#endif
}

ThreadReport::~ThreadReport()
{
#ifndef WIN32
	DEF_LOG_INFO("stat. leave thread, thread id is <%d>, fun is <%s>, file is <%s>, line is <%d>\n", gettid(), m_fun_name.c_str(), m_file_name.c_str(), m_file_line);
#else
	DEF_LOG_INFO("stat. leave thread, thread id is <%d>, fun is <%s>, file is <%s>, line is <%d>\n", ACE_OS::thr_self(), m_fun_name.c_str(), m_file_name.c_str(), m_file_line);
#endif
}
