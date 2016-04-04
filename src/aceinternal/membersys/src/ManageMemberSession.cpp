
#include <ace/Connector.h>
#include <ace/os_include/netinet/os_tcp.h>
#include <ace/SOCK_Connector.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "Logger.h"
#include "ManageMemberSession.h"
#include "ManageGateSession.h"
#include "opcode.h"
#include "ManageConfig.h"
#include "ManageRoleDb.h"
#include "ManageMemberSessionOutput.h"
#include "ManageOnlinePlayer.h"
#include "DynamicParseInterface.h"
#include <boost/typeof/typeof.hpp>

ManageMemberSession::ManageMemberSession()
: m_reactor(NULL)
{

}
ManageMemberSession::~ManageMemberSession()
{

}

int ManageMemberSession::handle_timeout (const ACE_Time_Value &current_time, const void *act)
{
	ManageRoleDb::instance()->pingDb();
	return 0;
}

void ManageMemberSession::input(Packet * packet)
{
	// todo
	uint32 opcode = packet->opcode();

	DEF_LOG_DEBUG("get member input packet, opcode is <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());

	if (MMSG_MEMBER_LOGIN == opcode)
	{
		handleMemberLogin(packet);
	}
	else if (MMSG_MEMBER_CHARGE == opcode)
	{
		handleMemberCharge(packet);
	}
	else if (MMSG_QUERY_ROLE == opcode)
	{
		handleMemberQueryRole(packet);
	}
	else if (MMSG_QUERY_ONLINE_NUMBER == opcode)
	{
		handleMemberQueryOnlineNumber(packet);
	}
	else if (MMSG_MEMBER_GIVE_GIFT == opcode)
	{
		handleMemberGiveGift(packet);
	}
    else if(MMSG_QUERY_REGISTER_PLAYERS == opcode)
    {
        handleQueryRegisterUserInfo(packet);
    }
    else if( MMSG_QUERY_DAILY_TOP_PLAYERS == opcode)
    {
        handleQueryDailyTopPlayers(packet);
    }
    else if( MMSG_QUERY_DAILY_AVERAGE_PLAYER == opcode)
    {
        handleQueryDailyAveragePlayers(packet);
    }
	else
	{
		DEF_LOG_ERROR("get unknown opcode <%d>, body size is <%d>\n", packet->opcode(), packet->body_size());
	}

	delete packet;
	packet = NULL;
}

int ManageMemberSession::svc()
{
#ifdef WIN32
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	m_reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(60000), true);
#endif

	int reactor_result = 0;

	int last_error = 0;

	ACE_Time_Value tv(15 * 60, 0);

	while (true)
	{
		m_reactor->schedule_timer(this, NULL, tv, tv);
		reactor_result = m_reactor->run_reactor_event_loop();
#ifdef WIN32
		break;
#else
		last_error = ACE_OS::last_error();
		if ((EINTR == last_error) || (EAGAIN == last_error))
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

int ManageMemberSession::init()
{
	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

//void ManageMemberSession::handleOutputStream(Packet * packet)
//{
//	
//	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_que_mutex, );
//	//m_output_packet_que.push(packet);
//}

void ManageMemberSession::handleMemberSession(MemberSession * member_session)
{
	member_session->setHandleInput(this);

	sendRegisterRespond(member_session);

	m_reactor->register_handler(member_session, ACE_Event_Handler::READ_MASK);

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_member_session_info_map_mutex, );
	m_member_session_info_map[member_session] = MemberSessionInfo();
}

MemberSession * ManageMemberSession::getMemberSession(void * pointer)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_member_session_info_map_mutex, NULL);
	MemberSessionInfoMap_t::iterator it = m_member_session_info_map.find((MemberSession *)pointer);
	if (it != m_member_session_info_map.end())
	{
		return it->first;
	}

	return NULL;
}

void ManageMemberSession::removeMemberSession(MemberSession * member_session)
{
	m_reactor->remove_handler(member_session, ACE_Event_Handler::DONT_CALL);

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_member_session_info_map_mutex, );
	MemberSessionInfoMap_t::iterator it = m_member_session_info_map.find(member_session);
	if (it != m_member_session_info_map.end())
	{
		m_member_session_info_map.erase(member_session);
	}
}

void ManageMemberSession::addMemberRoute(uint64 request_index, MemberSession * member_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_member_session_route_mutex, );
	MemberSessionRoute_t::iterator it = m_member_session_route.find(request_index);
	if (it != m_member_session_route.end())
	{
		DEF_LOG_ERROR("get redumplicate member route key <%d>, curr member is <%x>, new member is <%x>\n", request_index, it->second, member_session);
		return ;
	}
	else
	{
		m_member_session_route.insert(std::make_pair(request_index, member_session));
	}
}

