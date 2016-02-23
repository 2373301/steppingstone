#ifndef _MANAGE_GS_STREAM_HPP
#define _MANAGE_GS_STREAM_HPP

#include <list>
#include <ace/Task.h>
#include "AppNotifyInterface.h"
#include "Singleton.h"
#include "GSSession.h"
#include "HandleGSStream.h"

class ManageGSStream : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageGSStream();
	~ManageGSStream();
public:
	static ManageGSStream * instance()
	{
		return Singleton<ManageGSStream>::instance();
	}
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	typedef list<HandleGSStream *> HandleGSStreamList;

	virtual int svc(void);

public:
	/// AppNotifyInterface init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// AppNotifyInterface fini, stop
	virtual int fini();

	/// AppNotifyInterface wait for finish
	virtual int wait();

	int activeThread(int thread_no);
public:
	void handleStream(GSSession * gs_session, ACE_Reactor_Mask mask);

	bool isStop();

	bool isWait();
private:

	void collectFreeThread(int thread_no);
private:
	bool m_stop;

	bool m_wait;

	int m_free_thread;

	ACE_Thread_Mutex m_mutex;

	ACE_Thread_Mutex m_thread_number_mutex;

	HandleGSStreamList m_gs_stream_list;
};

#endif