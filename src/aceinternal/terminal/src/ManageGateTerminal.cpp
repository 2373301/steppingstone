
#include <ace/SOCK_Connector.h>
#include <ace/Dev_Poll_Reactor.h>
#include <ace/Select_Reactor.h>
#include <boost/lexical_cast.hpp>
#include "opcode.h"
#include "GateTerminal.h"
#include "ManageGateTerminal.h"
#include "Logger.h"
#include <protocol/msg_gate.pb.h>

ManageGateTerminal::ManageGateTerminal()
: m_lost_gate_connection(false)
, m_player_route(this)
, m_is_stop(false)
{

}

ManageGateTerminal::~ManageGateTerminal()
{
	for (TerminalVec_t::iterator it = m_terminal_vec.begin(); it != m_terminal_vec.end(); ++it)
	{
		delete *it;
	}

	for (ReactorVec_t::iterator it = m_reactor_vec.begin(); it != m_reactor_vec.end(); ++it)
	{
		delete *it;
	}
}

int ManageGateTerminal::svc()
{
	REPORT_THREAD_INFO();

	ACE_Reactor * reactor = NULL;
#ifdef WIN32
	reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(), true);
#endif // WIN32

	registerReactor(reactor);

	int reactor_result = 0;	

	while (!m_is_stop)
	{
		reactor_result = reactor->run_reactor_event_loop();
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

void ManageGateTerminal::ISessionIn_sync_read(Session * session, ACE_Message_Block & msg_block)
{
	PacketVec_t packet_vec;
	parsePacketFromStream(session, msg_block, packet_vec);
	for (PacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
	{
		Packet * packet = *it;

		if (m_lost_gate_connection)
		{
			delete packet;
			return ;
		}

		// build terminal route
		if (packet->opcode() == CMSG_PLAYER_ENTER_MAP)
		{
			//只负责建立路由，不负责错误情况的处理，比如多次建立路由等，这类错误由游戏逻辑处理
			addRoute(packet->guid(), packet->getOwner());
		}
		else if (packet->opcode() == SMSG_LOST_GATE_CONNECTION)
		{
			DEF_LOG_DEBUG("recv gate lost connection msg in ManageGateTerminal::input\n");
			lostGateConnection();
		}
		//else if (packet->opcode() == CMSG_PLAYER_LEAVE_WORLD)
		//{
		//	removeRoute(packet->guid(), packet->getOwner());
		//}

		m_terminal_cfg.handle_input->IInput_input(packet);
	}
}

void ManageGateTerminal::IOutput_output(Packet * packet)
{
	m_player_route.input(packet);
}

void ManageGateTerminal::registerScene(uint64 scene_id, uint64 scene_type)
{
	typed::protocol::gcmsg_register reg;
	reg.set_zone_id(scene_id);
	reg.set_zone_type((::typed::protocol::gcmsg_register_zone_t)scene_type);

	Packet * ps = NULL;
	for (TerminalVec_t::iterator it = m_terminal_vec.begin(); it != m_terminal_vec.end(); ++it)
	{
		MAKE_NEW_PACKET_NO_DEC(ps, GCMSG_REGISTER, scene_id, reg.SerializeAsString());
		(*it)->IOutput_output(ps);
	}
}

Terminal * ManageGateTerminal::getTerminal(uint64 guid)
{
	Terminal * ter = NULL;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_terminal_map_mutex, NULL);
	TerminalMap_t::iterator it = m_terminal_map.find(guid);
	if (it != m_terminal_map.end())
	{
		ter = it->second;
	}

	return ter;
}

Terminal * ManageGateTerminal::getTerminalByPointer(void * terminal)
{
	for (TerminalVec_t::const_iterator it = m_terminal_vec.begin(); it != m_terminal_vec.end(); ++it)
	{
		if (*it == terminal)
		{
			return *it;
		}
	}

	return NULL;
}

void ManageGateTerminal::processPacketRoute(PacketRouteInfoQue_t * packet_route_info)
{
	if (NULL == packet_route_info)
	{
		return ;
	}

	for (PacketRouteInfoQue_t::iterator it = packet_route_info->begin(); it != packet_route_info->end(); ++it)
	{
		PacketRouteInfo & packet_route_info = *it;

		if (packet_route_info.packet->opcode() == SMSG_PLAYER_LEAVE_MAP)
		{
			removeRoute(packet_route_info.packet->guid(), packet_route_info.packet->getOwner());
		}

		// todo
		packet_route_info.terminal->outputPacket(packet_route_info.packet);
	}
}

//void ManageGateTerminal::processOutputPacket(Packet * packet)
//{
//	Terminal * ter = getTerminal(packet->guid());
//
//	if ((packet->opcode() == SMSG_PLAYER_LEAVE_WORLD) || (packet->opcode() == SMSG_PLAYER_SWITCH_MAP))
//	{
//		removeRoute(packet->guid(), packet->getOwner());
//	}
//
//	if (NULL != ter)
//	{
//		ter->IOutput_output(packet);
//	}
//	else
//	{
//		// fatal error
//		DEF_LOG_ERROR("failed to get gate terminal where dispatch packet, guid is <%s>\n", boost::lexical_cast<string>(packet->guid()).c_str());
//		delete packet;
//	}
//}

int ManageGateTerminal::stop()
{
	m_is_stop = true;
	for (ReactorVec_t::iterator it = m_reactor_vec.begin(); it != m_reactor_vec.end(); ++it)
	{
		ACE_Reactor * reactor = *it;
		reactor->end_reactor_event_loop();
	}

	m_player_route.stop();
	m_manage_gate_output.stop();
	for (TerminalVec_t::iterator it = m_terminal_vec.begin(); it != m_terminal_vec.end(); ++it)
	{
		GateTerminal * gt = *it;
		gt->finit();
	}
	return 0;
}

int ManageGateTerminal::finit()
{
	return 0;
}

int ManageGateTerminal::initing()
{
	if (this->activate(THR_JOINABLE, m_terminal_cfg.gate_cfg.network_thread_no) == -1)
	{
		DEF_LOG_ERROR("error to call active of ManageGateTerminal, last error is <%d>\n", ACE_OS::last_error());
		return -1;
	}

	// player route must be thread or should make sure the scene's packet order
	if (m_player_route.init(20) == -1)
	{
		DEF_LOG_ERROR("error to call active of PlayerRoute, last error is <%d>\n", ACE_OS::last_error());
		return -1;
	}

	if (m_manage_gate_output.init(m_terminal_cfg.gate_cfg.gate_addrs.size()) == -1)
	{
		return -1;
	}

	if (connectGate() == -1)
	{
		return -1;
	}
	return 0;
}

int ManageGateTerminal::connectGate()
{
	ACE_SOCK_Connector connector;
	ACE_INET_Addr addr;
	for (GateCfg::GateVec_t::iterator it = m_terminal_cfg.gate_cfg.gate_addrs.begin(); it != m_terminal_cfg.gate_cfg.gate_addrs.end(); ++it)
	{
		addr.set(it->c_str());
		GateTerminal * gate_terminal = new GateTerminal();
		if (connector.connect(gate_terminal->peer(), addr) == -1)
		{
			return -1;
		}
		else
		{
			gate_terminal->init("", this);
			m_terminal_vec.push_back(gate_terminal);
		}
	}

	while (true)
	{
		bool reactor_is_ok = false;
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_reactor_vec_mutex, -1);
			reactor_is_ok = m_reactor_vec.size() == m_terminal_cfg.gate_cfg.network_thread_no;
		}

		if (!reactor_is_ok)
		{
			ACE_OS::sleep(1);
		}
		else
		{
			break;
		}
	}

	// assign gate reactor and output thread
	int index = 0;
	int reactor_number = m_reactor_vec.size();
	for (TerminalVec_t::iterator it = m_terminal_vec.begin(); it != m_terminal_vec.end(); ++it)
	{
		ACE_Reactor * reactor = m_reactor_vec[index % reactor_number];
		reactor->register_handler(*it, ACE_Event_Handler::READ_MASK);

		m_manage_gate_output.handleGateTerminal(*it);
		++index;
	}

	return 0;
}