MemberSession * ManageMemberSession::getMemberByRoute(uint64 request_index)
{
	MemberSession * member_session = NULL;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_member_session_route_mutex, NULL);
	MemberSessionRoute_t::iterator it = m_member_session_route.find(request_index);
	if (it != m_member_session_route.end())
	{
		member_session = it->second;
		m_member_session_route.erase(it);
		return member_session;
	}

	return NULL;
}

void ManageMemberSession::removeMemberRoute(uint64 request_index)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_member_session_route_mutex, );
	MemberSessionRoute_t::iterator it = m_member_session_route.find(request_index);
	if (it != m_member_session_route.end())
	{
		m_member_session_route.erase(it);
	}
}

//bool ManageMemberSession::connectToServer()
//{
//	m_member_session = new MemberSession();
//	ACE_SOCK_Connector connector;
//	ACE_INET_Addr addr(m_server_addr.c_str());
//	if (connector.connect(m_member_session->peer(), addr) == -1)
//	{
//		DEF_LOG_ERROR("failed to connect the server <%s>\n", m_server_addr.c_str());
//		delete m_member_session;
//		m_member_session = NULL;
//		ACE_OS::sleep(3);
//		return false;
//	}
//	else
//	{
//		m_member_session->reactor(NULL);
//		m_member_session->open();
//		makeMemberRegisterPacket();
//		return true;
//	}
//}

//int ManageMemberSession::processRead(MemberSession * member_session)
//{
//	if (NULL == member_session)
//	{
//		return 1;
//	}
//
//	return member_session->processRead();
//}
//
//int ManageMemberSession::processWrite(MemberSession * member_session)
//{
//	if (NULL == member_session)
//	{
//		return 1;
//	}
//
//	if (NULL != m_member_register_packet)
//	{
//		PacketQue_t back_que;
//		while (m_copy_output_packet_que.size() > 0)
//		{
//			back_que.push(m_copy_output_packet_que.front());
//			m_copy_output_packet_que.pop();
//		}
//
//		while (back_que.size() > 0)
//		{
//			m_copy_output_packet_que.push(back_que.front());
//			back_que.pop();
//		}
//	}
//
//	{
//		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_que_mutex, 1);
//		while (m_output_packet_que.size() > 0)
//		{
//			m_copy_output_packet_que.push(m_output_packet_que.front());
//			m_output_packet_que.pop();
//		}
//	}
//
//	if (m_copy_output_packet_que.size() == 0)
//	{
//		return 1;
//	}
//
//	return member_session->processWrite(m_copy_output_packet_que);
//}

//void ManageMemberSession::handleMemberRegister(Packet * packet)
//{
//	if (packet->body_size() != sizeof(membersys::mmsg_member_register))
//	{
//		DEF_LOG_ERROR("get member register packet, but the body size <%d> is wrong, right should be <%d>\n", packet->body_size(), sizeof(membersys::mmsg_member_register));
//		return ;
//	}
//
//	membersys::mmsg_member_register member_register;
//	memcpy(&member_register, packet->ch_body(), packet->body_size());
//}

void ManageMemberSession::handleMemberLogin(Packet * packet)
{
	if (packet->body_size() != sizeof(membersys::mmsg_member_login))
	{
		DEF_LOG_ERROR("get member login packet, but the body size <%d> is wrong, right size should be <%d>\n", packet->body_size(), sizeof(membersys::mmsg_member_login));
		return ;
	}

	membersys::mmsg_member_login member_login;
	memcpy(&member_login, packet->ch_body(), packet->body_size());

	this->addMemberRoute(member_login.request_index, (MemberSession *)packet->getOwner());

	ManageGateSession::instance()->handleMemberLogin(member_login);
}

void ManageMemberSession::handleMemberCharge(Packet * packet)
{
	if (packet->body_size() != sizeof(membersys::mmsg_member_charge))
	{
		DEF_LOG_ERROR("get member charge packet, but the body size <%d> is wrong, right size should be <%d>\n", packet->body_size(), sizeof(membersys::mmsg_member_charge));
		return ;
	}

	membersys::mmsg_member_charge member_charge;
	memcpy(&member_charge, packet->ch_body(), packet->body_size());

	this->addMemberRoute(member_charge.request_index, (MemberSession *)packet->getOwner());

	ManageGateSession::instance()->handleMemberCharge(member_charge);
}

