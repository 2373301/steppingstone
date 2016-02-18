//////////////////////////////////////////////////////////////////////////
///未捕获异常处理类，在发生未捕获异常的时候会写minidump文件
///dump文件存放的默认位置是与exe同一级目录，默认文件名为：minidump.dmp
///
///
//////////////////////////////////////////////////////////////////////////
#ifndef _HISO_MINIDUMPER_H
#define _HISO_MINIDUMPER_H

#include <string>
#include <ace/Thread_Mutex.h>
#include <ace/Global_Macros.h>
#include <ace/Guard_T.h>

#ifdef WIN32

#ifndef DEBUG_LOG
#define DEBUG_LOG printf
#endif

#ifndef INFO_FORMAT_LOG
#define INFO_FORMAT_LOG printf
#endif

#ifndef ERROR_FORMAT_LOG
#define ERROR_FORMAT_LOG printf
#endif

#ifndef ERROR_LOG
#define ERROR_LOG printf
#endif

#ifndef INFO_LOG
#define INFO_LOG printf
#endif

#include <Windows.h>
#include <dbghelp.h>

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
									CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
									CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
									CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

class MiniDumper
{
public:
	enum Flags{timestamp_file_name = 0x0001};

	MiniDumper();
public:
	static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS *pExceptionInfo);
public:
	static void filePath(const std::string & file_path);
	static void fileName(const std::string & file_name);
	static int  addFlags(Flags flags);
private:
	static void disableUnhandleFilter();
	static std::string newFilePath();
private:
	static int m_flags;
	static std::string m_dump_file_path;
	static std::string m_dump_file_name;
	static ACE_Thread_Mutex m_mutex;
};

#else
class MiniDumper
{
public:
	MiniDumper();
	~MiniDumper();
};

#endif

#endif
