#pragma once
#include "coredef.h"
#include "Session.h"
#include "Scene.h"

class scenex_ut
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

		Scene * s = createScene();
		SceneCfg cfg;
		s->init(cfg);


// 		netcore_ut ut;
// 		SessionPoolx* pool = SessionPoolFactory::createSessionPool();
// 		pool->init(1, 1, &ut, &ut, &ut);
// 		SessionAddrVec_t vec;
// 		vec.push_back("127.0.0.1:80");
// 		vec.push_back("127.0.0.1:80");
// 		pool->connect(vec);

		while (true)
		{
			ACE_OS::sleep(1);
		}
	}

};
