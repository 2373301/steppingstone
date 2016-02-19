
#ifndef MANAGE_SESSION_HPP
#define MANAGE_SESSION_HPP

#include "SingletonInit.h"
#include "SessionPoolx.h"
#include "Packet.h"
#include "AllocGuid.h"
#include "Singleton.h"

class ManageSession 
	: public SingletonInit
	, public HandleInputx
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
	virtual void input(Packet * packet) override;

public:
	virtual void sessionOpen(Sessionx * session);

	virtual void sessionClosed(Sessionx * session);

public:
	virtual void sessionRouterAdd(Packet * packet);

	virtual void sessionRouterRemove(uint64 guid);

	virtual Sessionx * getSession(Packet * packet);


private:
	netcore::SessionPoolx * m_session_pool;

	AllocGuid m_alloc_guid;
};

#endif