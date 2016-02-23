
#include "ManageClientStreamLimit.h"
#include "ManageConfig.h"
#include "ClientStrategyCenter.h"

class find_stream_info
{
public:
	find_stream_info(uint64 guid)
		: m_guid(guid)
	{}
	bool operator ()(const StreamInfo & stream_info)
	{
		return stream_info.guid == m_guid;
	}
private:
	uint64 m_guid;
};

ManageClientStreamLimit::ManageClientStreamLimit()
: m_stop(false)
, m_wait(true)
, m_stream_limit_speed(4096)
, m_stream_stat_interval(10)
{
	m_wait = false;
}

ManageClientStreamLimit::~ManageClientStreamLimit()
{
}

int ManageClientStreamLimit::svc(void)
{
	StreamInfoList_t stream_info_list;
	StreamInfoList_t stream_remove_list;
	uint64 current_time_in_sec;
	while (!m_stop)
	{
		{
			ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_stream_info_mutex, );
			std::copy(m_stream_info_list.begin(), m_stream_info_list.end(), std::back_inserter(stream_info_list));
			m_stream_info_list.clear();
		}

		{
			ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_stream_remove_list_mutex, );
			std::copy(m_stream_remove_list.begin(), m_stream_remove_list.end(), std::back_inserter(stream_remove_list));
			m_stream_remove_list.clear();
		}

		for (StreamInfoList_t::iterator it = stream_remove_list.begin(); it != stream_remove_list.end(); ++it)
		{
			{
				StreamStatInfoMap_t::iterator remove_it = m_stream_stat_info_map.find(it->guid);
				if (remove_it != m_stream_stat_info_map.end())
				{
					m_stream_stat_info_map.erase(remove_it);
				}
				else
				{
					GATE_LOG_ERROR(ACE_TEXT("failed to get palyer stream info in m_stream_stat_info_map, guid is <%llu>\n"), it->guid);
				}
			}

			std::remove_if(stream_info_list.begin(), stream_info_list.end(), find_stream_info(it->guid));
		}
		stream_remove_list.clear();
		
		current_time_in_sec = ACE_OS::gettimeofday().sec();

		for (StreamInfoList_t::iterator it = stream_info_list.begin(); it != stream_info_list.end(); ++it)
		{
			m_stream_stat_info_map[it->guid].addStream(current_time_in_sec, it->stream_size);
			//StreamStatInfoMap_t::iterator stat_it = m_stream_stat_info_map.find(it->guid);
			//if (stat_it != m_stream_stat_info_map.end())
			//{
			//	stat_it->second.addStream(current_time_in_sec, it->stream_size);
			//}
			//else
			//{
			//	m_stream_stat_info_map[it->guid].addStream(current_time_in_sec, it->stream_size);
			//}
		}
		stream_info_list.clear();

		int speed = 0;
		for (StreamStatInfoMap_t::iterator it = m_stream_stat_info_map.begin(); it != m_stream_stat_info_map.end(); ++it)
		{
			it->second.removeOverdueStream(current_time_in_sec, m_stream_stat_interval);
			speed = it->second.statSpeed();
			if (speed > m_stream_limit_speed)
			{
				GATE_LOG_ERROR(ACE_TEXT("get client stream speed is big, guid is <%llu>, value is <%d>\n"), it->first, speed);
				ClientStrategyCenter::instance()->streamSpeedException(it->first);
			}
		}		
	}

	m_wait = false;
	return 0;
}

int ManageClientStreamLimit::init(int argc, ACE_TCHAR * argv[])
{
	m_stream_limit_speed = ManageConfig::instance()->getClientCfg().stream_limit_spead;
	m_stream_stat_interval = ManageConfig::instance()->getClientCfg().stream_stat_interval;
	if (this->activate() == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("failed to active class of ManageClientStreamLimit\n"));
		return -1;
	}

	return 0;
}

/// fini, stop
int ManageClientStreamLimit::fini()
{
	m_stop = true;
	return 0;
}

/// wait for finish
int ManageClientStreamLimit::wait()
{
	while(m_wait)
	{
		DEF_LOG_INFO("wait for ManageClientStreamLimit\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

void ManageClientStreamLimit::stream_info(uint64 player_guid, int stream_size)
{
	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_stream_info_mutex, );
	m_stream_info_list.push_back(StreamInfo(player_guid, stream_size));
}

void ManageClientStreamLimit::remove_stream(uint64 player_guid)
{
	return ;

	ACE_GUARD_REACTION(ACE_Thread_Mutex, guard, m_stream_remove_list_mutex, );
	m_stream_remove_list.push_back(StreamInfo(player_guid, 0));
}