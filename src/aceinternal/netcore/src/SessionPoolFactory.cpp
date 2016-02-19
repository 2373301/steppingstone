
#include "SessionPoolFactory.h"
#include "SessionPoolImp.h"

namespace netcore
{

SessionPoolx * SessionPoolFactory::createSessionPool()
{
	return new SessionPoolImp();
}

}