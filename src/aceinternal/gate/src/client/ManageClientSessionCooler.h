
#ifndef _MANAGE_CLIENT_SESSION_COOLER_HPP
#define _MANAGE_CLIENT_SESSION_COOLER_HPP

#include <list>
#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Global_Macros.h>
#include "Singleton.h"
#include "GateLogger.h"
#include "ClientSession.h"
#include "AppNotifyInterface.h"

class ManageClientSessionCooler : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageClientSessionCooler();
	~ManageClientSessionCooler();
public:
	static ManageClientSessionCooler * instance()
	{
		return Singleton<ManageClientSessionCooler>::instance();
	}

public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	typedef std::list<ClientSession *> ClientSessionList;

	virtual int svc(void);

public:
	int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	int fini();

	/// wait for finish
	int wait();

	void putClientSession(ClientSession * client_session);

protected:
	void putInCleanList(ClientSession * client_session);

	bool isInCleanList(ClientSession * client_session);

	void cleanClientSession(ClientSession * client_session);
private:
	bool m_stop;

	bool m_wait;

	long m_cooler_time;

	ACE_Thread_Mutex m_client_session_mutex;

	ClientSessionList m_client_session_list;

	ClientSessionList m_cleaned_client_session_list;
};

#endif