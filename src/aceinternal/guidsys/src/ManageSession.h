
#ifndef MANAGE_SESSION_HPP
#define MANAGE_SESSION_HPP

#include "SingletonInit.h"
#include "SessionPool.h"
#include "Packet.h"
#include "AllocGuid.h"
#include "Singleton.h"

class ManageSession : public SingletonInit, public HandleInput, public netcore::HandleSessionEvent, public netcore::HandleSessionRouter
{
public:
	ManageSession();

	~ManageSession();
public:
	static ManageSession * instance()
	{
		return Singleton<ManageSession>::instance();
	}
public:
	virtual int init();

	virtual int stop();

	virtual int finit();
public:
	virtual void input(Packet * packet);

public:
	virtual void newConnection(Session * session);

	virtual void connectionClosed(Session * session);

public:
	virtual void addRoute(Packet * packet);

	virtual void removeRoute(uint64 guid);

	virtual Session * getSession(Packet * packet);

protected:
private:
	netcore::SessionPool * m_session_pool;

	AllocGuid m_alloc_guid;
};

#endif