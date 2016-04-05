

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC 
#include<stdlib.h> 
#include<crtdbg.h> 
#endif

#include "Redirect.h"

#ifndef WIN32
#include <signal.h>
#include <unistd.h>
#endif

#include <iostream>
#include <ace/ACE.h>

class MemoryLeak
{
public:
	~MemoryLeak()
	{
#ifdef WIN32
		_CrtDumpMemoryLeaks();
#endif
	}
};

int startProc(int argc, char * argv[]);

MemoryLeak g_memory_leak;

int main(int argc, char * argv[])
{
	if (argc >= 2) 
	{
#ifndef WIN32

		START_DAEMONIZE_CMD();

		//ACE::daemonize("./");
		//close(0);
		//close(1);
		//close(2);
#endif
	}
	
#ifndef WIN32

	IGNORE_LINUX_SINGLE();

	//signal(SIGQUIT, SIG_IGN);
	//signal(SIGPIPE, SIG_IGN);
#endif

	try
	{
		return startProc(argc, argv);
	}
	catch(...)
	{
		std::cout << "Raise unknown exception in main\n";
	}
	return 1;
};