void ManageMemberSession::handleQueryDailyAveragePlayers(Packet *packet)
{
    if (packet->body_size() != sizeof(membersys::mmsg_query_daily_players))
    {
        DEF_LOG_ERROR("get handleQueryDailyAveragePlayers packet, but the body size <%d> is wrong, right size should be <%d>\n", packet->body_size(), sizeof(membersys::mmsg_query_role));
        return ;
    }

    membersys::mmsg_query_daily_players req;
    memcpy(&req, packet->ch_body(), packet->body_size());
    
    typed::protocol::smsg_query_daily_players msg;
    msg.set_result(0);
    string date;
    date.resize(10, 0);
    memcpy((char *)date.data(), req.date, DATE_LENGTH);
    msg.set_num(ManageOnlinePlayer::instance()->getDailyAveragePlayers(date));

    string json;
    getJsonFormat(&msg, json);

    this->addMemberRoute(req.request_index, (MemberSession *)packet->getOwner());

    MAKE_NEW_PACKET(ps, SMSG_QUERY_DAILY_AVERAGE_PLAYER, 0, json);
    ps->setOwner((void *)req.request_index);

    ManageMemberSessionOutput::instance()->handleOutputPacket(ps);

}

void ManageMemberSession::handleQueryDailyTopPlayers(Packet *packet)
{
    if (packet->body_size() != sizeof(membersys::mmsg_query_daily_players))
    {
        DEF_LOG_ERROR("get handleQueryDailyTopPlayers packet, but the body size <%d> is wrong, right size should be <%d>\n", packet->body_size(), sizeof(membersys::mmsg_query_role));
        return ;
    }

    membersys::mmsg_query_daily_players req;
    memcpy(&req, packet->ch_body(), packet->body_size());

    typed::protocol::smsg_query_daily_players msg;
    msg.set_result(0);
    string date;
    date.resize(10, 0);
    memcpy((char *)date.data(), req.date, DATE_LENGTH);
    msg.set_num(ManageOnlinePlayer::instance()->getDailyTopPlayers(date));

    string json;
    getJsonFormat(&msg, json);

    this->addMemberRoute(req.request_index, (MemberSession *)packet->getOwner());

    MAKE_NEW_PACKET(ps, SMSG_QUERY_DAILY_TOP_PLAYERS, 0, json);
    ps->setOwner((void *)req.request_index);

    ManageMemberSessionOutput::instance()->handleOutputPacket(ps);
}

void ManageMemberSession::handleQueryRegisterUserInfo(Packet * packet)
{
    if (packet->body_size() != sizeof(membersys::mmsg_query_register_players))
    {
        DEF_LOG_ERROR("get query register user info packet, but the body size <%d> is wrong, right size should be <%d>\n", packet->body_size(), sizeof(membersys::mmsg_query_role));
        return ;
    }

    membersys::mmsg_query_register_players req;
    memcpy(&req, packet->ch_body(), packet->body_size());
    
     typed::protocol::smsg_query_register_user_info msg;
     msg.set_error_no(0);
     msg.set_all_pages(0);
    do 
    {
        string date;
        date.resize(10, 0);
        memcpy((char *)date.data(), req.date, DATE_LENGTH);
        if( 0 == req.numPerPage || 0 == req.current_page)
        {
            msg.set_error_no(1);
            break;
        }

        Uint64Vec_t guids;
        bool query_status = ManageRoleDb::instance()->queryRegisterUser(date, guids);
        uint32 guids_size = guids.size();
        if(0 == guids_size)
            break;

        std::sort(guids.begin(), guids.end());

        // 总页数
        uint32 numPerPage = req.numPerPage;
        uint32 current_page = req.current_page;
        uint32 total_pages =  guids_size / numPerPage;
        total_pages +=  guids_size % numPerPage ? 1 : 0; 
        msg.set_all_pages(total_pages);

        if(current_page > total_pages)
        {  
             break;
        }

        // guid容器的位移开始段
        uint32 start_pos = (current_page - 1) * numPerPage;
        start_pos = std::min(start_pos, guids_size - 1);
        uint32 count = 0;
        for(Uint64Vec_t::iterator it = guids.begin() + start_pos; 
            it != guids.end() && count < numPerPage; ++it, ++count)
        {
            ManageRoleDb::instance()->queryPlayerRegisterInfo(*it, msg);
        }

    } while (false);
    
    string json;
    getJsonFormat(&msg, json);

    this->addMemberRoute(req.request_index, (MemberSession *)packet->getOwner());
    
    MAKE_NEW_PACKET(ps, SMSG_QUERY_REGISTER_PLAYERS, 0, json);
    ps->setOwner((void *)req.request_index);

    ManageMemberSessionOutput::instance()->handleOutputPacket(ps);
}

