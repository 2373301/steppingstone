
#include <ace/OS.h>
#include "PlayerRoute.h"
#include "ManageGateTerminal.h"
#include "Logger.h"

void TerminalRoute::addRoute(uint64 guid, void * terminal)
{
	Terminal * ter = reinterpret_cast<Terminal *>(terminal);
	if (NULL == ter)
	{
		return ;
	}

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_terminal_map_mutex, );
	m_terminal_map[guid] = ter;
	//TerminalMap_t::iterator it = m_terminal_map.find(guid);
	//if (m_terminal_map.end() == it)
	//{
	//	m_terminal_map.insert(std::make_pair(guid, ter));
	//}
	//else
	//{
	//	// fatal error, notify the player, then kick the player
	//	// todo kick the player
	//	DEF_LOG_ERROR("failed to get the player login again in manage gate terminal: <%s>\n", boost::lexical_cast<string>(guid).c_str());
	//}
}

void TerminalRoute::removeRoute(uint64 guid, void * terminal)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_terminal_map_mutex, );
	TerminalMap_t::iterator it = m_terminal_map.find(guid);
	if (m_terminal_map.end() != it)
	{
		m_terminal_map.erase(it);
	}
	else
	{
		DEF_LOG_ERROR("failed to find the terminal while remove it\n");
	}
}

Terminal * TerminalRoute::getRoute(uint64 guid)
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

PlayerRouteDispatch::PlayerRouteDispatch(ManageGateTerminal * manage_gt)
: m_manage_gate_terminal(manage_gt)
, m_packet_route_index(0)
, m_packet_route_pos(0)
, m_is_stop(false)
{}

PlayerRouteDispatch::~PlayerRouteDispatch()
{
	for (PacketRouteInfoQueMap_t::iterator it = m_packet_route_info_map.begin(); it != m_packet_route_info_map.end(); ++it)
	{
		PacketRouteInfoQue_t * pack_route_que = it->second;

		while (pack_route_que->size() > 0)
		{
			PacketRouteInfo pack_info = pack_route_que->front();
			delete pack_info.packet;
			pack_route_que->pop_front();
		}

		delete pack_route_que;
	}
}

int PlayerRouteDispatch::stop()
{
	m_is_stop = true;
	return 0;
}

int PlayerRouteDispatch::svc()
{
	REPORT_THREAD_INFO();

	PacketRouteInfoQue_t * packet_route_info = NULL;
	bool is_process = false;
	ACE_Time_Value sleep_time(0, 1000);

	while (!m_is_stop)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_route_info_map_mutex, -1);
			if (m_packet_route_info_map.size() > 0)
			{
				PacketRouteInfoQueMap_t::iterator it = m_packet_route_info_map.find(m_packet_route_pos);
				if (m_packet_route_info_map.end() != it)
				{
					packet_route_info = it->second;
					m_packet_route_info_map.erase(it);
					m_packet_route_pos = increaseNumber(m_packet_route_pos, 1);
				}
				else
				{
					continue;
				}
				is_process = true;
			}
			else
			{
				is_process = false;
			}
		}

		if (!is_process)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		m_manage_gate_terminal->processPacketRoute(packet_route_info);
		delete packet_route_info;
		packet_route_info = NULL;
	}
	return 0;
}

uint32 PlayerRouteDispatch::getRouteIndex()
{
	uint32 result = m_packet_route_index;
	m_packet_route_index = increaseNumber(m_packet_route_index, 1);

	return result;
}

void PlayerRouteDispatch::processPacketRoute(uint32 route_index, PacketRouteInfoQue_t * packet_route_info)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_route_info_map_mutex, );
	//PacketRouteInfoQueMap_t::iterator it = m_packet_route_info_map.find(route_index);
	//if (it != m_packet_route_info_map.end())
	//{
	//	//error
	//	DEF_LOG_ERROR("get redumplicate route index <%d>\n", route_index);
	//}
	m_packet_route_info_map.insert(std::make_pair(route_index, packet_route_info));
}



uint32 PlayerRouteDispatch::increaseNumber(uint32 src, uint32 increase_no)
{
	return (src + increase_no) % 0xFFFFFFFF;
}


PlayerRoute::PlayerRoute(ManageGateTerminal * manage_gt)
: m_manage_gate_terminal(manage_gt)
, m_player_route_dispatch(manage_gt)
, m_is_stop(false)
{}

