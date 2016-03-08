#pragma once
#include "../../pool/inc/Pool.h"
#include "CacheFactory.h"
#include "Cache.h"
#include "LoggerFactory.h"

ManageCache* g_cache = NULL;
Pool * g_pool = NULL;
class pool_ut
	:public CacheHandleInput
{
public:
	static void run()
	{	
		pool_ut ut;
		g_pool = createPool();
		PoolCfgx cfg;
		cfg.logger = LoggerFactory::createFileLogger("test.log");
		cfg.handle_output = std::bind(&pool_ut::output, &ut, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		g_pool->init(cfg);

		g_cache = CacheFactory::createManageCache(1);
		CacheCfg cache_cfg;
		cache_cfg.handle_input = &ut;
		cache_cfg.network_thread_no = 1;
		cache_cfg.local_path = "d:/temp";
		cache_cfg.remote_cfg.cache_addrs.push_back("127.0.0.1:13000");
		cache_cfg.remote_cfg.type = 1;
		if (-1 == g_cache->init(cache_cfg))
		{
			printf("failed to init cache !\n");
			g_cache->stop();
			g_cache->finit();
			
			ManageSingleton::instance()->finit();
			return;
		}

		ACE_OS::sleep(5);
		uint64 player_guid = 1;
		DECLARE_REQUEST_LIST(g_pool);
		LOAD_ONCE_REQUEST(1, 2);
		POOL_OP_COMMIT(g_pool, boost::bind(&pool_ut::loadPlayerFinish, &ut, _1, player_guid));

		while (true)
		{
			ACE_OS::sleep(1);
		}
	}

	void loadPlayerFinish(RequestList * request_list, uint64 player_guid)
	{

	}

	void output(Packet * packet, uint64 map_id, uint64 request_id, uint64 owner_guid)
	{
		g_cache->cacheOutput(packet, map_id, request_id, owner_guid);
	}

	virtual void cacheInput(Packet * packet, uint64 map_id, uint64 request_id) override
	{
		g_pool->cacheInput(packet, map_id, request_id);
	}

};