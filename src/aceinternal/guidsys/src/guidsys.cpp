// guidsys.cpp : 定义控制台应用程序的入口点。
//

#include <ace/ACE.h>
#include "Redirect.h"

int startProcess(int argc, char * argv[]);

int main(int argc, char * argv[])
{
	if (argc >= 2) 
	{
#ifndef WIN32
		START_DAEMONIZE_CMD();
		//ACE::daemonize("./");
		// close(0);
		// close(1);
		// close(2);
		// ACE::daemonize("./");
		// close(0);
		// close(1);
		// close(2);
#endif
	}
	
#ifndef WIN32
	
	IGNORE_LINUX_SINGLE();
	
#endif

	return startProcess(argc, argv);
}
