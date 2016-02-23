
#include "RouteRolePacket.h"
#include "ManageConfig.h"
#include "RouteClientPackage.h"
#include "opcode.h"
#include "protocol/msg_player.pb.h"

RouteRolePacket::RouteRolePacket()
: m_stop(false)
, m_wait(true)
, m_role_session(NULL)
{

}

RouteRolePacket::~RouteRolePacket()
{

}

int RouteRolePacket::svc(void)
{
	FUNCTIONTRACE(RouteRolePacket::svc);
	REPORT_THREAD_INFO("RouteRolePacket::svc", ACE_OS::thr_self());
	PackageStreamList packet_stream_list;
//	try
	{
		while (!m_stop)
		{
			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_mutex, 1);
				packet_stream_list = m_packet_stream_list;
				m_packet_stream_list.clear();
			}

			if (packet_stream_list.size() == 0)
			{
				ACE_OS::sleep(ACE_Time_Value(0, 1000));
				continue;
			}

			if (packet_stream_list.size() >= 1000)
			{
				GATE_LOG_INFO(ACE_TEXT("In RouteRolePacket::svc getting packet stream list size is <%d>\n"), packet_stream_list.size());
			}

			for (PackageStreamList::iterator it = packet_stream_list.begin(); it != packet_stream_list.end(); ++it)
			{
				doHandlePackage(*it);
			}
			packet_stream_list.clear();
		}
	}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in RouteRolePacket::svc, last error is <%d>\n"), ACE_OS::last_error());
	//	return 2;
	//}

	GATE_LOG_INFO(ACE_TEXT("Exit thread RouteRolePacket::svc, last error is <%d>\n"), ACE_OS::last_error());

	m_wait = false;
	return 0;
}

int RouteRolePacket::init(int argc, ACE_TCHAR * argv[])
{
	int r = this->activate(THR_JOINABLE, sMsgCfg->getRoleCfg().handle_packet_thread_pool_number);
	if (r < 0)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call RouteRolePacket::init, the last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init RouteRolePacket.\n"));
		return 0;
	}
}

int RouteRolePacket::fini()
{
	m_stop = true;
	return 0;
}

int RouteRolePacket::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for RouteRolePacket\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

int RouteRolePacket::handlePacket(PacketStream * packet_stream)
{
	RouteClientPackage::instance()->putOutputPackage(packet_stream);

	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_mutex, 1);
	//m_packet_stream_list.push_back(packet_stream);
	return 0;
}

void RouteRolePacket::handleClientPackage(PacketStream * packet_stream)
{
	if (NULL != m_role_session)
	{
		m_role_session->handleOutputStream(packet_stream);
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get role session\n"));
		delete packet_stream;
	}
}

void RouteRolePacket::collectRoleSession(RoleSession * role_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_role_session_mutex,  );
	m_role_session_list.push_back(role_session);
	setRoleSession();
}

void RouteRolePacket::removeRoleSession(RoleSession * role_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_role_session_mutex,  );
	RoleSessionList::iterator it = std::find(m_role_session_list.begin(), m_role_session_list.end(), role_session);
	if (it != m_role_session_list.end())
	{
		m_role_session_list.erase(it);
	}
	else
	{
		// error
	}
	setRoleSession();
}

void RouteRolePacket::doHandlePackage(PacketStream * packet_stream)
{
	if (packet_stream->opcode() == SMSG_PLAYER_MAKE)
	{
		handlePlayerMake(packet_stream);
	}
	else if (packet_stream->opcode() == SMSG_REQUEST_PLAYER_LIST)
	{
		handlePlayerList(packet_stream);
	}
}

void RouteRolePacket::handlePlayerMake(PacketStream * packet_stream)
{
	typed::protocol::smsg_player_make player_make;
	if (packet_stream->parse_protocol(player_make))
	{
		if (player_make.map_id() > 99999)
		{
			player_make.set_map_id(player_make.map_id() / 1000);
		}

		PacketStream * ps = new PacketStream(packet_stream->opcode(), packet_stream->guid(), player_make.SerializeAsString());
		RouteClientPackage::instance()->putOutputPackage(ps);

		delete packet_stream;
		//packet_stream = new PacketStream(packet_stream->opcode(), packet_stream->guid(), player_make.SerializeAsString());
		//RouteClientPackage::instance()->handleRolePackage(packet_stream);
	}
	else
	{
		// error
	}
}

void RouteRolePacket::handlePlayerList(PacketStream * packet_stream)
{
	typed::protocol::smsg_request_player_list player_list;
	if (packet_stream->parse_protocol(player_list))
	{

		RouteClientPackage::instance()->putOutputPackage(packet_stream);

		//RouteClientPackage::instance()->handleRolePackage(packet_stream);
	}
	else
	{
		// error
	}
}

void RouteRolePacket::setRoleSession()
{
	if (m_role_session_list.size() > 0)
	{
		m_role_session = *m_role_session_list.begin();
	}
	else
	{
		m_role_session = NULL;
	}
}