bool ManageGateTerminal::addRoute(uint64 guid, void * terminal)
{
	m_player_route.addRoute(guid, terminal);
	return true;

	//bool result = false;
	//Terminal * ter = reinterpret_cast<GateTerminal *>(terminal);
	//GateTerminal * gate_ter = dynamic_cast<GateTerminal *>(ter);
	//if (NULL != gate_ter)
	//{
	//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_terminal_map_mutex, false);
	//	TerminalMap_t::iterator it = m_terminal_map.find(guid);
	//	if (m_terminal_map.end() == it)
	//	{
	//		m_terminal_map.insert(std::make_pair(guid, gate_ter));
	//		result = true;
	//	}
	//	else
	//	{
	//		// fatal error, notify the player, then kick the player
	//		// todo kick the player
	//		DEF_LOG_ERROR("failed to get the player login again in manage gate terminal: <%s>\n", boost::lexical_cast<string>(guid).c_str());
	//	}
	//}

	//return result;
}

void ManageGateTerminal::removeRoute(uint64 guid, void * terminal)
{
	m_player_route.removeRoute(guid, terminal);

	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_terminal_map_mutex, );
	//TerminalMap_t::iterator it = m_terminal_map.find(guid);
	//if (m_terminal_map.end() != it)
	//{
	//	m_terminal_map.erase(it);
	//}
	//else
	//{
	//	DEF_LOG_ERROR("failed to find the single terminal while remove it\n");
	//}
}

void ManageGateTerminal::registerReactor(ACE_Reactor * reactor)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_reactor_vec_mutex, );
	m_reactor_vec.push_back(reactor);
}

void ManageGateTerminal::lostGateConnection()
{
	m_lost_gate_connection = true;
}


//Terminal * ManageGateTerminal::getTerminal(uint64 guid)
//{
//	Terminal * ter = NULL;
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_terminal_map_mutex, NULL);
//	TerminalMap_t::iterator it = m_terminal_map.find(guid);
//	if (it != m_terminal_map.end())
//	{
//		ter = it->second;
//	}
//
//	return ter;
//}
