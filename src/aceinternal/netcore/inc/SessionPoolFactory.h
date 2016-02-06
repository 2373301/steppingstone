
#ifndef SESSION_POOL_FACTORY_HPP
#define SESSION_POOL_FACTORY_HPP

#include "SessionPool.h"
#include "netcore_def.h"

namespace netcore
{

class NETCORE_EXOPRT SessionPoolFactory
{
public:
	static SessionPool * createSessionPool();
};

}

#endif