
#ifndef MANAGE_SESSION_POOL_HPP
#define MANAGE_SESSION_POOL_HPP

#include "SessionPool.h"
#include "SessionPoolFactory.h"
#include "EntityHander.h"
#include "SingletonInit.h"
#include "Singleton.h"

namespace cached {
	namespace service {

class ManageSessionPool	
	: public HandleInput
	, public HandleOutput
	, public netcore::HandleSessionOpenClosed
	, public netcore::HandleSessionRouterAddRemove
	, public SingletonInit
{
public:
	ManageSessionPool();
	~ManageSessionPool();
public:
	static ManageSessionPool * instance()
	{
		return Singleton<ManageSessionPool>::instance();
	}
public:
	int init();

	int stop();

	int finit();
public:
	//
	virtual void input(Packet * packet);

	virtual void output(Packet * packet);
	//
	virtual void sessionOpen(Session * session);

	virtual void sessionClosed(Session * session);

	//
	virtual void sessionRouterAdd(Packet * packet);

	virtual void sessionRouterRemove(uint64 guid);

	virtual Session * getSession(Packet * packet);

protected:
private:
	typedef define_unordered_set<Session *>	SessionSet_t;

	netcore::SessionPool * m_session_pool;

	SessionSet_t m_session_set;
};

};

};




#endif