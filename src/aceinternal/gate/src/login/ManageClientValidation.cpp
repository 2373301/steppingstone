
#include <ace/Reactor.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
//#include <protocol/msg_login.pb.h>
#include "ManageClientValidation.h"
#include "opcode.h"
#include "PacketStream.h"
#include "ReportTrafficToLoginServer.h"
#include "BeatToLoginServer.h"
#include "ManageStat.h"
#include "protocol/msg_member.pb.h"
#include "RouteGSPacket.h"
#include "ManageLoginStream.h"

ManageClientValidation::ManageClientValidation()
: m_stop(false)
, m_wait(true)
, m_timer_id(0)
, m_report_traffic_timer_id(0)
, m_beat_to_login_timer_id(0)
, m_check_client_auth(true)
{

}

ManageClientValidation::~ManageClientValidation()
{

}

int ManageClientValidation::svc(void)
{
	FUNCTIONTRACE(ManageClientValidation::svc);
	REPORT_THREAD_INFO("ManageClientValidation::svc", ACE_OS::thr_self());

	ACE_Reactor * reactor = NULL;
	while (!m_stop)
	{

#ifdef WIN32
		reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
		reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(), true);
#endif

		LoginAcceptor login_acceptor;
		ACE_INET_Addr port_to_listen(sMsgCfg->getLSCfg().listen_port, sMsgCfg->getLSCfg().host.c_str());
		if (login_acceptor.open(port_to_listen, reactor) == -1)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to call login_acceptor.open, host is <%s>, the port is <%d>, last error is <%d>\n"), sMsgCfg->getClientCfg().host.c_str(), sMsgCfg->getClientCfg().listen_port, ACE_OS::last_error());
			return 1;
		}

		ReportTrafficToLoginServer report_traffic;

		m_timer_id = reactor->schedule_timer(this, NULL, ACE_Time_Value(3, 0), ACE_Time_Value(3, 0));
		if (m_timer_id == -1)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to schedule timer in ManageClientValidation::svc, last error is <%d>\n"), ACE_OS::last_error());
			return 1;
		}

		//m_report_traffic_timer_id = reactor->schedule_timer(&report_traffic, NULL, ACE_Time_Value(sMsgCfg->getLSCfg().report_interval), ACE_Time_Value(sMsgCfg->getLSCfg().report_interval));
		//if (m_report_traffic_timer_id == -1)
		//{
		//	GATELogp(LM_ERROR, ACE_TEXT("Failed to schedule timer in ManageClientValidation::svc for report traffic, last error is <%d>\n"), ACE_OS::last_error());
		//	return 1;
		//}

		//reactor->run_reactor_event_loop();
		while (true)
		{
			reactor->run_reactor_event_loop();
#ifdef WIN32
			break;
#else
			if (ACE_OS::last_error() == EINTR)
			{
				continue;
			}
			else
			{
				break;
			}
#endif
		}


		GATE_LOG_INFO(ACE_TEXT("Exit thread ManageClientValidation::svc, last error is <%d>\n"), ACE_OS::last_error());
	}

	delete reactor;

	m_wait = false;

	return 0;
}

int ManageClientValidation::handle_timeout(const ACE_Time_Value &current_time, const void *act)
{
	if (m_stop)
	{
		this->reactor()->cancel_timer(m_timer_id);
//		this->reactor()->cancel_timer(m_report_traffic_timer_id);
		this->reactor()->end_reactor_event_loop();
	}

	return 0;
}

int ManageClientValidation::init(int argc, ACE_TCHAR * argv[])
{
	m_check_client_auth = 1 == ManageConfig::instance()->getLSCfg().check_client_auth;
	if (this->activate() == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call activate in ManageClientValidation::init, last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init ManageClientValidation\n"));
		return 0;
	}
}

int ManageClientValidation::fini()
{
	m_stop = true;
	
	return 0;
}

int ManageClientValidation::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageClientValidation\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

