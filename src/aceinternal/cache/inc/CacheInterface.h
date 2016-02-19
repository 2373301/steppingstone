
#ifndef CACHE_INTERFACE_HPP
#define CACHE_INTERFACE_HPP

#include "Cache_def.h"
#include "Packet.h"
#include "typedef.h"

class CACHE_EXOPRT CacheHandleInput
{
public:
	virtual void cacheInput(Packet * packet, uint64 map_id, uint64 request_id) = 0;
};

class CACHE_EXOPRT CacheHandleOutput
{
public:
	virtual void cacheOutput(Packet * packet, uint64 map_id, uint64 request_id, uint64 owner_guid) = 0;
};

struct RemoteCfg 
{
	RemoteCfg(): type(0) {}

	int type;

	typedef vector<string> CacheVec_t;
	CacheVec_t cache_addrs;
};


struct CACHE_EXOPRT CacheCfg 
{
	CacheCfg()
		: handle_input(NULL)
		, network_thread_no(0)
	{}

	CacheHandleInput * handle_input;
	int network_thread_no;
	string local_path;
	RemoteCfg remote_cfg;
};

class CACHE_EXOPRT ManageCache
	: public CacheHandleOutput
{
public:
	~ManageCache(){}

	virtual int init(const CacheCfg & cache_cfg)
	{
		m_cache_cfg = cache_cfg;
		return initing();
	}

	virtual int stop() = 0;

	virtual int finit() = 0;

protected:
	virtual int initing() = 0;


protected:
	CacheCfg m_cache_cfg;
};

#endif