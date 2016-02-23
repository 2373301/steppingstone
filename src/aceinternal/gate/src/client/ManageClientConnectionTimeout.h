#ifndef _MANAGE_CLIENT_CONNECTION_TIMEOUT_HPP
#define _MANAGE_CLIENT_CONNECTION_TIMEOUT_HPP

#include <deque>
#include <map>
#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Global_Macros.h>
#include "Singleton.h"
#include "AppNotifyInterface.h"
#include "ClientSession.h"

class ManageClientConnectionTimeout : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageClientConnectionTimeout();
	~ManageClientConnectionTimeout();
public:
	static ManageClientConnectionTimeout * instance()
	{
		return Singleton<ManageClientConnectionTimeout>::instance();
	}
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	typedef std::deque<ClientSession *> ClientSessionDeque;

	typedef std::set<ClientSession *> ClientSessionSet;

	virtual int svc(void);

public:
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	virtual int fini();

	/// wait for finish
	virtual int wait();
public:
	void putClientSession(ClientSession * client_session);

	void removeClientSession(ClientSession * client_session);
protected:
	void clientSessionTimeout(ClientSession * client_session);

	bool isRemovedSession(ClientSession * client_session, bool remove_session);

	void checkDeleteSession(ClientSessionSet & avilable_session_set);
private:
	bool m_stop;

	bool m_wait;

	ACE_Thread_Mutex  m_client_session_mutex;

	ClientSessionDeque m_client_session_deque;

	ClientSessionSet m_remove_client_session_set;

	ClientSessionSet m_remove_client_session_set_copy;

	ACE_Thread_Mutex m_remove_client_session_set_mutex;
};

#endif