void ManageClientValidation::handlePackage(PacketStream * package_stream)
{
	if (NULL == package_stream)
	{
		return;
	}

	int result = -1;

	//if (LOGIN_TOGATEWAY_AUTHMSG == package_stream->opcode())
	if (MMSG_MEMBER_LOGIN == package_stream->opcode())
	{
		result = handleMemberLogin(package_stream);
	}
	else if (MMSG_MEMBER_CHARGE == package_stream->opcode())
	{
		result = handleMemberCharge(package_stream);
	}
	else if (MMSG_MEMBER_GIVE_GIFT == package_stream->opcode())
	{
		result = handleMemberGiveGift(package_stream);
	}
	else
	{
		// error
		GATE_LOG_ERROR(ACE_TEXT("Get unknown login server message type, opcode is <%d>\n"), package_stream->opcode());
	}

	delete package_stream;

}

void ManageClientValidation::accountAuthSuccess(ClientSession * client_session)
{
	if (NULL == client_session)
	{
		return ;
	}
	
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_login_server_register_mutex, );
	GATE_LOG_INFO(ACE_TEXT("Getting client auth success account , account id is <%s>\n"), client_session->getAccountID().c_str());
	string account_id = client_session->getAccountID();

	typed::protocol::smsg_member_login_confirm sml_confirm;
	sml_confirm.set_request_index(1);
	sml_confirm.set_account(account_id);
	sml_confirm.set_line_id(client_session->getLineNo());

	//typed::protocol::gmsg_player_state player_state;
	//player_state.set_gate_id(ManageConfig::instance()->getGateID());
	//
	//player_state.set_account_id(account_id);
	//player_state.set_role_state(GPS_LOGIN_GATE);
	//player_state.set_line_no(client_session->getLineNo());
	PacketStream * ps = new PacketStream(SMSG_MEMBER_LOGIN_CONFIRM, 0, sml_confirm.SerializeAsString());
	ManageLoginStream::instance()->handleOutputStream(ps);
}

void ManageClientValidation::accountLogout(ClientSession * client_session)
{
	if (NULL == client_session)
	{
		return ;
	}

	accountLogout(client_session->getAccountID(), client_session->getLineNo());
}

void ManageClientValidation::accountLogout(const string & account_id, int line_no)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_login_server_register_mutex, );

	GATE_LOG_INFO(ACE_TEXT("Getting client logout ,msg to login server, account id is <%s>\n"), account_id.c_str());

	typed::protocol::smsg_member_logoff sm_logoff;
	sm_logoff.set_account(account_id);
	sm_logoff.set_line_id(line_no);


	//typed::protocol::gmsg_player_state player_state;
	//player_state.set_gate_id(ManageConfig::instance()->getGateID());
	//player_state.set_account_id(account_id);
	//player_state.set_role_state(GPS_LOGOUT_GATE);
	//player_state.set_line_no(line_no);
	PacketStream * ps = new PacketStream(SMSG_MEMBER_LOGOFF, 0, sm_logoff.SerializeAsString());
	ManageLoginStream::instance()->handleOutputStream(ps);
}

bool ManageClientValidation::validateClientSession(const string & session_id, ClientAuth *& client_auth, const string & account_id)
{
	if (!m_check_client_auth)
	{
		static ClientAuth ca;
		static int account_index = 1;
		//ca.account_id = account_id;
		static bool first_time = true;
		if (first_time)
		{
			first_time = false;

			ca.addiction_flag = 1;
			ca.line_id = 1;
			ca.network_type = 1;
			ca.session_id = "SEK_TEST";
		}
		ca.request_index = account_index;
		client_auth = &ca;

		return true;
	}
	else
	{
		bool result = false;
		ACE_Time_Value current_time = ACE_OS::gettimeofday();
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_auth_mutex, false);
		ClientAuthMap::iterator it = m_client_auth_map.find(session_id);
		if (it != m_client_auth_map.end())
		{
			ACE_Time_Value diff_time = current_time - it->second.register_time;
			if ((session_id == it->second.session_id) && (diff_time.sec() <= sMsgCfg->getLSCfg().validation_interval))
			{
				GATE_LOG_INFO(ACE_TEXT("success to validate client session, account is <%s>, session id is <%s>\n"), it->second.account_id.c_str(), session_id.c_str());
				client_auth = &it->second;
				//m_client_auth_map.erase(it);
				result = true;
			}
			else
			{
				m_client_auth_map.erase(it);
			}
		}
		return result;
	}
}

