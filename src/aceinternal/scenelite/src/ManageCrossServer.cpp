
#include "ManageCrossServer.h"
#include "opcode.h"
#include "protocol/msg_cross.pb.h"

ManageCrossServer::ManageCrossServer()
: m_cross_server_session_pool(NULL)
, m_cross_server_connect(NULL)
, m_line(NULL)
{

}

ManageCrossServer::~ManageCrossServer()
{

}

void ManageCrossServer::crossServerInput(Packet * packet)
{
	if (SMSG_CROSS_SERVER_REGISTER == packet->opcode())
	{
		handleCrossServerRegister(packet);
	}
	else if (SMSG_CROSS_SERVER_DISCONNECT == packet->opcode())
	{
		handleCrossServerDisconnect(packet);
	}
	m_line->input(packet);
}

void ManageCrossServer::sendCrossServerMsg(Packet * packet)
{
	if (NULL != m_cross_server_connect)
	{
		m_cross_server_connect->output(packet);
	}
	else
	{
		DEF_LOG_ERROR("the cross server connection is NULL while send packet opcode is <%d>, guid is <%llu>\n", packet->opcode(), packet->guid());
		delete packet;
	}
}

void ManageCrossServer::sendLineServerMsg(const string & line_id, const string & platform, Packet * packet)
{
	if (NULL != m_cross_server_session_pool)
	{
		m_cross_server_session_pool->output(line_id, platform, packet);
	}
	else
	{
		DEF_LOG_ERROR("the cross server pool is NULL while send packet opcode is <%d>, guid is <%llu>, platform is <%s>, line is <%s>\n", packet->opcode(), packet->guid(), platform.c_str(), line_id.c_str());
		delete packet;
	}
}

void ManageCrossServer::broadLineServerMsg(Packet * packet)
{
	Packet * ps = NULL;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_online_server_info_map_mutex, );
	for (OnlineServerInfoMap_t::iterator it = m_online_server_info_map.begin(); it != m_online_server_info_map.end(); ++it)
	{
		OnlineServerInfo * server_info = it->second;
		if (packet->body_size() == 0)
		{
			ps = new Packet(packet->opcode(), packet->guid(), "");
		}
		else
		{
			ps = new Packet(packet->opcode(), packet->guid(), string(packet->ch_body(), packet->body_size()));
		}
		this->sendLineServerMsg(server_info->line_id, server_info->platform, ps);
	}

	delete packet;
}

bool ManageCrossServer::isOnline(const string & platform, const string & line_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_online_server_info_map_mutex, false);
	string key = platform + line_id;

	OnlineServerInfoMap_t::iterator it = m_online_server_info_map.find(key);
	if (it != m_online_server_info_map.end())
	{
		return true;
	}

	return false;
}

//void ManageCrossServer::addPlayerRoute(const string & line_id, const string & platform, uint64 player_guid)
//{
//	if (NULL != m_cross_server_session_pool)
//	{
//		m_cross_server_session_pool->addPlayerRoute(line_id, platform, player_guid);
//	}
//}

int ManageCrossServer::init(const CrossServerCfg & cs_cfg, const ServerCfg & server_cfg, const string & security_key, Scene * line)
{
	// todo
//	return 0;

	m_line = line;

	if (1 == cs_cfg.server_type)
	{
		m_cross_server_connect = new CrossServerConnect();
		m_cross_server_connect->setServerInfo(server_cfg, security_key, this);
		if (m_cross_server_connect->init(cs_cfg.cross_server_addr) == -1)
		{
			DEF_LOG_ERROR("failed to init remote cross server, addr is <%s>\n", cs_cfg.cross_server_addr.c_str());
			return -1;
		}
	}
	else
	{
		m_cross_server_session_pool = new CrossServerSessionPool();
		if (m_cross_server_session_pool->init(cs_cfg, security_key, this) == -1)
		{
			DEF_LOG_ERROR("failed to init cross server session pool\n");
			return -1;
		}
	}

	return 0;
}

void ManageCrossServer::handleCrossServerRegister(Packet * packet)
{
	typed::protocol::smsg_cross_server_register scs_register;
	if (parsePacket(packet, &scs_register))
	{
		collectOnlineServer(scs_register.platform(), scs_register.line_id());
	}
}

void ManageCrossServer::handleCrossServerDisconnect(Packet * packet)
{
	typed::protocol::smsg_cross_server_disconnect scs_disconnect;
	if (parsePacket(packet, &scs_disconnect))
	{
		removeOnlineServer(scs_disconnect.platform(), scs_disconnect.line_id());
	}
}

void ManageCrossServer::collectOnlineServer(const string & platform, const string & line_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_online_server_info_map_mutex, );
	string key = platform + line_id;

	OnlineServerInfo * online_info = new OnlineServerInfo();
	online_info->platform = platform;
	online_info->line_id = line_id;
	m_online_server_info_map.insert(std::make_pair(key, online_info));
}

void ManageCrossServer::removeOnlineServer(const string & platform, const string & line_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_online_server_info_map_mutex, );
	string key = platform + line_id;

	OnlineServerInfoMap_t::iterator it = m_online_server_info_map.find(key);
	if (it != m_online_server_info_map.end())
	{
		delete it->second;
		m_online_server_info_map.erase(it);
	}
}