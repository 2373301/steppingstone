
#ifdef WIN32

#include "MiniDumper.h"

#pragma comment(lib, "dbghelp.lib")

static MiniDumper g_mini_dumper;

int MiniDumper::m_flags = timestamp_file_name;
std::string MiniDumper::m_dump_file_path;
std::string MiniDumper::m_dump_file_name;
ACE_Thread_Mutex MiniDumper::m_mutex;

MiniDumper::MiniDumper()
{
	DEBUG_LOG("Enter MiniDumper::MiniDumper()\n");

	::SetUnhandledExceptionFilter( TopLevelFilter );
	
#ifndef DEBUG
	disableUnhandleFilter();
#endif

	DEBUG_LOG("Leave MiniDumper::MiniDumper()\n");
}

LONG MiniDumper::TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	DEBUG_LOG("Enter MiniDumper::TopLevelFilter()\n");

	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_mutex, 1);
	std::string filepath = newFilePath();
	LONG retval = EXCEPTION_CONTINUE_SEARCH;
	HWND hParent = NULL;						// find a better value for your app

	// firstly see if dbghelp.dll is around and has the function we need
	// look next to the EXE first, as the one in System32 might be old 
	// (e.g. Windows 2000)
	
	TCHAR szDbgHelpPath[_MAX_PATH];

	HMODULE hDll = ::LoadLibrary("DBGHELP.DLL");

	if ((!hDll) && GetModuleFileName( NULL, szDbgHelpPath, _MAX_PATH ))
	{
		char *pSlash = strrchr( szDbgHelpPath, '\\' );
		if (pSlash)
		{
			strcpy( pSlash+1, "DBGHELP.DLL" );
			hDll = ::LoadLibrary( szDbgHelpPath );
		}
	}
	
	if (hDll)
	{
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
		if (pDump)
		{
			std::string dump_file = filepath; /*m_dump_file_path + m_dump_file_name;*/
			HANDLE hFile = ::CreateFile( dump_file.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL, NULL );

			if (hFile!=INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

				ExInfo.ThreadId = ::GetCurrentThreadId();
				ExInfo.ExceptionPointers = pExceptionInfo;
				ExInfo.ClientPointers = NULL;

				// write the dump
				BOOL bOK = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
				if (bOK)
				{
					INFO_FORMAT_LOG("Saved dump file to '%s'\n", dump_file.c_str());
					retval = EXCEPTION_EXECUTE_HANDLER;
				}
				else
				{
					ERROR_FORMAT_LOG("Failed to save dump file to '%s' (error %d)\n", dump_file.c_str(), GetLastError() );
				}
				::CloseHandle(hFile);
			}
			else
			{
				ERROR_FORMAT_LOG("Failed to create dump file '%s' (error %d)\n", dump_file.c_str(), GetLastError() );
			}
		}
		else
		{
			ERROR_LOG("DBGHELP.DLL too old\n");
		}
	}
	else
	{
		ERROR_LOG("DBGHELP.DLL not found\n");
	}

	DEBUG_LOG("Leave MiniDumper::TopLevelFilter()\n");
	INFO_LOG("This process would be terminated\n");
	::TerminateProcess(GetCurrentProcess(), 101);
	return retval;
}

void MiniDumper::disableUnhandleFilter()
{
// #ifndef _M_IX86
// 	#error "The following code only works for x86!"
// #endif
// 	void *addr = (void*)GetProcAddress(LoadLibrary("kernel32.dll"), "SetUnhandledExceptionFilter");
// 	if (addr)
// 	{
// 		unsigned char code[16];
// 		int size = 0;
// 		//xor eax, eax;
// 		code[size++] = 0x33;
// 		code[size++] = 0xc0;
// 		//ret 4
// 		code[size++] = 0xc2;
// 		code[size++] = 0x04;
// 		code[size++] = 0x00;
// 
// 		DWORD dwOldFlag, dwTempFlags;
// 		VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
// 		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
// 		VirtualProtect(addr, size, dwOldFlag, &dwTempFlags);
// 	}
}

void MiniDumper::filePath(const std::string & file_path)
{
	WIN32_FIND_DATA find_file_data;
	std::string find_file_path = file_path + "\\*.*";
	HANDLE hFind = ::FindFirstFile(find_file_path.c_str(), &find_file_data);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		if (('\\' == file_path[file_path.length() - 1]) || ('/' == file_path[file_path.length() - 1]))
		{
			m_dump_file_path = file_path;
		}
		else
		{
			m_dump_file_path = file_path + "\\";
		}
	}
	else
	{
		ERROR_FORMAT_LOG("Get bad file name while MiniDumper::filePath, the file path is <%s>\n", file_path.c_str());
	}
}

void MiniDumper::fileName(const std::string & file_name)
{
	m_dump_file_name = file_name;
}

int  MiniDumper::addFlags(Flags flags)
{
	m_flags |= (int)flags;
	return m_flags;
}

std::string make_time_string(const time_t &t)
{
	char timebuf[256] = {0};
	struct tm today;

	_localtime64_s(&today, &t);
	
	strftime(timebuf, sizeof(timebuf), "%Y-%m-%d_%H.%M.%S", &today);

	return std::string(timebuf);
}

std::string MiniDumper::newFilePath()
{
	std::string name, path;
	if (m_dump_file_name.empty())
	{
		if (m_flags & timestamp_file_name)
		{
			name = make_time_string(time(NULL)) + ".dmp";
		}
		else
		{
			name = "minidump.dmp";
		}
	}
	
	if (m_dump_file_path.empty())
	{
		TCHAR module_file_name[_MAX_PATH] = {0};
		if ( GetModuleFileName( NULL, module_file_name, _MAX_PATH ))
		{
			TCHAR *pSlash = strrchr( module_file_name, '\\' );
			if (pSlash)
			{
				pSlash[1] = '\0';
				path = module_file_name;
			}
		}
		if (path.empty())
		{
			path = "C:\\";
		}
	}

	return path + name;
}

#endif

