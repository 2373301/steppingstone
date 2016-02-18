
#include "CrossServerSessionPool.h"
#include "SessionPoolFactory.h"
#include "Session.h"
#include "protocol/msg_cross.pb.h"
#include "opcode.h"
#include "ace/OS_NS_unistd.h"

CrossServerSessionPool::CrossServerSessionPool()
: m_session_pool(NULL)
, m_cross_server_input(NULL)
{

}

CrossServerSessionPool::~CrossServerSessionPool()
{

}

int CrossServerSessionPool::init(const CrossServerCfg & corss_sever_cfg, const string & security_key, CrossServerInput * cross_server_input)
{
	m_corss_sever_cfg = corss_sever_cfg;
	m_security_key = security_key;
	m_cross_server_input = cross_server_input;

	m_session_pool = SessionPoolFactory::createSessionPool();
	if (m_session_pool->init(1, 1, this) == -1)
	{
		DEF_LOG_ERROR("failed to init SessionPool of CrossServerSessionPool\n");
		return -1;
	}

	if (!m_session_pool->listen(m_corss_sever_cfg.cross_server_addr))
	{
		DEF_LOG_ERROR("failed to listen cross server <%s>\n", m_corss_sever_cfg.cross_server_addr.c_str());
		return -1;
	}

	if (this->activate() == -1)
	{
		DEF_LOG_ERROR("failed to active CrossServerSessionPool, last error is <%d>\n", ACE_OS::last_error());
		return -1;
	}

	return 0;
}

void CrossServerSessionPool::output(const string & line_id, const string & platform, Packet * packet)
{
	Session_t session = getSession(platform, line_id);
	packet->setOwner(session);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_que_mutex, );
	m_output_packet_que.push(packet);
}

//void CrossServerSessionPool::addPlayerRoute(const string & line_id, const string & platform, uint64 player_guid)
//{
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cross_server_player_route_map_mutex, );
//	CrossServerPlayerRouteInfoMap_t::iterator it = m_cross_server_player_route_map.find(player_guid);
//	if (it != m_cross_server_player_route_map.end())
//	{
//		delete it->second;
//		m_cross_server_player_route_map.erase(it);
//	}
//
//	CrossServerPlayerRouteInfo * csp_route_info = new CrossServerPlayerRouteInfo();
//	csp_route_info->platform = platform;
//	csp_route_info->line_id = line_id;
//	csp_route_info->player_guid = player_guid;
//	m_cross_server_player_route_map.insert(std::make_pair(player_guid, csp_route_info));
//}

int CrossServerSessionPool::svc()
{
	ACE_Time_Value sleep_time(0, 10000);
	PacketQue_t output_packet_que;
	while (true)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_que_mutex, 1);
			while (m_output_packet_que.size() > 0)
			{
				output_packet_que.push(m_output_packet_que.front());
				m_output_packet_que.pop();
			}
		}

		if (output_packet_que.size() == 0)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		while (output_packet_que.size() > 0)
		{
			Packet * packet = output_packet_que.front();
			if (m_session_pool->handleOutputStream(packet->getOwner(), packet->stream(), packet->stream_size()))
			{
				delete packet;
				output_packet_que.pop();
			}
			else
			{
				break;
			}
		}
	}
	return 0;
}

void CrossServerSessionPool::newConnection(Session_t session)
{
	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cross_session_info_map_mutex, );
	//CrossSessionInfoMap_t::iterator it = m_cross_session_info_map.find(session);
	//if (it != m_cross_session_info_map.end())
	//{
	//	DEF_LOG_ERROR("error to get repeated cross server session <%d>\n", session);
	//	delete it->second;
	//	m_cross_session_info_map.erase(it);
	//}

	//CrossSessionInfo * info = new CrossSessionInfo();
	//m_cross_session_info_map.insert(std::make_pair(session, info));
}

