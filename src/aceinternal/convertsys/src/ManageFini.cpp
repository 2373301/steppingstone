
#include <iostream>
#include <string>
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Log_Msg.h>
#include <ace/OS.h>
#include <ace/Init_ACE.h>
#include "ManageFini.h"
#include "ManageConfig.h"
#include "ManageSingleton.h"
#include "ManageSingletonInit.h"
#include "ManageSQLiteFile.h"
#include "ManageCacheSession.h"
#include "ManageLogger.h"
#include "Logger.h"
#include "Redirect.h"
#include "ManageSQLiteFileToDb.h"

using namespace std;

const string stop_str = ACE_TEXT("stop convert");

int processSqliteFileToDb(int argc, ACE_TCHAR * argv[])
{
	if (ACE::init() < 0)
	{
		//std::cout << "error to call ACE::init, the last error is : " << ACE_OS::last_error() << std::endl;
		return 1;
	}

	ManageSingleton::instance();
	ManageSingletonInit::instance();

	if (ManageLogger::instance()->init() == -1)
	{
		return 1;
	}

	if (ManageConfig::instance()->init() == -1)
	{
		DEF_LOG_ERROR("failed to init config\n");
		return 2;
	}

	if (ManageSqliteFile::instance()->init() == -1)
	{
		DEF_LOG_ERROR("failed to init sqlite file\n");
		return 3;
	}

	vector<string> sqlite_path;
	for (int i = 2; i < argc; ++i)
	{
		sqlite_path.push_back(argv[i]);
	}

	if (ManageSQLiteFileToDb::instance()->init(sqlite_path) == -1)
	{
		DEF_LOG_ERROR("failed to init sqlite file to db file\n");
		return 4;
	}

	while(true)
	{
		ACE_OS::sleep(60);
	}

	ACE::fini();
}

int startConvert(int argc, ACE_TCHAR * argv[])
{
	if (2 == argc) 
	{
#ifndef WIN32

		START_DAEMONIZE_CMD();

		//ACE::daemonize("./");
		//close(0);
		//close(1);
		//close(2);
#endif
	}
	else if (argc >= 3)
	{
		// process sqlite file to database
		processSqliteFileToDb(argc, argv);
		return 0;
	}

#ifndef WIN32
	IGNORE_LINUX_SINGLE();
#endif	

	if (ACE::init() < 0)
	{
		//std::cout << "error to call ACE::init, the last error is : " << ACE_OS::last_error() << std::endl;
		return 1;
	}

	ManageFini manage_fini;

	///注册timer，用于侦测程序退出事件
	//long time_id = ACE_Reactor::instance()->schedule_timer(&manage_fini, NULL, ACE_Time_Value(3, 0), ACE_Time_Value(3, 0));
	//if (time_id < 0)
	//{
	//	//GATELogp(LM_ERROR, ACE_TEXT("Failed to register timer of ManageFini, last error is:<%d>\n"), ACE_OS::last_error());
	//	return 1;
	//}
	//manage_fini.setTimerID(time_id);

	if (manage_fini.init(argc, argv) != 0)
	{
		//GATELog(LM_ERROR, ACE_TEXT("Failed to call manage_fini.init\n"));
		return 1;
	}

	DEF_LOG_INFO("convert start sucess, le is <%d>\n", ACE_OS::last_error());

	//ACE_Reactor::instance()->run_reactor_event_loop();

	while (true)
	{
		ACE_OS::sleep(600);
	}

	//SingletonManage::instance()->fini();
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

	ManageConfig::instance();

	ManageConfig::instance()->init();

	Logger::setFilter(ManageConfig::instance()->getLogCfg().log_level);

	uint32 dst_mask = LD_TARGET;
	if (ManageConfig::instance()->getLogCfg().is_print)
	{
		dst_mask = dst_mask | LD_SCREEN;
	}
	else
	{
		dst_mask = dst_mask & (~LD_SCREEN);
	}

	if (ManageConfig::instance()->getLogCfg().enable_remote)
	{
		dst_mask = dst_mask | LD_REMOTE;
	}
	else
	{
		dst_mask = dst_mask & (~LD_REMOTE);
	}

	//ManageLogger::instance()->setLogDestination(dst_mask);

	ManageLogger::instance();

	ManageSqliteFile::instance();

	ManageCacheSession::instance();

	return ManageSingletonInit::instance()->init();

	///初始化所有需要被初始化的对象
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
	//this->reactor()->end_reactor_event_loop();
}
