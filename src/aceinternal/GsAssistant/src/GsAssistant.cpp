// GsAssistant.cpp : 定义控制台应用程序的入口点。
//

#ifndef WIN32
#include <signal.h>
#include <unistd.h>
#endif

#include <iostream>
#include "ace/OS_NS_unistd.h"
#include "ManageSingleton.h"
#include "ManageLogger.h"
#include "ManageSession.h"
#include "Logger.h"

void usage()
{
	std::cout << "please input the type and port" << std::endl;
}

int main(int argc, char * argv[])
{
	if (argc < 3)
	{
		usage();
		return -1;
	}

#ifndef WIN32
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	//signal(SIGUSR1, singleFlush);
#endif

	int monitor_type = atoi(argv[1]);

	//ACE::init();

	ManageSingleton::instance();
	ManageLogger::instance();

	if (ManageLogger::instance()->init() == -1)
	{
		std::cout << "failed to init ManageLogger" << std::endl;
		return -1;
	}


	ManageSession manage_session;
	if (manage_session.init(monitor_type, argv[2]) == -1)
	{
		DEF_LOG_ERROR("failed to init ManageSession\n");
		return -1;
	}

	while (true)
	{
		ACE_OS::sleep(1);
		if (CGT_STARTUP == monitor_type)
		{
			DEF_LOG_INFO("check for gs startup response ......\n");
		}
		else if (CGT_SHUTDOWN == monitor_type)
		{
			DEF_LOG_INFO("check for gs shutdown response ......\n");
		}
		if (manage_session.isFinish())
		{
			break;
		}
	}

	manage_session.finit();

	if (manage_session.isSuccess())
	{
		DEF_LOG_INFO("get gs repsonse : success\n");
		return 0;
	}
	else
	{
		DEF_LOG_ERROR("get gs repsonse : failed\n");
		return -1;
	}
}