#ifndef _MANAGECLIENTSESSION_HPP
#define _MANAGECLIENTSESSION_HPP

#include <ace/Task.h>
#include <ace/Reactor.h>
#include "AppNotifyInterface.h"
#include "Singleton.h"

class ManageClientSession : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	ManageClientSession();

	~ManageClientSession();
public:
	static ManageClientSession * instance()
	{
		return Singleton<ManageClientSession>::instance();
	}
public:
	/// ACE_Task_Base svc
	virtual int svc (void);

	/// ACE_Event_Handle handle_timeout
	virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act=0);
public:
	/// AppNotifyInterface init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// AppNotifyInterface fini, stop
	virtual int fini();

	/// AppNotifyInterface wait for finish
	virtual int wait();
public:

private:
	bool m_stop;

	bool m_wait;

	long m_timer_id;

	ACE_Thread_Mutex m_mutex;

	ACE_Reactor * m_reactor;
};


#endif