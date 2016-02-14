
#ifndef MANAGE_CACHE_HPP
#define MANAGE_CACHE_HPP

#include "Cache.h"
#include "CacheInterface.h"

struct ReqPackInfo 
{
	ReqPackInfo()
		: request_id(0)
		, guid(0)
		, map_id(0)
	{}

	ReqPackInfo(uint64 req_id, uint64 gid, uint64 mid)
		: request_id(req_id)
		, guid(gid)
		, map_id(mid)
	{}

	uint64 request_id;
	uint64 guid;
	uint64 map_id;
};


#endif