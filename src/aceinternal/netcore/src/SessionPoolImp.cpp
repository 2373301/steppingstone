
#include <ace/SOCK_Connector.h>
#include <ace/Dev_Poll_Reactor.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Select_Reactor.h>
#include "SessionPoolImp.h"
#include "Logger.h"
#include "SessionPoolAssistent.h"

namespace netcore
{

typedef ACE_Acceptor<CellSession, ACE_SOCK_ACCEPTOR> CellSessionAcceptor;

SessionPoolImp::SessionPoolImp()
: m_handle_input(NULL)
, m_handle_session_event(NULL)
, m_handle_session_router(NULL)
, m_reactor(NULL)
, m_stop(false)
, m_wait(true)
, m_save_pack_stream(false)
{
}

SessionPoolImp::~SessionPoolImp()
{
	for (CellSessionInfoMap_t::iterator it = m_cell_session_map.begin(); it != m_cell_session_map.end(); ++it)
	{
		delete it->first;
	}

	if (NULL != m_reactor)
	{
		delete m_reactor;
		m_reactor = NULL;
	}
}

int SessionPoolImp::svc()
{
	REPORT_THREAD_INFO();

	m_reactor->owner(ACE_OS::thr_self());

	int reactor_result = 0;

	CellSessionAcceptor session_acceptor;
	ACE_INET_Addr addr(m_listen_addr.c_str());
	if (session_acceptor.open(addr, m_reactor) == -1)
	{
		DEF_LOG_ERROR("failed to open the listen addr : <%s>\n", m_listen_addr.c_str());
		return 1;
	}
	else
	{
		DEF_LOG_INFO("success to listen at addr : <%s>\n", m_listen_addr.c_str());
	}

	while (true)
	{
		reactor_result = m_reactor->run_reactor_event_loop();
#ifdef WIN32
		break;
#else
		int last_error = ACE_OS::last_error();
		if ((last_error == EWOULDBLOCK) || (last_error == EINTR) || (last_error == EAGAIN))
		{
			continue;
		}
		else
		{
			break;
		}
#endif
	}

	m_wait = false;
	return 0;
}

void SessionPoolImp::input(Packet * packet)
{
	if (NULL != m_handle_session_router)
	{
		m_handle_session_router->sessionRouterAdd(packet);
	}

	if (NULL != m_handle_input)
	{
		m_handle_input->input(packet);
	}
}

int SessionPoolImp::init(int input_thr_no, int output_thr_no, HandleInput * handle_input, HandleSessionOpenClosed * handle_session_event, HandleSessionRouterAddRemove * handle_session_router)
{
#ifdef WIN32
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	m_reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(100000, true));
#endif

	collectSessionPool(this, m_reactor);

	if (m_input_session_pool.init(input_thr_no, this, this) == -1)
	{
		DEF_LOG_ERROR("failed to init input session pool, thread no is <%d>, last error is <%d>\n", input_thr_no, ACE_OS::last_error());
		return -1;
	}

	if (m_output_session_pool.init(output_thr_no, this) == -1)
	{
		DEF_LOG_ERROR("failed to init output session pool, thread no is <%d>, last error is <%d>\n", output_thr_no, ACE_OS::last_error());
		return -1;
	}

	m_handle_input = handle_input;

	m_handle_session_event = handle_session_event;

	m_handle_session_router = handle_session_router;

	return 0;
}

bool SessionPoolImp::connect(const SessionAddrVec_t & session_addr_vec)
{
	ACE_SOCK_Connector connector;
	ACE_INET_Addr addr;
	for (SessionAddrVec_t::const_iterator it = session_addr_vec.begin(); it != session_addr_vec.end(); ++it)
	{
		addr.set(it->c_str());
		CellSession * cell_session = new CellSession();

		if (m_save_pack_stream)
		{
			cell_session->setSavePackInfo(true, it->c_str());
		}

		if (connector.connect(cell_session->peer(), addr) != -1)
		{
			if (cell_session->open() == -1)
			{
				DEF_LOG_ERROR("failed to call open of cell session, last error is <%d>\n", ACE_OS::last_error());
				return false;
			}

			CellSessionInfo csi;
			csi.reference_no = 2;
			m_cell_session_map.insert(std::make_pair(cell_session, csi));

			if (NULL != m_handle_session_event)
			{
				m_handle_session_event->sessionOpen(cell_session);
			}

			cell_session->setHandleInput(this);

			m_input_session_pool.handleSession(cell_session);

			m_output_session_pool.handleSession(cell_session);

			DEF_LOG_INFO("success to connect the addr <%s>\n", it->c_str());
		}
		else
		{
			DEF_LOG_ERROR("failed to connect the addr : <%s>, last error is <%d>\n", it->c_str(), ACE_OS::last_error());
			return false;
		}

	}
	return true;
}

