/// cache.cpp
/// 
///

#ifdef WIN32
//#include <vld.h>
#endif
#include <boost/scoped_ptr.hpp>
#include <ace/Event_Handler.h>
#include <iostream>
#include "cached.h"
//#include "net.h"
#include "config.h"
//#include "stat.h"
#include "ManageSerialize.h"
#include "ManageConvertSession.h"
#include "ManageLogger.h"
#include "ManageSingleton.h"
//#include "ManageTerminalSession.h"
#include "ManageSingletonInit.h"
#include "ManageSessionPool.h"
#include "DbLoader.h"
#include "EntityHander.h"
#include "Redirect.h"
#include "ManageSerialize.h"

const string stop_str = ACE_TEXT("stop cache");

string stop_service_cmd_name = "stop cache";

struct CacheRunInfo 
{
	CacheRunInfo()
	: is_stopping(false)
	{}

	bool is_stopping;
};

CacheRunInfo g_cache_run_info;

class StopService : public ACE_Event_Handler
{
public:
	StopService()
	: m_timer_id(0)
	{}
	virtual int handle_timeout (const ACE_Time_Value &current_time, const void *act = 0)
	{
		//char cmd[101] = {0};
		//std::cin.getline(cmd, 100);
		//if (stop_service_cmd_name.compare(cmd) == 0)
		//{
		//	ACE_Reactor::instance()->cancel_timer(m_timer_id);
		//	ACE_Reactor::instance()->end_reactor_event_loop();
		//}
		ACE_OS::sleep(1);

		return 0;
	}

	void setTimerID(long timer_id)
	{
		m_timer_id = timer_id;
	}
private:
	long m_timer_id;
};

void stopCache()
{
	ManageConvertSession::instance()->stop();
	
	cached::service::ManageSerialize::instance()->stop();

	//ManageSingletonInit::instance();

	//EntityHander::instance()->stop();

	//DbLoader::instance()->stop();

	//ManageSessionPool::instance()->stop();

	ManageSingletonInit::instance()->stop();

}

bool cacheStopped()
{
	ManageConvertSession::instance()->finit();

	cached::service::ManageSerialize::instance()->finit();

	ManageSingletonInit::instance()->finit();

	ManageSingleton::instance()->finit();

	return true;
}

int startCache(int argc, char * argv[])
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
#endif

	using namespace cached::service;

	string boost_format("cache_log_old");
	::boost::shared_ptr < ::std::ostream > osm = ::boost::shared_ptr< ::std::ostream >(new ::std::ofstream(boost_format.c_str()));
	if (osm.get() == NULL)
		return -1;

	//ACE_LOG_MSG->msg_ostream(osm.get());
	//ACE_LOG_MSG->open(argv[0], ACE_Log_Msg::OSTREAM | ACE_Log_Msg::STDERR, "cache_log");

	StopService stop_serivces;

	ManageSingleton::instance();

	ManageSingletonInit::instance();

	ManageLogger::instance();
	ManageLogger::instance()->init();

	CONTAINER;
	//if (CONTAINER->open() == -1)
	//	return -1;

	ManageSessionPool::instance();

	DbLoader::instance();

	EntityHander::instance();

	EntityHander::instance()->setOutputHandle(ManageSessionPool::instance());
	if (ManageSingletonInit::instance()->init() == -1)
	{
		return -1;
	}

	DEF_LOG_INFO("Starting log service......\n");


	DEF_LOG_INFO("Starting open singletion......\n");

	if (!ManageSerialize::instance()->open())
	{
		return -1;
	}

	if (ManageConvertSession::instance()->init(CONTAINER->lookup(SERIALIZE_LISTEN_ADDR)) == -1)
	{
		return -1;
	}

	//ManageTermminalSession::instance()->init();

	/// 
	DEF_LOG_INFO("Starting reactor......, le : <%d>\n", ACE_OS::last_error());

	DEF_LOG_INFO("Server running......, le : <%d>\n", ACE_OS::last_error());
//	ACE_Reactor::instance()->owner(ACE_OS::thr_self());
	//ACE_Time_Value timer_interval(3);
	//long timer_id = ACE_Reactor::instance()->schedule_timer(&stop_serivces, 0, timer_interval, timer_interval);
	//stop_serivces.setTimerID(timer_id);
	while (true)
	{
		//ACE_Reactor::instance()->run_reactor_event_loop();
		if (argc >= 2) 
		{
			ACE_OS::sleep(600);
		}
		else
		{
			if (!g_cache_run_info.is_stopping)
			{
				char buf[256] = {0};
				std::cin.getline(buf, 255);
				if (stop_str.compare(buf) == 0)
				{
					// stop gs
					g_cache_run_info.is_stopping = true;
					stopCache();
				}
			}
			else
			{
				// check
				if (cacheStopped())
				{
					break;
				}
			}
		}

//#ifdef WIN32
//		break;
//#else
//		int last_error = ACE_OS::last_error();
//		if ((last_error == EWOULDBLOCK) || (last_error == EINTR) || (last_error == EAGAIN))
//		{
//			continue;
//		}
//		else
//		{
//			break;
//		}
//#endif

	}

	//DEF_LOG_INFO("server ending..., le : <%d>\n", ACE_OS::last_error());

	//if (CONTAINER->close() == -1)
	//	return -1;

//	ManageTermminalSession::instance()->stop();

//	ManageTermminalSession::instance()->finit();

	//ManageSerialize::instance()->stop();

	//ManageSerialize::instance()->finit();

	//ManageConvertSession::instance()->stop();

	//ManageConvertSession::instance()->finit();

	//std::cout << "start to call finit of ManageSingletion" << std::endl;

	//ManageSingleton::instance()->finit();

	//std::cout << "end to call finit of ManageSingletion" << std::endl;

	return 0;
}

int main(int argc, char * argv[])
{
    /// Verify that the version of the library that we linked against is
    /// compatible with the version of the headers we compiled against.
    //GOOGLE_PROTOBUF_VERIFY_VERSION;
    /// 
	//DEF_LOG_INFO("Starting cache service......, le : <%d>\n", ACE_OS::last_error());
//#ifndef WIN32
//    ACE::daemonize("./");
//#endif

    ////ACE_Date_Time adt(ACE_OS::gettimeofday());
    ////::boost::format boost_format("cache_%04d_%02d_%02d_%02d_%02d_%02d.log");
    ////boost_format % adt.year() % adt.month() % adt.day() % adt.hour() % adt.minute() % adt.second();
//#ifndef WIN32

// 	if (ACE::init() == -1)
// 	{
// 		std::cout << "failed to inti ace" << std::endl;
// 		return -1;
// 	}

	startCache(argc, argv);

	std::cout << "end of startCache" << std::endl;

    /// Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    /// 

	std::cout << "start to fini ACE library" << std::endl;

	//ACE::fini();
    return 0;
}