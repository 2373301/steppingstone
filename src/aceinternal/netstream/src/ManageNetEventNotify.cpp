
#include "CellSession.h"
#include "SessionPoolImp.h"
#include "ManageNetEventNotify.h"

namespace netstream
{

void ManageNetEventNotify::collectSessionPoolReactor(SessionPoolImp * session_pool, ACE_Reactor * reactor)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_pool_mutex, );
	m_session_pool_reactor_map.insert(std::make_pair(reactor, session_pool));
}

void ManageNetEventNotify::handleNewSession(CellSession * cell_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_pool_mutex, );
	SessionPoolReactorMap_t::iterator it = m_session_pool_reactor_map.find(cell_session->reactor());
	if (it != m_session_pool_reactor_map.end())
	{
		it->second->handleNewConnection(cell_session);
	}
	else
	{
		// never happen
	}
}

void ManageNetEventNotify::handleSessionClose(CellSession * cell_session, int trigger_source)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_pool_mutex, );
	SessionPoolReactorMap_t::iterator it = m_session_pool_reactor_map.find(cell_session->reactor());
	if (it != m_session_pool_reactor_map.end())
	{
		it->second->connectionClosed(cell_session, trigger_source);
	}
	else
	{
		// never happen
	}
}

}
