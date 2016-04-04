
#include "ManageGateSession.h"
#include <ace/Connector.h>
#include <ace/SOCK_Connector.h>
#include "Logger.h"
#include "msg_member.pb.h"
#include "ManageConfig.h"
#include "opcode.h"
#include "ManageMemberSession.h"
#include "ManageMemberSessionOutput.h"
#include "ManageOnlinePlayer.h"

ManageGateSession::ManageGateSession()
: m_is_connected(false)
{

}

ManageGateSession::~ManageGateSession()
{

}

void ManageGateSession::input(Packet * packet)
{
	// todo
	uint32 opcode = packet->opcode();

	DEF_LOG_DEBUG("get gate input packet, opcode is <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());

	if (SMSG_GATE_LOGIN_RESPOND == opcode)
	{
		handleGateLoginRespond(packet);
	}
	else if (SMSG_MEMBER_LOGIN_CONFIRM == opcode)
	{
		handleMemberLoginConfirm(packet);
	}
	else if (SMSG_MEMBER_CHARGE_CONFIRM == opcode)
	{
		handleMemberChargeConfirm(packet);
	}
	else if (SMSG_MEMBER_GIVE_GIFT == opcode)
	{
		handleMemberGiveGiftPacket(packet);
	}
	else if (SMSG_REPORT_ONLINE_NUMBER == opcode)
	{
		handleReportOnlineNumber(packet);
	}
	else if (SMSG_MEMBER_LOGOFF == opcode)
	{
		handlePlayerLogoff(packet);
	}
	else 
	{
		DEF_LOG_ERROR("get unknown gate packet, opcode is <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());
	}

	delete packet;
	packet = NULL;
}

int ManageGateSession::svc()
{
	ACE_Time_Value sleep_time(0, 1000);

	GateInfo * gate_info = getGateInfo();

	while (true)
	{
		if (NULL == gate_info)
		{
			gate_info = getGateInfo();
			ACE_OS::sleep(1);
		}

		if (!gate_info->is_connected)
		{
			DEF_LOG_INFO("start to connect the server : <%s>\n", gate_info->gate_addr.c_str());
			connectToServer(gate_info);
			ACE_OS::sleep(1);
		}

		if (!gate_info->is_connected)
		{
			continue;
		}

		if (NULL == gate_info->gate_session)
		{
			DEF_LOG_ERROR("the gate session is NULL\n");
		}

		int read_result = processRead(gate_info->gate_session);

		int write_result = processWrite(gate_info->gate_session);

		if ((1 == read_result) && (1 == write_result))
		{
			// no task
			ACE_OS::sleep(sleep_time);
		}
		else if ((2 == read_result) || (2 == write_result))
		{
			// lost connection
			gate_info->is_connected = false;
			delete gate_info->gate_session;
			gate_info->gate_session = NULL;
		}
	}

	return 0;
}

int ManageGateSession::init()
{
	const GateAddrVec_t & telecom_gate_addr = ManageConfig::instance()->getTelecomGateAddrs();
	for (GateAddrVec_t::const_iterator it = telecom_gate_addr.begin(); it != telecom_gate_addr.end(); ++it)
	{
		GateInfo * gate_info = new GateInfo();
		gate_info->gate_addr = it->gate_member;
		m_telecom_gate.push_back(gate_info);
	}

	//const GateAddrVec_t & united_gate_addr = ManageConfig::instance()->getUnitedGatesAddrs();
	//for (GateAddrVec_t::const_iterator it = united_gate_addr.begin(); it != united_gate_addr.end(); ++it)
	//{
	//	GateInfo * gate_info = new GateInfo();
	//	gate_info->gate_addr = it->gate_member;
	//	m_united_gate.push_back(gate_info);
	//}	

	if (!connectToServer())
	{
		DEF_LOG_ERROR("failed to connect the gate server \n");
		return -1;
	}

	if (this->activate() == -1)
	{
		return -1;
	}

	m_is_connected = true;
	
	return 0;
}

void ManageGateSession::handleOutputStream(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_que_mutex, );
	m_output_packet_que.push(packet);
}

void ManageGateSession::handleMemberLogin(membersys::mmsg_member_login & member_login)
{
	string account;
	account.resize(100, 0);
	memcpy((char *)account.data(), member_login.account, MEMBER_ACCOUNT_LENGTH);

	string security_key;
	security_key.resize(100, 0);
	memcpy((char *)security_key.data(), member_login.security_key, MEMBER_SECURITY_KEY_LENGTH);

	DEF_LOG_INFO("get member login, account is <%s>, security key is <%s>\n", account.c_str(), security_key.c_str());

	// todo
	typed::protocol::mmsg_member_login mmsg_member_login;
	mmsg_member_login.set_request_index(member_login.request_index);
	mmsg_member_login.set_account(member_login.account, sizeof(member_login.account));
	mmsg_member_login.set_addiction_flag(member_login.addiction_flag);
	mmsg_member_login.set_network_type(member_login.network_type);
	mmsg_member_login.set_security_key(member_login.security_key);
	mmsg_member_login.set_line_id(member_login.line_id);

	mmsg_member_login.set_open_id(member_login.open_id, sizeof(member_login.open_id));
	mmsg_member_login.set_open_key(member_login.open_key, sizeof(member_login.open_key));
	mmsg_member_login.set_pf(member_login.pf, sizeof(member_login.pf));
	mmsg_member_login.set_pfkey(member_login.pfkey, sizeof(member_login.pfkey));

	MAKE_NEW_PACKET(ps, MMSG_MEMBER_LOGIN, 0, mmsg_member_login.SerializeAsString());
	
	this->handleOutputStream(ps);
}

void ManageGateSession::handleMemberCharge(membersys::mmsg_member_charge & member_charge)
{
	string account;
	account.resize(100, 0);
	memcpy((char *)account.data(), member_charge.account, MEMBER_ACCOUNT_LENGTH);

	string role_guid;
	role_guid = getRoleGuid((char *)member_charge.role_guid);

	DEF_LOG_INFO("get member charge, account is <%s>, request index is <%d>, point is <%d>, role guid is <%llu>\n", account.c_str(), member_charge.request_index, member_charge.point, member_charge.role_guid);

	typed::protocol::mmsg_member_charge mmsg_member_charge;
	mmsg_member_charge.set_request_index(member_charge.request_index);
	mmsg_member_charge.set_account(member_charge.account);
	mmsg_member_charge.set_point(member_charge.point);
	mmsg_member_charge.set_line_id(member_charge.line_id);
	mmsg_member_charge.set_role_guid(role_guid);

	MAKE_NEW_PACKET(ps, MMSG_MEMBER_CHARGE, 0, mmsg_member_charge.SerializeAsString());
	
	this->handleOutputStream(ps);
}

void ManageGateSession::handleMemberGiveGift(membersys::mmsg_member_give_gift & member_give_gift)
{
	string account;
	account.resize(100, 0);
	memcpy((char *)account.data(), member_give_gift.account, MEMBER_ACCOUNT_LENGTH);

	DEF_LOG_INFO("get member give gift, account is <%s>, request index is <%d>, gift number is <%d>, gift type is <%d>, replay is <%d>\n"
		, account.c_str(), member_give_gift.request_index, member_give_gift.gift_number, member_give_gift.gift_type, member_give_gift.replay);

	typed::protocol::mmsg_member_give_gift give_gift;
	give_gift.set_request_index(member_give_gift.request_index);
	give_gift.set_line_id(member_give_gift.line_id);
	give_gift.set_account(account);
	give_gift.set_gift_number(member_give_gift.gift_number);
	give_gift.set_replay(member_give_gift.replay);
	give_gift.set_gift_type(member_give_gift.gift_type);
	give_gift.set_start_day(member_give_gift.start_day);
	give_gift.set_end_day(member_give_gift.end_day);
	give_gift.set_role_guid(member_give_gift.role_guid);

	MAKE_NEW_PACKET(ps, MMSG_MEMBER_GIVE_GIFT, 0, give_gift.SerializeAsString());

	this->handleOutputStream(ps);
}

void ManageGateSession::handleGateLoginRespond(Packet * packet)
{
	typed::protocol::smsg_gate_login_respond gate_respond;
	if (!PARSE_PACKET(packet, &gate_respond))
	{
		DEF_LOG_ERROR("failed to parse the member login respond message, opcode is <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());
		return ;
	}

	DEF_LOG_INFO("get member login respond, account is <%s>, request index is <%d>\n", gate_respond.account().c_str(), gate_respond.request_index());

	membersys::smsg_member_login_respond smsg_member_login_respond;
	copyAccount(smsg_member_login_respond.account, gate_respond.account().c_str());

	smsg_member_login_respond.request_index = gate_respond.request_index();

	string gate_addr = ManageConfig::instance()->getGateAddrByType(gate_respond.network_type());
	copyGateAddr(smsg_member_login_respond.gate_addr, gate_addr.c_str());

	MAKE_NEW_PACKET(ps, SMSG_MEMBER_LOGIN_RESPOND, 0, string((char *)&smsg_member_login_respond, sizeof(smsg_member_login_respond)));
	ps->setOwner((void *)smsg_member_login_respond.request_index);

	ManageMemberSessionOutput::instance()->handleOutputPacket(ps);
}

void ManageGateSession::handleMemberLoginConfirm(Packet * packet)
{
	typed::protocol::smsg_member_login_confirm login_confirm;
	if (!PARSE_PACKET(packet, &login_confirm))
	{
		DEF_LOG_ERROR("failed to parse the member login confirm, opcode is <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());
		return ;
	}

	ManageOnlinePlayer::instance()->playerLoginSuccess(login_confirm.line_id(), login_confirm.account());

	//membersys::smsg_member_login_confirm smsg_member_login_confirm;
	//copyAccount(smsg_member_login_confirm.account, login_confirm.account().c_str());

	//smsg_member_login_confirm.request_index = login_confirm.request_index();

	//MAKE_NEW_PACKET(ps, SMSG_MEMBER_LOGIN_CONFIRM, 0, string((char *)&smsg_member_login_confirm, sizeof(smsg_member_login_confirm)));
	//ps->setOwner((void *)smsg_member_login_confirm.request_index);

	//ManageMemberSessionOutput::instance()->handleOutputPacket(ps);
}

void ManageGateSession::handleMemberChargeConfirm(Packet * packet)
{
	typed::protocol::smsg_member_charge_confirm charge_confirm;
	if (!PARSE_PACKET(packet, &charge_confirm))
	{
		DEF_LOG_ERROR("failed to parse the member charge confirm, opcode is <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());
		return ;
	}

	membersys::smsg_member_charge_confirm smsg_member_charge_confirm;
	copyAccount(smsg_member_charge_confirm.account, charge_confirm.account().c_str());

	smsg_member_charge_confirm.request_index = charge_confirm.request_index();
	smsg_member_charge_confirm.result = charge_confirm.result();

	MAKE_NEW_PACKET(ps, SMSG_MEMBER_CHARGE_CONFIRM, 0, string((char *)&smsg_member_charge_confirm, sizeof(smsg_member_charge_confirm)));
	ps->setOwner((void *)smsg_member_charge_confirm.request_index);

	ManageMemberSessionOutput::instance()->handleOutputPacket(ps);
}

void ManageGateSession::handleMemberGiveGiftPacket(Packet * packet)
{
	typed::protocol::smsg_member_give_gift sm_give_gift;
	if (!PARSE_PACKET(packet, &sm_give_gift))
	{
		DEF_LOG_ERROR("failed to parse the member give gift, opcode is <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());
		return ;
	}

	membersys::smsg_member_give_gift struct_give_gift;
	struct_give_gift.request_index = sm_give_gift.request_index();
	struct_give_gift.line_id = sm_give_gift.line_id();
	copyAccount(struct_give_gift.account, sm_give_gift.account().c_str());
	copyRoleGuid(struct_give_gift.role_guid, sm_give_gift.role_guid().c_str());
	struct_give_gift.result = sm_give_gift.result();

	MAKE_NEW_PACKET(ps, SMSG_MEMBER_GIVE_GIFT, 0, string((char *)&struct_give_gift, sizeof(struct_give_gift)));
	ps->setOwner((void *)struct_give_gift.request_index);

	ManageMemberSessionOutput::instance()->handleOutputPacket(ps);
}

void ManageGateSession::handleReportOnlineNumber(Packet * packet)
{
	typed::protocol::smsg_report_online_number sr_online_number;
	if (!PARSE_PACKET(packet, &sr_online_number))
	{
		DEF_LOG_ERROR("failed to parse th report online number, opcode is <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());
		return ;
	}

	ManageOnlinePlayer::instance()->handleReportOnlineNumber(sr_online_number);
}

void ManageGateSession::handlePlayerLogoff(Packet * packet)
{
	typed::protocol::smsg_member_logoff sm_logoff;
	if (!PARSE_PACKET(packet, &sm_logoff))
	{
		DEF_LOG_ERROR("failed to parse the member logoff message, opcode is <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());
		return ;
	}
	ManageOnlinePlayer::instance()->playerLoginoff(sm_logoff.line_id(), sm_logoff.account());
}

bool ManageGateSession::connectToServer()
{
	if ((m_telecom_gate.size() == 0) && (m_united_gate.size() == 0))
	{
		return false;
	}

	bool result = true;
	for (GateInfoVec_t::iterator it = m_telecom_gate.begin(); it != m_telecom_gate.end(); ++it)
	{
		GateInfo * gate_info = *it;
		if (!connectToServer(gate_info))
		{
			result = false;
		}
	}

	return result;
}

bool ManageGateSession::connectToServer(GateInfo * gate_info)
{
	bool result = false;
	GateSession * gate_session = new GateSession();
	ACE_SOCK_Connector connector;
	ACE_INET_Addr addr(gate_info->gate_addr.c_str());
	if (connector.connect(gate_session->peer(), addr) == -1)
	{
		DEF_LOG_ERROR("failed to connect the server <%s>\n", gate_info->gate_addr.c_str());
		delete gate_session;
		gate_session = NULL;
		ACE_OS::sleep(3);
		result = false;
	}
	else
	{
		gate_session->reactor(NULL);
		gate_session->open();
		gate_session->setHandleInput(this);
		gate_info->gate_session = gate_session;
		gate_info->is_connected = true;
		result = true;
	}

	return result;
}

int ManageGateSession::processRead(GateSession * gate_session)
{
	if (NULL == gate_session)
	{
		return 1;
	}

	return gate_session->processRead();
}

int ManageGateSession::processWrite(GateSession * gate_session)
{
	if (NULL == gate_session)
	{
		return 1;
	}

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_que_mutex, 1);
		while (m_output_packet_que.size() > 0)
		{
			m_copy_output_packet_que.push(m_output_packet_que.front());
			m_output_packet_que.pop();
		}
	}

	if (m_copy_output_packet_que.size() == 0)
	{
		return 1;
	}

	return gate_session->processWrite(m_copy_output_packet_que);
}

GateInfo * ManageGateSession::getGateInfo()
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gate_mutex, NULL);
	for (GateInfoVec_t::iterator it = m_telecom_gate.begin(); it != m_telecom_gate.end(); ++it)
	{
		GateInfo * gate_info = *it;
		if (!gate_info->assigned)
		{
			gate_info->assigned = true;
			return gate_info;
		}
	}

	//for (GateInfoVec_t::iterator it = m_united_gate.begin(); it != m_united_gate.end(); ++it)
	//{
	//	GateInfo * gate_info = *it;
	//	if (!gate_info->assigned)
	//	{
	//		gate_info->assigned = true;
	//		return gate_info;
	//	}
	//}

	return NULL;
}

string ManageGateSession::getRoleGuid(char * role_guid)
{
	// todo
	return role_guid;
}