void CrossServerSessionPool::connectionClosed(Session_t session, int trigger_source)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cross_session_info_map_mutex, );
	for (CrossSessionInfoMap_t::iterator it = m_cross_session_info_map.begin(); it != m_cross_session_info_map.end(); ++it)
	{
		CrossSessionInfo * info = it->second;
		if (info->session == session)
		{
			typed::protocol::smsg_cross_server_disconnect scs_dis;
			scs_dis.set_platform(info->platform);
			scs_dis.set_line_id(info->line_id);
			Packet * ps = new Packet(SMSG_CROSS_SERVER_DISCONNECT, 0, scs_dis.SerializeAsString());
			if (NULL != m_cross_server_input)
			{
				m_cross_server_input->crossServerInput(ps);
			}
			delete it->second;
			m_cross_session_info_map.erase(it);
			break;
		}
	}

	m_validated_session_set.erase(session);
}

void CrossServerSessionPool::handleInputStream(Session_t session, ACE_Message_Block & msg_block)
{
	PacketVec_t packet_vec;
	parsePacketFromStream(session, msg_block, packet_vec);
	for (PacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
	{
		Packet * packet = *it;
		if (packet->opcode() == SMSG_CROSS_SERVER_REGISTER)
		{
			handleCrossServerRegister(session, packet);
		}

		if (!isValidatedSession(session))
		{
			delete packet;
			continue;
		}

		if (NULL != m_cross_server_input)
		{
			m_cross_server_input->crossServerInput(packet);
		}
		else
		{
			delete packet;
		}
	}
}

bool CrossServerSessionPool::handleCrossServerRegister(Session_t session, Packet * packet)
{
	typed::protocol::smsg_cross_server_register scs_register;
	if (parsePacket(packet, &scs_register))
	{
		if (checkServerValidatedInfo(session, scs_register.platform(), scs_register.line_id(), scs_register.security_key()))
		{
			collectServerRegisterInfo(session, scs_register.platform(), scs_register.line_id(), scs_register.security_key());
			return true;
		}
	}

	DEF_LOG_ERROR("failed to check cross server gegister, platform is <%s>, line id is <%s>, security key is <%s>\n", scs_register.platform().c_str(), scs_register.line_id().c_str(), scs_register.security_key().c_str());

	return false;
}

bool CrossServerSessionPool::isValidatedSession(Session_t session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cross_session_info_map_mutex, false);
	return m_validated_session_set.find(session) != m_validated_session_set.end();
}

bool CrossServerSessionPool::checkServerValidatedInfo(Session_t session, const string & platform, const string & line_id, const string & security_key)
{
	// todo
	return true;
}

void CrossServerSessionPool::collectServerRegisterInfo(Session_t session, const string & platform, const string & line_id, const string & security_key)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cross_session_info_map_mutex, );
	string server_id = platform + line_id;
	CrossSessionInfoMap_t::iterator it = m_cross_session_info_map.find(server_id);
	if (it != m_cross_session_info_map.end())
	{
		delete it->second;
		m_cross_session_info_map.erase(it);
	}

	CrossSessionInfo * cs_info = new CrossSessionInfo();
	cs_info->session = session;
	cs_info->platform = platform;
	cs_info->line_id = line_id;
	cs_info->validated = true;

	m_cross_session_info_map.insert(std::make_pair(server_id, cs_info));

	m_validated_session_set.insert(session);
}

Session_t CrossServerSessionPool::getSessionByPlayer(uint64 player_guid)
{
	string line_id;
	string platform;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cross_server_player_route_map_mutex, NULL);
		CrossServerPlayerRouteInfoMap_t::iterator it = m_cross_server_player_route_map.find(player_guid);
		if (it == m_cross_server_player_route_map.end())
		{
			return NULL;
		}
		else
		{
			platform = it->second->platform;
			line_id = it->second->line_id;
		}
	}

	string server_id = platform + line_id;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cross_session_info_map_mutex, NULL);
		CrossSessionInfoMap_t::iterator it = m_cross_session_info_map.find(server_id);
		if (it != m_cross_session_info_map.end())
		{
			return it->second->session;
		}
		else
		{
			return NULL;
		}
	}
}

Session_t CrossServerSessionPool::getSession(const string & platform, const string & line_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cross_session_info_map_mutex, NULL);
	string key_value = platform + line_id;
	CrossSessionInfoMap_t::iterator it = m_cross_session_info_map.find(key_value);
	if (it != m_cross_session_info_map.end())
	{
		return it->second->session;
	}
	else
	{
		DEF_LOG_ERROR("failed to get session by platform and line info, platform <%s>, line <%s>\n", platform.c_str(), line_id.c_str());
		return NULL;
	}
}