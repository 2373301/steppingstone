#pragma once
#include "../../netcore/inc/SessionPoolFactory.h"

namespace netcore
{
class netcore_ut
	: public HandleSessionOpenClosed
	, public HandleSessionRouterAddRemove
	, HandleInputx
{
public:
	static void run()
	{	

		ManageSingleton::instance();
		ManageLogger::instance();

		if (ManageLogger::instance()->init() == -1)
		{
			std::cout << "failed to init ManageLogger" << std::endl;
			return;
		}


		netcore_ut ut;
		SessionPoolx* pool = SessionPoolFactory::createSessionPool();
		pool->init(1, 1, &ut, &ut, &ut);
		SessionAddrVec_t vec;
		vec.push_back("127.0.0.1:80");
		vec.push_back("127.0.0.1:80");
		pool->connect(vec);
		
		while (true)
		{
			ACE_OS::sleep(1);
		}
	}

	virtual void sessionOpen(Sessionx * session) override
	{
		MAKE_NEW_PACKET(ps, 11, 22, "333");
		session->output(ps);
	}

	virtual void sessionClosed(Sessionx * session) override
	{

	}

	virtual void input(Packet * packet) override
	{

	}

	virtual void sessionRouterAdd(Packet * packet) override
	{

	}

	virtual void sessionRouterRemove(uint64 guid) override
	{

	}

	virtual Sessionx * getSession(Packet * packet) override
	{
		return (Sessionx *)packet->getOwner();
	}
};

}