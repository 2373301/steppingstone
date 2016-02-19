
#ifndef CACHE_FACTORY_HPP
#define CACHE_FACTORY_HPP

#include "CacheInterface.h"
#include "Cache_def.h"

class CACHE_EXOPRT CacheFactory
{
public:
	static ManageCache * createManageCache(int type_value);
};

#endif