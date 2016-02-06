
#include "SessionPoolFactory.h"
#include "SessionPoolImp.h"

namespace netcore
{

SessionPool * SessionPoolFactory::createSessionPool()
{
	return new SessionPoolImp();
}

}