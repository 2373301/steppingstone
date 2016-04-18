
#ifndef CACHE_HPP
#define CACHE_HPP

#include "typedef.h"
#include "Session.h"
#include "Cache_def.h"
#include "Logger.h"
#include "Packet.h"

using namespace netstream;


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
	RemoteCfg() : type(0) {}

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
	~ManageCache() {}

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


struct CachePackInfo 
{
	CachePackInfo()
		: request_id(0)
		, map_id(0)
		, packet(NULL)
	{
	}

	CachePackInfo(uint64 req_id, uint64 mid, Packet * pack)
		: request_id(req_id)
		, map_id(mid)
		, packet(pack)
	{
	}

	uint64 request_id;
	uint64 map_id;
	Packet * packet;
};


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


class CACHE_EXOPRT Cache 
	: public Session
{
public:
	// remote: empty, local: local path
	virtual int init(const string & parameter, ISessionIn * handle_input)
	{
		if (NULL == handle_input)
		{
			return false;
		}

		Session::setHandleInput(handle_input);
		m_local_path = parameter;
		return initing();
	}

	virtual int finit() = 0;

protected:
	virtual int initing() = 0;

protected:
	string m_local_path;
};

#endif