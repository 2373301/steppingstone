
#include <ace/OS.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include <boost/lexical_cast.hpp>
#include "ManageSingleTerminal.h"
#include "Logger.h"
#include "opcode.h"

typedef ACE_Acceptor<SingleTerminal, ACE_SOCK_ACCEPTOR> SingleTerminalAcceptor;

ManageSingleTerminal * ManageSingleTerminal::m_static_instance = NULL;

ManageSingleTerminal::ManageSingleTerminal()
: m_reactor(NULL)
, m_max_epoll_socket(100000)
, m_timer_id(0)
{
}

int ManageSingleTerminal::stop()
{
	m_stop = true;
	return 0;
}

int ManageSingleTerminal::finit()
{
	m_finit = true;
	return 0;
}

int ManageSingleTerminal::svc()
{
	REPORT_THREAD_INFO();

#ifdef WIN32
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	m_reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(m_max_epoll_socket), true);
#endif

	while (!m_stop)
	{
		SingleTerminalAcceptor single_terminal_acceptor(m_reactor);
		ACE_INET_Addr addr(m_terminal_cfg.local_addr.local_port, m_terminal_cfg.local_addr.local_ip.c_str());
		if (single_terminal_acceptor.open(addr, m_reactor) == -1)
		{
			DEF_LOG_ERROR("failed to open single terminal acceptor, addr is <%s>, port is <%d>, last error is <%d>", m_terminal_cfg.local_addr.local_ip.c_str(), m_terminal_cfg.local_addr.local_port, ACE_OS::last_error());
			return 1;
		}

		ACE_Time_Value tv(3, 0);
		m_timer_id = m_reactor->schedule_timer(this, NULL, tv, tv);
		if (-1 == m_timer_id)
		{
			DEF_LOG_ERROR("failed to schedule timer in ManageSingleTerminal::svc\n");
			return 1;
		}

		int reactor_result = 0;
		while (true)
		{
			reactor_result = m_reactor->run_reactor_event_loop();
#ifdef WIN32
			break;
#else
			if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
			{
				continue;
			}
			else
			{
				break;
			}
#endif
		}

	}
	return 0;
}

int ManageSingleTerminal::handle_timeout(const ACE_Time_Value &current_time, const void *act)
{
	ACE_Time_Value diff_time;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_deleting_single_terminal_vec_mutex, 0);
	for (SingleTerminalVec_t::iterator it = m_deleting_single_terminal_vec.begin(); it != m_deleting_single_terminal_vec.end(); )
	{
		diff_time = current_time - (*it)->getCloseTime();
		if (diff_time.sec() >= 20)
		{
			delete *it;
			it = m_deleting_single_terminal_vec.erase(it);
		}
		else
		{
			++it;
		}
	}

	return 0;
}

void ManageSingleTerminal::ISessionIn_sync_read(Session * session, ACE_Message_Block & msg_block)
{
	//if (packet->opcode() == CMSG_PLAYER_ENTER_MAP)
	//{
	//	//只负责建立路由，不负责错误情况的处理，比如多次建立路由等，这类错误由游戏逻辑处理
	//	addRoute(packet->guid(), packet->getOwner());
	//}
	////else if (packet->opcode() == CMSG_PLAYER_LEAVE_WORLD)
	////{
	////	removeRoute(packet->guid());
	////}

	//handle_input_->input(packet);
}

void ManageSingleTerminal::output(Packet * packet)
{
	Head head(packet->opcode(), packet->body_size(), packet->guid());

	int op_code = packet->opcode();
	uint64 player_guid = packet->guid();
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_single_terminal_map_mutex, );
		SingleTerminalMap_t::iterator it = m_single_terminal_map.find(packet->guid());
		if (m_single_terminal_map.end() != it)
		{
			// todo
			//it->second->output(packet);
		}
		else
		{
			// fatal error
			DEF_LOG_ERROR("failed to get single terminal where dispatch packet, guid is <%s>\n", boost::lexical_cast<string>(packet->guid()).c_str());
			delete packet;
		}
	}

	if (head.opcode == CMSG_PLAYER_LEAVE_MAP)
	{
		removeRoute(head.guid);
	}
}

void ManageSingleTerminal::registerScene(uint64 scene_id, uint64 scend_type)
{
	// do nothing
}

void ManageSingleTerminal::registerSingleTerminal(SingleTerminal * single_terminal)
{
	single_terminal->init("", this);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_single_terminal_mutex, );
	m_single_terminal_vec.push_back(single_terminal);
}

void ManageSingleTerminal::removeSingleTerminal(SingleTerminal * single_terminal)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_single_terminal_mutex, );
	SingleTerminalVec_t::iterator it = std::find(m_single_terminal_vec.begin(), m_single_terminal_vec.end(), single_terminal);
	if (m_single_terminal_vec.end() != it)
	{
		/// todo
		// delay to delete the single terminal
		//delete *it;
		m_single_terminal_vec.erase(it);
	}
}

int ManageSingleTerminal::initing()
{
	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

bool ManageSingleTerminal::addRoute(uint64 guid, void * terminal)
{
	bool result = false;
	Terminal * ter = reinterpret_cast<SingleTerminal *>(terminal);
	SingleTerminal * sin_ter = dynamic_cast<SingleTerminal *>(ter);
	if (NULL != sin_ter)
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_single_terminal_map_mutex, false);
		SingleTerminalMap_t::iterator it = m_single_terminal_map.find(guid);
		if (m_single_terminal_map.end() == it)
		{
			m_single_terminal_map.insert(std::make_pair(guid, sin_ter));
			result = true;
		}
		else
		{
			// fatal error, notify the player, then kick the player
			// todo kick the player
			DEF_LOG_ERROR("failed to get the player login again : <%s>\n", boost::lexical_cast<string>(guid).c_str());
		}
	}

	return result;
}

void ManageSingleTerminal::removeRoute(uint64 guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_single_terminal_map_mutex, );
	SingleTerminalMap_t::iterator it = m_single_terminal_map.find(guid);
	if (m_single_terminal_map.end() != it)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_deleting_single_terminal_vec_mutex, );
			m_deleting_single_terminal_vec.push_back(it->second);
		}
		m_single_terminal_map.erase(it);
	}
	else
	{
		DEF_LOG_ERROR("failed to find the single terminal while remove it\n");
	}
}

