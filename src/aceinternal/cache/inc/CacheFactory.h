
#ifndef CACHE_FACTORY_HPP
#define CACHE_FACTORY_HPP

#include "Cache_def.h"

class ManageCache;
class CACHE_EXOPRT CacheFactory
{
public:
	static ManageCache * createManageCache(int type_value);
};

#endif