void ManageClientValidation::waitFromGSLogoutMsg(const string & account_id, int line_no, uint64 player_guid, uint64 map_id)
{
	GATE_LOG_INFO(ACE_TEXT("wait from gs logout msg, account id is <%s>, player guid is <%llu>\n"), account_id.c_str(), player_guid);
	LogoutAccount logout_account(account_id, line_no, player_guid, map_id);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_logout_account_mutex, );
	m_logout_account_map[player_guid] = logout_account;
}

bool ManageClientValidation::isInWaitQueue(uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_logout_account_mutex, false);
	LogoutAccountMap_t::iterator it = m_logout_account_map.find(player_guid);
	if (it != m_logout_account_map.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ManageClientValidation::logoutFromGS(uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_logout_account_mutex, );
	LogoutAccountMap_t::iterator it = m_logout_account_map.find(player_guid);
	if (it != m_logout_account_map.end())
	{
		accountLogout(it->second.account_id, it->second.line_no);
		m_logout_account_map.erase(it);
	}
	else
	{
		// error
		GATE_LOG_ERROR(ACE_TEXT("failed to get player account info, player guid is <%llu>\n"), player_guid);
	}
}

void ManageClientValidation::getOnlineClient(std::list<LogoutAccount> & online_client)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_logout_account_mutex, );
	for (LogoutAccountMap_t::iterator it = m_logout_account_map.begin(); it != m_logout_account_map.end(); ++it)
	{
		online_client.push_back(it->second);
	}
}

void ManageClientValidation::gsClosed(uint64 map_id)
{
	vector<uint64> gs_closed_player;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_logout_account_mutex, );
		for (LogoutAccountMap_t::const_iterator it = m_logout_account_map.begin(); it != m_logout_account_map.end(); ++it)
		{
			uint64 role_map = MAKE_MAP_GUID_(it->second.line_no, it->second.map_id & 0x0000000000FFFFFF);
			if ((role_map == map_id) || (map_id == it->second.map_id))
			{
				gs_closed_player.push_back(it->second.player_guid);
			}
		}		
	}

	for (vector<uint64>::iterator it = gs_closed_player.begin(); it != gs_closed_player.end(); ++it)
	{
		GATE_LOG_INFO(ACE_TEXT("gs closed, notify login sever, player guid is <%llu>\n"), (*it));
		logoutFromGS(*it);
	}
}

void ManageClientValidation::roleChooseLine(int account_id, int line_no)
{
	//if (NULL != m_login_session)
	//{
	//	typed::protocol::cmsg_choose_line choose_line;
	//	choose_line.set_account_id(account_id);
	//	choose_line.set_line_no(line_no);
	//	PacketStream * ps = new PacketStream(CMSG_CHOOSE_LINE, 0, choose_line.SerializeAsString());
	//	m_login_session->handleOutputPacket(ps);
	//}
}

void ManageClientValidation::handleMemberChargeConfirm(PacketStream * packet_stream)
{
	typed::protocol::smsg_member_charge_confirm charge_confirm;
	if (!packet_stream->parse_protocol(charge_confirm))
	{
		GATE_LOG_ERROR("failed to parse member charge confirm, opcode is <%d>, body size is <%d>\n", packet_stream->opcode(), packet_stream->body_size());
		return ;
	}

	GATE_LOG_INFO("get member charge confirm, account is <%s>, request index is <%d>\n", charge_confirm.account().c_str(), charge_confirm.request_index());

	ManageLoginStream::instance()->handleOutputStream(packet_stream);
}

