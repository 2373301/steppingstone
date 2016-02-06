
#include "typedef.h"
#include <boost/date_time.hpp>
#include <ace/OS.h>
#include "EffectTest.h"
#include "Logger.h"
#include "LoggerFactory.h"

EffectTest::EffectTest()
: m_test_count(100000)
,started_(false)
,stoped_(false)
{

}

int EffectTest::svc()
{	
	started_ = true;
	Logger * stat_logger = LoggerFactory::createFileLogger("./logs/stat_logger");
	Logger * stat_test_logger = LoggerFactory::createFileLogger("./logs/stat_test_logger");
	Logger * stat_sync_test_logger = LoggerFactory::createFileLogger("./logs/stat_sync_test_logger");

	int sync_log_time = 0;
	int normal_log_time = 0;

	uint32 index = 0;

	boost::posix_time::ptime start_time;
	boost::posix_time::ptime end_time;
	boost::posix_time::time_duration time_dura;
	
	{
		start_time = boost::posix_time::microsec_clock::local_time();
		for (int i = 0; i < m_test_count; ++i)
		{
			LOG_DEBUG(stat_test_logger, "normal , index is <%d>, sub index is <%d>\n", index, i);
		}
		end_time = boost::posix_time::microsec_clock::local_time();
		time_dura = end_time - start_time;
		normal_log_time += time_dura.total_milliseconds();

		start_time = boost::posix_time::microsec_clock::local_time();
		for (int i = 0; i < m_test_count; ++i)
		{
			LOG_DEBUG(stat_sync_test_logger, "sync , index is <%d>, sub index is <%d>\n", index, i);
		}
		end_time = boost::posix_time::microsec_clock::local_time();
		time_dura = end_time - start_time;
		sync_log_time += time_dura.total_milliseconds();

		++index;
		//ACE_OS::sleep(1);
	}

	int log_per_second = m_test_count * index / (sync_log_time / 1000.0 + 0.5);
	LOG_DEBUG(stat_logger, "sync log amount is <%d>, spend time <%d>, log number per second <%d>\n", m_test_count * index, sync_log_time, log_per_second);

	log_per_second = m_test_count * index / (normal_log_time / 1000.0 + 0.5);
	LOG_DEBUG(stat_logger, "normal log amount is <%d>, spend time <%d>, log number per second <%d>\n", m_test_count * index, normal_log_time, log_per_second);

	stoped_ = true;
	return 0;
}

int EffectTest::init()
{
	if (this->activate() == -1)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int EffectTest::stop()
{
	SingletonInit::stop();
	return 0;
}

int EffectTest::finit()
{
	this->wait();
	return 0;
}