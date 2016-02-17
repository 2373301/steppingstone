
#include <ace/Thread_Mutex.h>
#include "typedef.h"
#include "SessionPoolAssistent.h"
#include "CellSession.h"
#include "SessionPoolImp.h"

namespace netcore
{

ACE_Thread_Mutex * session_pool_mutex = NULL;
map<ACE_Reactor *, HandleSessionOpenClosed *> session_pool_reactor_map;

void notifySessionPool(CellSession * cell_session, SessionState session_state)
{
	if (NULL == session_pool_mutex)
	{
		return ;
	}

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, *session_pool_mutex, );
	map<ACE_Reactor *, HandleSessionOpenClosed *>::iterator it = session_pool_reactor_map.find(cell_session->reactor());
	if (it != session_pool_reactor_map.end())
	{
		if (SS_CONNECTED == session_state)
		{
			it->second->sessionOpen(cell_session);
		}
		else if (SS_CLOSE == session_state)
		{
			it->second->sessionClosed(cell_session);
		}
	}
	else
	{
		// error
	}
}

// 线程绑定的reactor <-> session pool 
void collectSessionPool(HandleSessionOpenClosed * session_pool, ACE_Reactor * reactor)
{
	if (NULL == session_pool_mutex)
	{
		session_pool_mutex = new ACE_Thread_Mutex();
	}
	
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, *session_pool_mutex, );
	session_pool_reactor_map.insert(std::make_pair(reactor, session_pool));
}

};