
#ifndef CACHE_HPP
#define CACHE_HPP

#include "typedef.h"
#include "Session.h"
#include "Cache_def.h"
#include "Logger.h"
#include "Packet.h"
#include "CacheInterface.h"

using namespace netstream;

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

class Scene;

struct PoolCfg 
{
	PoolCfg()
		: handle_output(NULL)
		, map_id(0)
		, logger(NULL)
		, line_id(0)
		, scene(NULL)
	{}

	CacheHandleOutput * handle_output;
	uint64				map_id;
	uint32				line_id;
	Logger *			logger;
	Scene *				scene;
};


class CACHE_EXOPRT Cache : public Session
{
public:
	virtual int init(const string & parameter, HandleInputStream * handle_input)
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