int ManageClientValidation::handleMemberGiveGiftReturn(PacketStream * packet_stream)
{
	typed::protocol::smsg_member_give_gift give_gift;
	if (!packet_stream->parse_protocol(give_gift))
	{
		GATE_LOG_ERROR("failed to parse member give gift, opcode is <%d>, body size is <%d>\n", packet_stream->opcode(), packet_stream->body_size());
		return 0;
	}

	GATE_LOG_INFO("get member give gift, account is <%s>, request index is <%d>\n", give_gift.account().c_str(), give_gift.request_index());

	ManageLoginStream::instance()->handleOutputStream(packet_stream);

	return 0;
}

int ManageClientValidation::handleMemberLogin(PacketStream * packet_stream)
{
	typed::protocol::mmsg_member_login member_login;
	//if (!client_auth.ParseFromString(std::string(package_stream->pe_body(), package_stream->body_size())))
	if (!packet_stream->parse_protocol(member_login))
	{
		// error
		GATE_LOG_ERROR(ACE_TEXT("Failed to parse member login msg\n"));
	}
	else
	{
		// success 
		registerClientAuth(member_login);
		respondMemberLogin(member_login);
		GATE_LOG_INFO(ACE_TEXT("Getting login auth info, account is <%s>, session id is <%s>\n"), member_login.account().c_str(), member_login.security_key().c_str());
	}

	return 0;
}

int ManageClientValidation::handleMemberCharge(PacketStream * packet_stream)
{
	typed::protocol::mmsg_member_charge member_charge;
	if (!packet_stream->parse_protocol(member_charge))
	{
		GATE_LOG_ERROR("failed to parse member charge msg\n");
	}
	else
	{
		uint64 map_id = 0;

		map_id = MAKE_MAP_GUID_(member_charge.line_id(), 0);

		PacketStream * ps = new PacketStream(MMSG_MEMBER_CHARGE, 0, member_charge.SerializeAsString());
		RouteGSPacket::instance()->handleClientPackage(ps, map_id);
	}

	return 0;
}

int ManageClientValidation::handleMemberGiveGift(PacketStream * packet_stream)
{
	typed::protocol::mmsg_member_give_gift give_gift;
	if (!packet_stream->parse_protocol(give_gift))
	{
		GATE_LOG_ERROR("failed to parse member give gift msg\n");
	}
	else
	{
		uint64 map_id = 0;

		map_id = MAKE_MAP_GUID_(give_gift.line_id(), 0);

		PacketStream * ps = new PacketStream(MMSG_MEMBER_GIVE_GIFT, 0, give_gift.SerializeAsString());
		RouteGSPacket::instance()->handleClientPackage(ps, map_id);
	}

	return 0;
}

void ManageClientValidation::registerClientAuth(typed::protocol::mmsg_member_login & member_login)
{
	ManageStat::instance()->statLoginRecvAuth(1);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_auth_mutex, );
	ClientAuth auth_info(member_login.request_index(), member_login.account(), member_login.addiction_flag(), member_login.network_type(), member_login.security_key(), member_login.line_id());

	auth_info.open_id = member_login.open_id();
	auth_info.open_key = member_login.open_key();
	auth_info.pf = member_login.pf();
	auth_info.pfkey = member_login.pfkey();

	m_client_auth_map[auth_info.session_id] = auth_info;
}

void ManageClientValidation::respondMemberLogin(typed::protocol::mmsg_member_login & member_login)
{

	typed::protocol::smsg_gate_login_respond login_respond;
	login_respond.set_request_index(member_login.request_index());
	login_respond.set_account(member_login.account());
	login_respond.set_network_type(member_login.network_type());

	PacketStream * ps = new PacketStream(SMSG_GATE_LOGIN_RESPOND, 0, login_respond.SerializeAsString());
	ManageLoginStream::instance()->handleOutputStream(ps);
}