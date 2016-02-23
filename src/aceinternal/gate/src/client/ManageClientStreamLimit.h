
#ifndef MANAGE_CLIENT_STREAM_LIMIT_HPP
#define MANAGE_CLIENT_STREAM_LIMIT_HPP

#include <list>
#include <map>
#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Global_Macros.h>
#include "Singleton.h"
#include "GateLogger.h"
#include "ClientSession.h"
#include "AppNotifyInterface.h"

using namespace std;

struct StreamTime 
{
	StreamTime()
	: time_in_sec(0)
	, stream_size(0)
	{}

	StreamTime(uint64 tis, int ss)
	: time_in_sec(tis)
	, stream_size(ss)
	{}

	uint64 time_in_sec;

	int	   stream_size;
};

class find_stream_time
{
public:
	find_stream_time(uint64 cti, int ti)
		: curr_time_in_sec(cti)
		, time_interval(ti)
	{}

	bool operator ()(const StreamTime & stream_time)
	{
		return curr_time_in_sec - stream_time.time_in_sec > time_interval;
	}
private:
	uint64 curr_time_in_sec;
	int time_interval;
};

class StreamStatInfo 
{
public:
	StreamStatInfo()
	: m_guid(0)
	{
	}

	void addStream(uint64 time_in_sec, int stream_size)
	{
		if (m_stream_list.size() > 0)
		{
			if (m_stream_list.rbegin()->time_in_sec == time_in_sec)
			{
				m_stream_list.rbegin()->stream_size = m_stream_list.rbegin()->stream_size + stream_size;
				return;
			}
		}
		m_stream_list.push_back(StreamTime(time_in_sec, stream_size));
	}

	int statSpeed()
	{
		int result = 0;
		if (m_stream_list.size() >= 2)
		{
			uint64 first_time = m_stream_list.begin()->time_in_sec;
			uint64 last_time = m_stream_list.rbegin()->time_in_sec;
			int total_size = 0;

			for (StreamList_t::iterator it = m_stream_list.begin(); it != m_stream_list.end(); ++it)
			{
				total_size += it->stream_size;
			}
			
			int time_interval = last_time - first_time;
			time_interval = max(time_interval, 1);
			result = total_size / time_interval;
		}

		return result;
	}

	void removeOverdueStream(uint64 curr_time_in_sec, int time_interval)
	{
		std::remove_if(m_stream_list.begin(), m_stream_list.end(), find_stream_time(curr_time_in_sec, time_interval));
	}

private:
	typedef list<StreamTime> StreamList_t;

	uint64 m_guid;
	
	StreamList_t m_stream_list;
};

struct StreamInfo 
{
	StreamInfo()
	: guid(0)
	, stream_size(0)
	{}

	StreamInfo(uint64 gd, int ss)
	: guid(gd)
	, stream_size(ss)
	{}

	uint64 guid;
	int    stream_size;
};

class ManageClientStreamLimit : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageClientStreamLimit();

	~ManageClientStreamLimit();

public:
	static ManageClientStreamLimit * instance()
	{
		return Singleton<ManageClientStreamLimit>::instance();
	}
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	virtual int svc(void);

public:
	int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	int fini();

	/// wait for finish
	int wait();
public:
	void stream_info(uint64 player_guid, int stream_size);

	void remove_stream(uint64 player_guid);
protected:
private:
	typedef list<StreamInfo> StreamInfoList_t;

	typedef map<uint64, StreamStatInfo> StreamStatInfoMap_t;

	bool m_stop;

	bool m_wait;

	int m_stream_limit_speed;

	int m_stream_stat_interval;

	ACE_Thread_Mutex m_stream_info_mutex;

	ACE_Thread_Mutex m_stream_remove_list_mutex;

	StreamInfoList_t m_stream_info_list;

	StreamInfoList_t m_stream_remove_list;

	StreamStatInfoMap_t m_stream_stat_info_map;
};
#endif