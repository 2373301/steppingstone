
#include <iostream>
#include <string>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Log_Msg.h>
#include <ace/Init_ACE.h>
#include "AppNotifyInterface.h"
#include "ManageFini.h"
//#include "ManageDatabase.h"
#include "ManageClientSession.h"
#include "ManageConfig.h"
#include "GateLogger.h"
//#include "SingletonManage.h"
#include "RouteClientPackage.h"
#include "ManageGSSession.h"
#include "ManageGSStream.h"
#include "RouteGSPacket.h"
#include "ManageStat.h"
#include "ManageClientSessionCooler.h"
#include "ManageGSSessionCooler.h"
#include "ManageClientValidation.h"
#include "ManageClientConnectionTimeout.h"
#include "ManageClientStreamEx.h"
#include "ManageClientBeat.h"
#include "ManageTranscriptStream.h"
#include "ManageRoleSession.h"
#include "RouteRolePacket.h"
#include "ManageLoginStream.h"

#include "ManageSingleton.h"
#include "ManageSingletonInit.h"
#include "ManageLogger.h"
#include "Redirect.h"
#include "ManageMasterSession.h"
#include "ManageReduplication.h"

#ifdef WIN32
#include "MiniDumper.h"
#endif

#ifndef WIN32
#include <signal.h>
#include <unistd.h>
#endif

using std::string;

const string stop_str = ACE_TEXT("stop gate");

struct GateRunInfo 
{
	GateRunInfo()
		: is_stopping(false)
	{}

	bool is_stopping;
};

GateRunInfo g_gate_run_info;

#ifdef WIN32
///MiniDumper g_mini_dump1;
#endif

int startGate(int argc, ACE_TCHAR * argv[])
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

	if (ACE::init() < 0)
	{
		//std::cout << "error to call ACE::init, the last error is : " << ACE_OS::last_error() << std::endl;
		return 1;
	}

#ifndef WIN32
	IGNORE_LINUX_SINGLE();
	//signal(SIGQUIT, SIG_IGN);
	//signal(SIGPIPE, SIG_IGN);
#endif

//	ManageSingleton::instance()->init();

	//// todo 
	//ACE_LOG_MSG->priority_mask(~LM_DEBUG, ACE_Log_Msg::PROCESS);

	ManageFini manage_fini;

	///注册timer，用于侦测程序退出事件
	//long time_id = ACE_Reactor::instance()->schedule_timer(&manage_fini, NULL, ACE_Time_Value(3, 0), ACE_Time_Value(3, 0));
	//if (time_id < 0)
	//{
	//	std::cout << "Failed to register timer of ManageFini, last error is:" << ACE_OS::last_error();
	//	return 1;
	//}
	//manage_fini.setTimerID(time_id);

	if (manage_fini.init(argc, argv) != 0)
	{
		std::cout << ACE_TEXT("Failed to call manage_fini.init\n");
		return 1;
	}

	REPORT_THREAD_INFO("main", ACE_OS::thr_self());

	//ACE_Reactor::instance()->run_reactor_event_loop();

	while (true)
	{
		if (argc >= 2) 
		{
			ACE_OS::sleep(600);
		}
		else
		{
			if (!g_gate_run_info.is_stopping)
			{
				char buf[256] = {0};
				std::cin.getline(buf, 255);
				if (stop_str.compare(buf) == 0)
				{
					// stop gs
					g_gate_run_info.is_stopping = true;
					manage_fini.stop();
					break;
				}
			}
		}
	}

//	ManageSingleton::instance()->fini();
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

	ManageSingleton::instance();
	ManageSingletonInit::instance();
	ManageLogger::instance()->init();

	if (ManageConfig::instance()->init(argc, argv) != 0)
	{
		DEF_LOG_ERROR("failed to init ManageConfig\n");
		return -1;
	}

	if (ManageReduplication::instance()->init() != 0)
	{
		DEF_LOG_ERROR("failed to init managereduplication\n");
		return -1;
	}

	//Logger::setFilter(ManageConfig::instance()->getLogCfg().log_level);

	//uint32 dst_mask = LD_TARGET;
	//if (ManageConfig::instance()->getLogCfg().is_print)
	//{
	//	dst_mask = dst_mask | LD_SCREEN;
	//}
	//else
	//{
	//	dst_mask = dst_mask & (~LD_SCREEN);
	//}

	//if (ManageConfig::instance()->getLogCfg().enable_remote)
	//{
	//	dst_mask = dst_mask | LD_REMOTE;
	//}
	//else
	//{
	//	dst_mask = dst_mask & (~LD_REMOTE);
	//}

	//GATE_LOG_DEBUG("debug level test\n");

	//ManageLogger::instance()->setLogDestination(dst_mask);

	ManageAppNotifyInterface::instance();
	//ManageLog::instance();
	ManageConfig::instance();
	ManageStat::instance();
//	ManageDatabase::instance();
	ManageClientSessionCooler::instance();
	ManageGSSessionCooler::instance();
	ManageClientValidation::instance();
	ManageClientBeat::instance();

	ManageMasterSession::instance();

	/// for role
	RouteRolePacket::instance();
	ManageRoleSession::instance();

	/// for client
	RouteClientPackage::instance();
	ManageClientStreamEx::instance();
	//ManageClientStream::instance();
	ManageClientSession::instance();

	/// for gs
	RouteGSPacket::instance();
	ManageGSStream::instance();
	ManageTranscriptStream::instance();
	ManageGSSession::instance();

	ManageClientConnectionTimeout::instance();
	ManageLoginStream::instance();

	if (ManageSingletonInit::instance()->init() == -1)
	{
		std::cout << "failed to init ManageSingletonInit" << std::endl;
		return -1;
	}

	///初始化所有需要被初始化的对象
	return ManageAppNotifyInterface::instance()->initApp(argc, argv);
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

	ACE_OS::sleep(1);

	return 0;
}

void ManageFini::stop()
{
	ManageAppNotifyInterface::instance()->finiApp();
	ManageAppNotifyInterface::instance()->waitApp();
	ManageAppNotifyInterface::instance()->clear();

	ManageSingletonInit::instance()->stop();
	ManageSingletonInit::instance()->finit();

	ManageSingleton::instance()->finit();

	//this->reactor()->end_reactor_event_loop();
}
