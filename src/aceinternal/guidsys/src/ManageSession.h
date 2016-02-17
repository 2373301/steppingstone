
#ifndef MANAGE_SESSION_HPP
#define MANAGE_SESSION_HPP

#include "SingletonInit.h"
#include "SessionPool.h"
#include "Packet.h"
#include "AllocGuid.h"
#include "Singleton.h"

class ManageSession 
	: public SingletonInit
	, public HandleInput
	, public netcore::HandleSessionOpenClosed
	, public netcore::HandleSessionRouterAddRemove
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
	virtual void sessionOpen(Session * session);

	virtual void sessionClosed(Session * session);

public:
	virtual void sessionRouterAdd(Packet * packet);

	virtual void sessionRouterRemove(uint64 guid);

	virtual Session * getSession(Packet * packet);

protected:
private:
	netcore::SessionPool * m_session_pool;

	AllocGuid m_alloc_guid;
};

#endif