PlayerRoute::~PlayerRoute()
{
	cleanPacketVec(m_packet_vec);
}

int PlayerRoute::init(int thread_no)
{
	if (this->activate(THR_JOINABLE, thread_no) == -1)
	{
		return -1;
	}

	if (m_player_route_dispatch.activate() == -1)
	{
		return -1;
	}

	return 0;
}

int PlayerRoute::stop()
{
	m_is_stop = true;
	m_player_route_dispatch.stop();
	return 0;
}

void PlayerRoute::input(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_vec_mutex, );
	m_packet_vec.push_back(packet);
}

void PlayerRoute::addRoute(uint64 guid, void * terminal)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_terminal_route_que_mutex, );
	for (TerminalRouteQue_t::iterator it = m_terminal_route_que.begin(); it != m_terminal_route_que.end(); ++it)
	{
		TerminalRoute * ter_r = *it;
		ter_r->addRoute(guid, terminal);
	}
}

void PlayerRoute::removeRoute(uint64 guid, void * terminal)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_terminal_route_que_mutex, );
	for (TerminalRouteQue_t::iterator it = m_terminal_route_que.begin(); it != m_terminal_route_que.end(); ++it)
	{
		TerminalRoute * ter_r = *it;
		ter_r->removeRoute(guid, terminal);
	}
}


int PlayerRoute::svc()
{
	REPORT_THREAD_INFO();

	ACE_Time_Value sleep_time(0, 1 * 1000);
	ACE_Time_Value start_time;
	ACE_Time_Value diff_time;

	Packet * packet = NULL;
	PacketVec_t packet_vec;
	uint32 packet_route_index = 0;

	TerminalRoute terminal_route;
	registerTerminalRoute(&terminal_route);

	while(!m_is_stop)
	{
		{
			packet_vec.clear();
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_vec_mutex, -2);
			std::copy(m_packet_vec.begin(), m_packet_vec.end(), std::back_inserter(packet_vec));
			if (packet_vec.size() > 0)
			{
				packet_route_index = m_player_route_dispatch.getRouteIndex();
			}
			m_packet_vec.clear();
		}

		if (packet_vec.empty())
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		if (packet_vec.size() > 10000)
		{
			DEF_LOG_INFO("PlayerRoute::svc, packet size is <%d>\n", packet_vec.size());
		}

		start_time = ACE_OS::gettimeofday();

		PacketRouteInfoQue_t * packet_route_info = new PacketRouteInfoQue_t();

		for (PacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
		{
			std::auto_ptr<Packet> packet(*it);
			if (0 == packet->guid())
			{
				Terminal * ter = this->getRouteByOwner(packet->getOwner());
				if (NULL != ter)
				{
					packet_route_info->push_back(PacketRouteInfo(packet.release(), ter));
				}
				else
				{
					DEF_LOG_ERROR("failed to find terminal of guid, guid is <%llu>, ower is <%d>\n", packet->guid(), packet->getOwner());
				}
			}
			else
			{
				Terminal * ter = terminal_route.getRoute(packet->guid());
				if (NULL != ter)
				{
					packet_route_info->push_back(PacketRouteInfo(packet.release(), ter));
				}
				else
				{
					DEF_LOG_ERROR("failed to find terminal of guid, guid is <%llu>, ower is <%d>\n", packet->guid(), packet->getOwner());
				}
			}
		}

		m_player_route_dispatch.processPacketRoute(packet_route_index, packet_route_info);

		if (packet_vec.size() > 10000)
		{
			diff_time = ACE_OS::gettimeofday() - start_time;
			uint64 spend_time = 0;
			diff_time.to_usec(spend_time);
			int st = spend_time;
			DEF_LOG_ERROR("PlayerRoute::svc, packet size is <%d>, spend time is <%d> u seconds\n", packet_vec.size(), st);
		}
	}
	return 0;
}

void PlayerRoute::registerTerminalRoute(TerminalRoute * ter_route)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_terminal_route_que_mutex, );
	m_terminal_route_que.push_back(ter_route);
}

Terminal * PlayerRoute::getRouteByOwner(void * owner)
{
	return m_manage_gate_terminal->getTerminalByPointer(owner);
}
