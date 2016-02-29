#pragma once
#include "../../pool/inc/Pool.h"

class pool_ut
{
public:
	static void run()
	{	
		pool_ut ut;
		Pool * pool = createPool();
		PoolCfgx cfg;
		cfg.handle_output = std::bind(&pool_ut::output, &ut, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		pool->init(cfg);
		uint64 player_guid = 1;
		DECLARE_REQUEST_LIST(pool);
		LOAD_ONCE_REQUEST(1, 2);
		POOL_OP_COMMIT(pool, boost::bind(&pool_ut::loadPlayerFinish, &ut, _1, player_guid));
	}

	void loadPlayerFinish(RequestList * request_list, uint64 player_guid)
	{

	}

	void output(Packet * packet, uint64 map_id, uint64 request_id, uint64 owner_guid)
	{

	}

};