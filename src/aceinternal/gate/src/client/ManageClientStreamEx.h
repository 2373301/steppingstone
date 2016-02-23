
#ifndef MANAGE_CLIENT_STREAMEX_HPP
#define MANAGE_CLIENT_STREAMEX_HPP

#include <map>
#include <vector>
#include <ace/Task.h>
#include <ace/Reactor.h>
#include "GateLogger.h"
#include "Singleton.h"
#include "AppNotifyInterface.h"
#include "ClientSession.h"


class ManageClientStreamEx : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageClientStreamEx();
	~ManageClientStreamEx();

	typedef ACE_Task<ACE_NULL_SYNCH> super;

	typedef std::vector<ACE_Reactor *>	ReactorList;
public:
	static ManageClientStreamEx * instance()
	{
		return Singleton<ManageClientStreamEx>::instance();
	}
public:
	/// ACE_Task_Base svc
	virtual int svc(void);

public:
	/// AppNotifyInterface init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// AppNotifyInterface fini, stop
	virtual int fini();

	/// AppNotifyInterface wait for finish
	virtual int wait();

	void registerReactor(ACE_Reactor * reactor);

	void unregistorReactor(ACE_Reactor * reactor);

	bool handleClientStream(ClientSession * client_stream);

protected:
private:
	bool m_stop;

	bool m_wait;

	int  m_reactor_index;

	ReactorList	m_reactor_list;

	ACE_Thread_Mutex m_reactor_mutex;
};

#endif