
#ifndef _MANAGE_CLIENT_BEAT_HPP
#define _MANAGE_CLIENT_BEAT_HPP

#include <list>
#include <map>
#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Global_Macros.h>
#include "Singleton.h"
#include "AppNotifyInterface.h"
#include "ClientSession.h"

class ManageClientBeat : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageClientBeat();
	~ManageClientBeat();
public:
	static ManageClientBeat * instance()
	{
		return Singleton<ManageClientBeat>::instance();
	}
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	typedef std::list<ClientSession *> ClientSessionList;

	virtual int svc(void);

public:
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	virtual int fini();

	/// wait for finish
	virtual int wait();

	void addClient(ClientSession * client_session);

	void removeClient(ClientSession * client_session);

	void getOnlineClient(ClientSessionList & online_client_list);

private:
	bool lastBeatTimeout(const ACE_Time_Value & current_time, ClientSession * client_session);

private:
	bool m_stop;

	bool m_wait;

	int  m_check_interval;

	int  m_max_lost_beat;

	int  m_beat_interval;

	int  m_max_timeout_second;

	ACE_Thread_Mutex m_beat_mutex;

	ACE_Thread_Mutex m_beat_put_mutex;

	ClientSessionList m_client_session_list;

	ClientSessionList m_client_session_list_put;

	ClientSessionList m_client_session_list_remove;

	ACE_Thread_Mutex m_client_session_copy_mutex;

	ClientSessionList m_client_session_list_copy;
};

#endif