
#ifndef _MANAGE_GS_SESSION_COOLER_HPP
#define _MANAGE_GS_SESSION_COOLER_HPP

#include <list>
#include <ace/Task.h>
#include <ace/Guard_T.h>
#include <ace/Thread_Mutex.h>
#include <ace/Global_Macros.h>
#include "Singleton.h"
#include "GateLogger.h"
#include "GSSession.h"
#include "AppNotifyInterface.h"

class ManageGSSessionCooler : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageGSSessionCooler();
	~ManageGSSessionCooler();
public:
	static ManageGSSessionCooler * instance()
	{
		return Singleton<ManageGSSessionCooler>::instance();
	}
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	typedef std::list<GSSession *> GSSessionList;

	virtual int svc(void);

public:
	int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	int fini();

	/// wait for finish
	int wait();

	void putGSSession(GSSession * gs_session);
private:
	bool m_stop;

	bool m_wait;

	long m_cooler_time;

	ACE_Thread_Mutex m_gs_session_mutex;

	GSSessionList m_gs_session_list;

	GSSessionList m_gs_cleaned_session_list;
};

#endif