
#include "SessionPoolFactory.h"
#include "SessionPoolImp.h"

namespace netstream
{

SessionPool * SessionPoolFactory::createSessionPool()
{
	return new SessionPoolImp();
}

}
