// world.cpp : Defines the entry point for the console application.
//

#include <ace/ACE.h>
#include "World.h"
#include "Redirect.h"

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC 
#include<stdlib.h> 
#include<crtdbg.h> 
#endif

#ifdef WIN32

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
	MemoryLeak g_memory_leak;

#endif

int main(int argc, char * argv[])
{
	if (argc >= 3) 
	{
#ifndef WIN32
		START_DAEMONIZE_CMD();
		//ACE::daemonize("./");
		//close(0);
		//close(1);
		//close(2);
#endif
	}

#ifdef WIN32
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	return startWorld(argc, argv);
}