bool SessionPoolImp::listen(const string & listen_addr)
{

	m_listen_addr = listen_addr;

	if (this->activate() == -1)
	{
		DEF_LOG_ERROR("failed to active SessionPoolImp, last error is <%d>\n", ACE_OS::last_error());
		return false;
	}

	return true;
}

void SessionPoolImp::setHandleInput(HandleInput * handle_input)
{
	m_handle_input = handle_input;
}

void SessionPoolImp::setHandleSessionEvent(HandleSessionOpenClosed * handle_event)
{
	m_handle_session_event = handle_event;
}

void SessionPoolImp::setHandleSessionRouter(HandleSessionRouterAddRemove * handle_session_router)
{
	m_handle_session_router = handle_session_router;
}

void SessionPoolImp::output(Packet * packet)
{
	if (NULL != m_handle_session_router)
	{
		Session * session = NULL;
		if (NULL != m_handle_session_router)
			session = m_handle_session_router->getSession(packet);

		//if (packet->getOwner() != NULL)
		//{
		//	session = (Session *)packet->getOwner();
		//}
		//else
		//{
		//	if (NULL != m_handle_session_router)
		//		session = m_handle_session_router->getSession(packet);
		//}

		if (NULL != session)
		{
			session->output(packet);
		}
		else
		{
			DEF_LOG_ERROR("failed to get session by session router, packet guid is <%s>\n", boost::lexical_cast<string>(packet->guid()).c_str());
		}
	}
	else
	{
		DEF_LOG_ERROR("occur error, the handle session router is NULL, last error is <%d>\n", ACE_OS::last_error());
	}
}

void SessionPoolImp::stop()
{
	if (NULL != m_reactor)
	{
		m_reactor->end_reactor_event_loop();
	}
	else
	{
		m_wait = true;
	}

	m_input_session_pool.stop();

	m_output_session_pool.stop();
}

void SessionPoolImp::finit()
{
	m_input_session_pool.finit();

	m_output_session_pool.finit();

	while (m_wait)
	{
		//std::cout << "wait SessionPoolImp" << std::endl;
		ACE_OS::sleep(1);
	}

	//std::cout << "exit SessionPoolImp" << std::endl;
}

void SessionPoolImp::sessionOpen(Session * session)
{
	if (NULL != m_handle_session_event)
	{
		m_handle_session_event->sessionOpen(session);
	}

	if (session->reactor() != NULL)
	{
		session->reactor()->remove_handler(session, ACE_Event_Handler::READ_MASK);
	}

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cell_session_map_mutex, );
		CellSessionInfo cell_si;
		cell_si.reference_no = 2;
		m_cell_session_map.insert(CellSessionInfoMap_t::value_type((CellSession *)session, cell_si));
	}

	session->setHandleInput(this);
	m_input_session_pool.handleSession((CellSession *)session);
	m_output_session_pool.handleSession((CellSession *)session);
}

void SessionPoolImp::sessionClosed(Session * session)
{
	removeSession(session);
	if (NULL != m_handle_session_event)
	{
		m_handle_session_event->sessionClosed(session);
	}

	m_output_session_pool.removeSession((CellSession *)session);
}

void SessionPoolImp::removeSession(Session * session)
{
	CellSession * cell_session = (CellSession *)session;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cell_session_map_mutex, );
	CellSessionInfoMap_t::iterator it = m_cell_session_map.find(cell_session);
	if (it != m_cell_session_map.end())
	{
		--(it->second.reference_no);
		if (0 == it->second.reference_no)
		{
			m_handle_session_router->sessionRouterRemove(cell_session->getGUID());
			cell_session->peer().close();
			delete cell_session;
			m_cell_session_map.erase(cell_session);
		}
	}
}

void SessionPoolImp::savePackStream()
{
	m_save_pack_stream = true;
}

}