void ManageMemberSession::handleMemberQueryRole(Packet * packet)
{
	if (packet->body_size() != sizeof(membersys::mmsg_query_role))
	{
		DEF_LOG_ERROR("get member query role packet, but the body size <%d> is wrong, right size should be <%d>\n", packet->body_size(), sizeof(membersys::mmsg_query_role));
		return ;
	}

	membersys::mmsg_query_role query_role;
	memcpy(&query_role, packet->ch_body(), packet->body_size());

	string account;
	account.resize(36, 0);
	memcpy((char *)account.data(), query_role.account, MEMBER_ACCOUNT_LENGTH);

	RoleInfoVec_t role_info_vec;

	bool query_status = ManageRoleDb::instance()->queryRoleInfo(account, role_info_vec);
	membersys::smsg_query_role_return query_return;
	query_return.request_index = query_role.request_index;
	copyAccount(query_return.account, query_role.account);
	query_return.line_id = query_role.line_id;
	if (query_status)
	{
		query_return.query_status = 1;
	}
	else
	{
		query_return.query_status = 2;
	}

	uint32 role_size = role_info_vec.size();
	uint32 buf_index = 0;
	uint32 buf_len = sizeof(query_return) + sizeof(role_size) + role_info_vec.size() * sizeof(membersys::RoleInfo);
	char * buffer = new char[buf_len];
	memset(buffer, 0, buf_len);

	memcpy(buffer, &query_return, sizeof(query_return));
	buf_index += sizeof(query_return);

	memcpy(buffer + buf_index, &role_size, sizeof(role_size));
	buf_index += sizeof(role_size);

	if (query_status)
	{
		for (RoleInfoVec_t::iterator it = role_info_vec.begin(); it != role_info_vec.end(); ++it)
		{
			membersys::RoleInfo & role_info = *it;
			memcpy(buffer + buf_index, &role_info, sizeof(role_info));
			buf_index += sizeof(role_info);
		}
	}

	this->addMemberRoute(query_role.request_index, (MemberSession *)packet->getOwner());

	MAKE_NEW_PACKET(ps, SMSG_QUERY_ROLE_RETURN, 0, string((char *)buffer, buf_len));
	ps->setOwner((void *)query_role.request_index);

	ManageMemberSessionOutput::instance()->handleOutputPacket(ps);

	delete buffer;
	buffer = NULL;
}

void ManageMemberSession::handleMemberQueryOnlineNumber(Packet * packet)
{
	if (packet->body_size() != sizeof(membersys::mmsg_query_online_number))
	{
		DEF_LOG_ERROR("get member query online number packet, but the body size <%d> is wrong, right size should be <%d>\n", packet->body_size(), sizeof(membersys::mmsg_query_online_number));
		return ;
	}

	membersys::mmsg_query_online_number mqo_number;
	memcpy(&mqo_number, packet->ch_body(), packet->body_size());

	uint32 online_number = ManageOnlinePlayer::instance()->getOnlineNumber(mqo_number.line_id);

	this->addMemberRoute(mqo_number.request_index, (MemberSession *)packet->getOwner());

	membersys::smsg_query_online_number  sqo_number;
	sqo_number.request_index = mqo_number.request_index;
	sqo_number.line_id = mqo_number.line_id;
	sqo_number.online_number = online_number;

	MAKE_NEW_PACKET(ps, SMSG_QUERY_ONLINE_NUMBER, 0, string((char *)&sqo_number, sizeof(sqo_number)));
	ps->setOwner((void *)sqo_number.request_index);
	
	ManageMemberSessionOutput::instance()->handleOutputPacket(ps);
}

void ManageMemberSession::handleMemberGiveGift(Packet * packet)
{
	if (packet->body_size() != sizeof(membersys::mmsg_member_give_gift))
	{
		DEF_LOG_ERROR("get member give gift packet, but the body size <%d> is wrong, right size should be <%d>\n", packet->body_size(), sizeof(membersys::mmsg_member_give_gift));
		return ;
	}

	membersys::mmsg_member_give_gift mm_give_gift;
	memcpy(&mm_give_gift, packet->ch_body(), packet->body_size());

	this->addMemberRoute(mm_give_gift.request_index, (MemberSession *)packet->getOwner());

	ManageGateSession::instance()->handleMemberGiveGift(mm_give_gift);

}

void ManageMemberSession::sendRegisterRespond(MemberSession * member_session)
{
	this->addMemberRoute((uint64)member_session, member_session);

	MAKE_NEW_PACKET(ps, SMSG_MEMBER_REGISTER_SUCCESS, 0, "");
	ps->setOwner(member_session);
	ManageMemberSessionOutput::instance()->handleOutputPacket(ps);
}

//void ManageMemberSession::makeMemberRegisterPacket()
//{
//	if (NULL != m_member_register_packet)
//	{
//		delete m_member_register_packet;
//		m_member_register_packet = NULL;
//	}
//
//	membersys::smsg_member_register member_register;
//	member_register.line_id = ManageConfig::instance()->getLineId();
//	copySecurityKey(member_register.security_key, ManageConfig::instance()->getMemberSecurityKey().c_str());
//
//	MAKE_NEW_PACKET(m_member_register_packet, SMSG_MEMBER_REGISTER, 0, string((char *)&member_register, sizeof(member_register)));
//
//}
