
#include "SessionPoolFactory.h"
#include "SessionPoolImp.h"

namespace netstream
{

ISessionPool * SessionPoolFactory::createSessionPool()
{
	return new SessionPoolImp();
}

}
