
#include <ace/SOCK_Connector.h>
#include <ace/Dev_Poll_Reactor.h>
#include <ace/Global_Macros.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Select_Reactor.h>
#include "SessionPoolImp.h"
#include "Logger.h"
#include "ManageNetEventNotify.h"

namespace netstream
{

typedef ACE_Acceptor<CellSession, ACE_SOCK_ACCEPTOR> CellSessionAcceptor;

SessionPoolImp::SessionPoolImp()
: m_handle_session_event(NULL)
, m_reactor(NULL)
, m_listen_status(0)
, m_socket_intput_buffer_size(0)
, m_socket_output_buffer_size(0)
{
}

SessionPoolImp::~SessionPoolImp()
{
	for (SessionInfoMap_t::iterator it = m_session_info_map.begin();
		it != m_session_info_map.end(); ++it)
	{
		delete it->first;
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
		DEF_LOG_ERROR("failed to open the listen addr : <%s>, last error is <%d>\n", m_listen_addr.c_str(), ACE_OS::last_error());
		m_listen_status = 2;
		return 1;
	}

	m_listen_status = 1;
	
	DEF_LOG_INFO("success to listen at addr : <%s>\n", m_listen_addr.c_str());

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

	return 0;
}

void SessionPoolImp::input(Session * session, ACE_Message_Block & msg_block)
{
	m_handle_session_event->handleInputStream(session, msg_block);
}

int SessionPoolImp::init(int input_thr_no, int output_thr_no, HandleSessionEvent * handle_session_event)
{
	m_handle_session_event = handle_session_event;

	if (m_input_session_pool.init(input_thr_no, this) != 0)
	{
		DEF_LOG_ERROR("failed to init input session, input thread no is <%d>\n", input_thr_no);
		return -1;
	}

	if (m_output_session_pool.init(output_thr_no, this) != 0)
	{
		DEF_LOG_ERROR("failed to init output session, output thread no is <%d>\n", output_thr_no);
		return -1;
	}

	return 0;
}

void SessionPoolImp::setSocketBufferSize(int input_buf_size, int output_buf_size)
{
	m_socket_intput_buffer_size = input_buf_size;
	m_socket_output_buffer_size = output_buf_size;
}

bool SessionPoolImp::connect(const SessionAddrVec_t & session_addr_vec)
{
	ACE_SOCK_Connector connector;
	ACE_INET_Addr addr;
	for (SessionAddrVec_t::const_iterator it = session_addr_vec.begin(); it != session_addr_vec.end(); ++it)
	{
		addr.set(it->c_str());
		CellSession * cell_session = new CellSession(true);

		if (connector.connect(cell_session->peer(), addr) != -1)
		{
			if (cell_session->open() == -1)
			{
				DEF_LOG_ERROR("failed to call open of cell session, last error is <%d>\n", ACE_OS::last_error());
				return false;
			}

			if (m_socket_intput_buffer_size > 0)
			{
				cell_session->setSocketBufferSize(m_socket_intput_buffer_size, m_socket_output_buffer_size);
			}

			cell_session->setHandleInput(this);

			m_input_session_pool.handleSession(cell_session);

			m_output_session_pool.handleSession(cell_session);


			DEF_LOG_INFO("success to connect the addr <%s>\n", it->c_str());
			if (NULL != m_handle_session_event)
			{
				m_handle_session_event->newConnection(cell_session, cell_session->isClientSide());
			}
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
#ifdef WIN32
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	m_reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(100000, true));
#endif

	ManageNetEventNotify::instance()->collectSessionPoolReactor(this, m_reactor);

	m_listen_addr = listen_addr;

	if (this->activate() == -1)
	{
		DEF_LOG_ERROR("failed to active SessionPoolImp, last error is <%d>\n", ACE_OS::last_error());
		return false;
	}

	while (0 == m_listen_status)
	{
		ACE_OS::sleep(1);
	}

	return 1 == m_listen_status;
}

bool SessionPoolImp::handleOutputStream(Session_t session, char * buffer, int buff_size)
{
	return m_output_session_pool.handleOutputStream(session, buffer, buff_size);
}

void SessionPoolImp::stop()
{

}

void SessionPoolImp::finit()
{

}

void SessionPoolImp::removeSession(Session_t session)
{
	// todo
	//m_input_session_pool.removeSession((CellSession *)session);
	m_output_session_pool.removeSession((CellSession *)session);
}

void SessionPoolImp::onSessionOpenNotify(Session * session)
{
	if (NULL != m_handle_session_event)
	{
		m_handle_session_event->newConnection(session, session->isClientSide());
	}

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_info_map_mutex, );
		m_session_info_map.insert(std::make_pair(session, SessionStatus()));
	}

	if (m_socket_intput_buffer_size > 0)
	{
		session->setSocketBufferSize(m_socket_intput_buffer_size, m_socket_output_buffer_size);
	}

	session->reactor()->remove_handler(session, ACE_Event_Handler::ALL_EVENTS_MASK);

	session->reactor(NULL);

	session->setHandleInput(this);

	m_input_session_pool.handleSession((CellSession *)session);

	m_output_session_pool.handleSession((CellSession *)session);
}

void SessionPoolImp::onSessionCloseNotify(Session * session, int trigger_source)
{
	int ref_no = 2;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_info_map_mutex, );
		SessionInfoMap_t::iterator it = m_session_info_map.find(session);
		if (m_session_info_map.end() == it)
		{
			return ;
		}

		SessionStatus & session_status = it->second;
		if (ACE_Event_Handler::READ_MASK == trigger_source)
		{
			if (session_status.closed_read)
			{
				return ;
			}

			session_status.closed_read = true;
		}

		if (ACE_Event_Handler::WRITE_MASK == trigger_source)
		{
			if (session_status.closed_write)
			{
				return ;
			}

			session_status.closed_write = true;
		}

		session_status.ref_no -= 1;
		ref_no = session_status.ref_no;
	}

	if (1 == ref_no)
	{
		m_handle_session_event->connectionClosed(session, trigger_source);

		if (ACE_Event_Handler::READ_MASK == trigger_source)
		{
			m_output_session_pool.removeSession((CellSession *)session);
		}
		else if (ACE_Event_Handler::WRITE_MASK == trigger_source)
		{
			m_input_session_pool.removeSession((CellSession *)session);
		}
	}
	else if (0 == ref_no)
	{
		if (session->reactor() != NULL)
		{
			session->reactor()->remove_handler(this, ACE_Event_Handler::ALL_EVENTS_MASK);
			session->reactor(NULL);
		}

		session->peer().close();
		delete session;

		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_info_map_mutex, );
		SessionInfoMap_t::iterator it = m_session_info_map.find(session);
		if (it != m_session_info_map.end())
		{
			m_session_info_map.erase(it);
		}
	}
}

}
