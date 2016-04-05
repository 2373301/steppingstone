
#include <iostream>
#include <string>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Log_Msg.h>
#include <ace/OS_NS_unistd.h>
#include <ace/Init_ACE.h>
#include "ManageFini.h"
#include "ManageSingleton.h"
#include "ManageSingletonInit.h"
//#include "ManageLog.h"
#include "ManageConfig.h"
#include "ManageGateSession.h"
#include "RouteGatePacket.h"
#include "RouteCachePacket.h"
#include "ManageAccountDatabase.h"
#include "ManageLogger.h"
#include "Logger.h"
#include "ManageGuid.h"
#include "ManageJobInit.h"
#include "ManagePlayerInit.h"
#include "ManageRoleRes.h"
#include "ManageRoundomName.h"

#ifdef WIN32
#include "MiniDumper.h"
#endif

using std::string;

const string stop_str = ACE_TEXT("stop char");

#ifdef WIN32
MiniDumper g_mini_dump1;
#endif

int startProc(int argc, ACE_TCHAR * argv[])
{
	if (ACE::init() < 0)
	{
		//std::cout << "error to call ACE::init, the last error is : " << ACE_OS::last_error() << std::endl;
		return 1;
	}

	ManageSingleton::instance();
	ManageSingletonInit::instance();

	//// todo 
	//ACE_LOG_MSG->priority_mask(~LM_DEBUG, ACE_Log_Msg::PROCESS);

	ManageFini manage_fini;

	///注册timer，用于侦测程序退出事件
	//long time_id = ACE_Reactor::instance()->schedule_timer(&manage_fini, NULL, ACE_Time_Value(3, 0), ACE_Time_Value(3, 0));
	//if (time_id < 0)
	//{
	//	DEF_LOG_ERROR("Failed to register timer of ManageFini, last error is:<%d>\n", ACE_OS::last_error());
	//	return 1;
	//}
	//manage_fini.setTimerID(time_id);

	if (manage_fini.init(argc, argv) != 0)
	{
		DEF_LOG_ERROR("Failed to call manage_fini.init\n");
		return 1;
	}
	else
	{
		DEF_LOG_INFO("success to start charsys\n");
	}

	//ACE_Reactor::instance()->run_reactor_event_loop();
	// todo
	while(true)
	{
		ACE_OS::sleep(600);
	}

	ManageSingleton::instance()->finit();
	if (ACE::fini() < 0)
	{
		//std::cout << "error to call ACE::fini, the last error is : " << ACE_OS::last_error() << std::endl;
		return 1;
	}
	return 0;
}

ManageFini::ManageFini()
: m_timer_id(0)
{

}

ManageFini::~ManageFini()
{

}

int ManageFini::init(int argc, ACE_TCHAR * argv[])
{
	///初始化全局静态对象
	ManageLogger::instance();
	ManageConfig::instance();

	ManageGuid::instance();

	ManageJobInit::instance();

	ManagePlayerInit::instance();
	ManageRoleRes::instance();
	ManageRoundomName::instance();

	ManageAccountDatabase::instance();
	RouteCachePacket::instance();
	RouteGatePacket::instance();
	ManageGateSession::instance();

	//////////////////////////////////////////////////////////////////////////

	//ManageLogger::instance()->init();

	//ManageConfig::instance()->init();
	//DEF_LOG_INFO("finish to init manage config\n");
	////ACE_OS::sleep(3);

	//ManageGuid::instance()->init();
	//DEF_LOG_INFO("finish to init manage guid\n");
	////ACE_OS::sleep(3);

	//ManageJobInit::instance()->init();
	//DEF_LOG_INFO("finish to init manage job\n");
	////ACE_OS::sleep(3);


	//ManagePlayerInit::instance()->init();
	//DEF_LOG_INFO("finish to init manage player\n");
	////ACE_OS::sleep(3);

	//ManageRoleRes::instance()->init();
	//DEF_LOG_INFO("finish to init manage role\n");
	////ACE_OS::sleep(3);

	//ManageAccountDatabase::instance()->init();
	//DEF_LOG_INFO("finish to init manage account\n");
	////ACE_OS::sleep(3);

	//RouteCachePacket::instance()->init();
	//DEF_LOG_INFO("finish to init manage cache\n");
	////ACE_OS::sleep(3);

	//RouteGatePacket::instance()->init();
	//DEF_LOG_INFO("finish to init manage gate packet\n");
	////ACE_OS::sleep(3);

	//ManageGateSession::instance()->init();
	//DEF_LOG_INFO("finish to init manage gate session\n");
	////ACE_OS::sleep(3);

	//return 0;

	//////////////////////////////////////////////////////////////////////////

	///初始化所有需要被初始化的对象
	std::cout << "start to init ..." << std::endl;
	return ManageSingletonInit::instance()->init();
}

void ManageFini::setTimerID(long timer_id)
{
	m_timer_id = timer_id;
}

int ManageFini::handle_timeout (const ACE_Time_Value &current_time, const void *act)
{
	//ACE_TCHAR buf[128] = {0};
	//std::cin.getline(buf, 127);
	//if (stop_str.compare(buf) == 0)
	//{
	//	reactor()->cancel_timer(m_timer_id);
	//	stop();
	//}
	return 0;
}

void ManageFini::stop()
{
	ManageSingletonInit::instance()->stop();
	ManageSingletonInit::instance()->finit();

	ManageSingleton::instance()->finit();

	//this->reactor()->end_reactor_event_loop();
}