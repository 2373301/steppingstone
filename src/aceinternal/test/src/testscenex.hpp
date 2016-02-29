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

		scenex_ut ut;
		Scene * s = createScene();
		SceneCfg cfg;
		cfg.naming_addr = argv[1];
		cfg.listen_addr = argv[2];
		cfg.srv_type = argv[3];
		cfg.srv_id = argv[4];
		cfg.cache_handle_output = std::bind(&scenex_ut::output, &ut, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		s->init(cfg);
		s->startup();

		while (true)
		{
			ACE_OS::sleep(1);
		}
	}

	void output(Packet * packet, uint64 map_id, uint64 request_id, uint64 owner_guid)
	{

	}

};
