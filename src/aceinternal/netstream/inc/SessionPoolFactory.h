
#ifndef SESSION_POOL_FACTORY_HPP
#define SESSION_POOL_FACTORY_HPP

#include "SessionPool.h"
#include "netstream_def.h"

namespace netstream
{

class NETSTREAM_EXOPRT SessionPoolFactory
{
public:
	static ISessionPool * createSessionPool();
};

}

#endif