
#include "CacheFactory.h"
#include "ManageLocalCache.h"
#include "LocalCache.h"
#include "RemoteCache.h"
#include "ManageRemoteCache.h"

ManageCache * CacheFactory::createManageCache(int type_value)
{
	if (0 == type_value)
	{
		return new ManageLocalCache();
	}
	else
	{
		return new ManageRemoteCache();
	}
}
