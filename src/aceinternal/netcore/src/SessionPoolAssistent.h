
#ifndef SESSION_POOL_ASSISTENT_HPP
#define SESSION_POOL_ASSISTENT_HPP

#include <ace/Reactor.h>
#include "Session.h"
#include "SessionPool.h"

namespace netcore
{

class CellSession;

class SessionPoolImp;

void notifySessionPool(CellSession * new_session, SessionState session_state);

void collectSessionPool(HandleSessionOpenClosed * session_pool, ACE_Reactor * reactor);

};

#endif