
#ifndef SESSION_POOL_ASSISTENT_HPP
#define SESSION_POOL_ASSISTENT_HPP

#include <ace/Reactor.h>
#include "Sessionx.h"
#include "SessionPoolx.h"

namespace netcore
{

class CellSessionx;

class SessionPoolImp;

void notifySessionPool(CellSessionx * new_session, SessionStatex session_state);

void collectSessionPool(HandleSessionOpenClosed * session_pool, ACE_Reactor * reactor);

};

#endif