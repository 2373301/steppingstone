
#include <iostream>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include "typedef.h"
#include "ManageFini.h"
#include "ManageSingleton.h"
#include "ManageSingletonInit.h"
#include "ManageLogger.h"
#include "ManageConfig.h"
#include "ManageSession.h"

#ifndef WIN32
#include <signal.h>
#include <unistd.h>
#endif

using namespace std;

int startProcess(int argc, char * argv[])
{

// 	if (ACE::init() == -1)
// 	{
// 		std::cout << "error to init ACE" << std::endl;
// 		return -1;
// 	}

#ifndef WIN32
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
#endif

	ManageFini manage_fini;
	if (manage_fini.init() == -1)
	{
		std::cout << "error to init ManageFini" << std::endl;
	}

	if (manage_fini.finit() == -1)
	{
		std::cout << "error to fini ManageFini" << std::endl;
	}

	//ACE::fini();

	return 0;
}

int ManageFini::init()
{
	ManageSingleton::instance();
	ManageSingletonInit::instance();
	ManageLogger::instance();
	ManageConfig::instance();
	ManageSession::instance();

	ManageSingletonInit::instance()->init();

	ACE_Reactor::instance()->run_reactor_event_loop();

	return 0;
}

int ManageFini::finit()
{
	ManageSingletonInit::instance()->stop();
	ManageSingletonInit::instance()->finit();

	ManageSingleton::instance()->finit();

	return 0;
}