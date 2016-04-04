// membersys.cpp : 定义控制台应用程序的入口点。
//

#ifndef WIN32
#include <signal.h>
#include <unistd.h>
#endif

#include <iostream>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Init_ACE.h>
#include "typedef.h"
#include "Logger.h"
#include "Singleton.h"
#include "ManageSingleton.h"
#include "ManageLogger.h"
#include "Redirect.h"
#include "ManageConfig.h"
#include "ManageMemberSession.h"
#include "ManageGateSession.h"
#include "ManageMemberListen.h"
#include "ManageMemberRegister.h"
#include "ManageRoleDb.h"
#include "ManageMemberSessionCooler.h"
#include "ManageMemberSessionOutput.h"
#include "ManageOnlinePlayer.h"

int startMember()
{
	if (ACE::init() < 0)
	{
		//std::cout << "error to call ACE::init, the last error is : " << ACE_OS::last_error() << std::endl;
		return 1;
	}

	ManageSingleton::instance();

	if (ManageLogger::instance()->init() == -1)
	{
		return 1;
	}

	DEF_LOG_INFO("success to init logger\n");

	if (ManageConfig::instance()->init("member_config.xml") == -1)
	{
		DEF_LOG_ERROR("failed to load config\n");
		return 1;
	}

	if (ManageOnlinePlayer::instance()->init() == -1)
	{
		DEF_LOG_ERROR("failed to init ManageOnlinePlayer\n");
		return 1;
	}

	if (ManageRoleDb::instance()->init(ManageConfig::instance()->getAccountDb(), ManageConfig::instance()->getGameDb()) == -1)
	{
		DEF_LOG_ERROR("failed to inti manage role db\n")
			return 1;
	}

	if (ManageGateSession::instance()->init() == -1)
	{
		DEF_LOG_ERROR("failed to init manage gate session\n");
		return 1;
	}

	if (ManageMemberSessionCooler::instance()->init() == -1)
	{
		DEF_LOG_ERROR("failed to init manage session cooler\n");
		return 1;
	}

	if (ManageMemberSessionOutput::instance()->init() == -1)
	{
		DEF_LOG_ERROR("failed to init manage session output\n");
		return 1;
	}

	if (ManageMemberSession::instance()->init() == -1)
	{
		DEF_LOG_ERROR("failed to init manage member session\n");
		return 1;
	}

	if (ManageMemberRegister::instance()->init())
	{
		DEF_LOG_ERROR("failed to init mange member register\n");
		return 1;
	}

	if (ManageMemberListen::instance()->init() == -1)
	{
		DEF_LOG_ERROR("failed to init manage member listen\n");
		return 1;
	}

	while (true)
	{
		ACE_OS::sleep(600);
	}

	return 0;
}

int main(int argc, char * argv[])
{
	if (argc >= 2) 
	{
#ifndef WIN32
		START_DAEMONIZE_CMD();
		//ACE::daemonize("./");
		//Redirect * redirect = new Redirect();
#endif
	}


#ifndef WIN32
	IGNORE_LINUX_SINGLE();
	//signal(SIGQUIT, SIG_IGN);
	//signal(SIGPIPE, SIG_IGN);
#endif

	int r = startMember();
	ACE_OS::sleep(3);
	return r;
}


