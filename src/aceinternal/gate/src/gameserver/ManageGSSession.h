#ifndef _MANAGE_GSSESSION_HPP
#define _MANAGE_GSSESSION_HPP

#include <ace/Task.h>
#include "AppNotifyInterface.h"
#include "Singleton.h"
#include "GSSession.h"

typedef ACE_Acceptor<GSSession, ACE_SOCK_ACCEPTOR> GSAcceptorInstance;

class ManageGSSession : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;
	
	ManageGSSession();

	~ManageGSSession();
public:
	static ManageGSSession * instance()
	{
		return Singleton<ManageGSSession>::instance();
	}
public:
	typedef std::list<GSSession *> GSSessionList;
	virtual int svc(void);

	/// ACE_Event_Handle handle_timeout
	virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act=0);
public:
	/// AppNotifyInterface init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// AppNotifyInterface fini, stop
	virtual int fini();

	/// AppNotifyInterface wait for finish
	virtual int wait();

	void addGSSession(GSSession * gs_session);

	void removeGSSession(GSSession * gs_session);
private:
	void closeGSSession();

private:
	bool m_stop;

	bool m_wait;

	long m_timer_id;

	ACE_Thread_Mutex m_mutex;

	ACE_Reactor * m_reactor;

	GSSessionList m_gs_session_list;

	ACE_Thread_Mutex m_gs_session_mutex;
};

#endif