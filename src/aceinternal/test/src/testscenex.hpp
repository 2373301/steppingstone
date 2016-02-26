#pragma once
#include "coredef.h"
#include "Session.h"
#include "Scene.h"

class scenex_ut
{
public:
	static void run(int argc, char * argv[])
	{

		ManageSingleton::instance();
		ManageLogger::instance();

		if (ManageLogger::instance()->init() == -1)
		{
			std::cout << "failed to init ManageLogger" << std::endl;
			return;
		}
		
		if (5 != argc)
		{
			std::cout << "参数不够啊 要  nsip, listenip, type, name" << std::endl;
			return;
		}

		Scene * s = createScene();
		SceneCfg cfg;
		cfg.naming_addr = argv[1];
		cfg.listen_addr = argv[2];
		cfg.srv_type = argv[3];
		cfg.srv_id = argv[4];
		s->init(cfg);
		s->startup();


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
