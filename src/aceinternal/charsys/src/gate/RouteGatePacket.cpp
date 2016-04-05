
#include <ace/OS.h>
#include "RouteGatePacket.h"
#include "opcode.h"
#include "CheckRoleName.h"
#include "ManageAccountDatabase.h"
#include "../common/cachesys/inc/cachesys_opcode.hpp"
#include "RouteCachePacket.h"
#include "../common/material-cpp/protocol/msg_cache.pb.h"
#include "ManagePlayerInit.h"
#include "ManageGuid.h"
#include "ManageRoundomName.h"
#include "../../../common/material-cpp/protocol/msg_char.pb.h"

RouteGatePacket::RouteGatePacket()
: m_stop(false)
, m_max_role_number(1)
//, m_remote_logger(NULL)
{

}

RouteGatePacket::~RouteGatePacket()
{

}

int RouteGatePacket::init()
{
	//m_remote_logger = hiso::LogSys::CreateLogger();
	//if (NULL == m_remote_logger)
	//{
	//	////TRACELog(LM_ERROR, ACE_TEXT("failed to create logger\n"));
	//	return -1;
	//}

//	using namespace hiso;

	std::auto_ptr<Document> document (XMLFactory::create_document());

	//Element *elem = document->new_element ("logger");
	//string logger_addr = ManageConfig::instance()->getLogsysCfg().ip + ":" + ManageConfig::instance()->getLogsysCfg().port;
	//elem->set_attribute ("remote", logger_addr);
	//elem->set_attribute ("level", 3);
	//elem->set_attribute ("console", 0);

	//int res = m_remote_logger->open (elem);
	//if (res < -1)
	//{
	//	//////TRACELogp(LM_ERROR, ACE_TEXT("Failed to open syslog, address is <%s>\n"), logger_addr.c_str());
	//	delete m_remote_logger;
	//	m_remote_logger = NULL;
	//	return -1;
	//}

	if (this->activate() == -1)
	{
		return -1;
	}
	return 0;
}

int RouteGatePacket::stop()
{
	return 0;
}

int RouteGatePacket::finit()
{
	return 0;
}

int RouteGatePacket::handlePacket(Packet * packet_stream)
{
	DEF_LOG_DEBUG("get gate package, opcode is <%d>, guid is <%llu>\n", packet_stream->opcode(), packet_stream->guid());

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_queqe_mutex, -1);
	m_input_queue.push_back(packet_stream);
	return 0;
}

int RouteGatePacket::handleCachePacket(Packet * packet_stream)
{
	DEF_LOG_DEBUG("get cache packet, opcode is <%d>, guid is <%llu>\n", packet_stream->opcode(), packet_stream->guid());

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_queqe_mutex, -1);
	m_input_queue.push_back(packet_stream);
	return 0;
}

void RouteGatePacket::collectGateSession(GateSession * gate_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gate_session_map_mutex, );
	m_gate_session_map.insert(std::make_pair(gate_session, 0));
}

void RouteGatePacket::removeGateSession(GateSession * gate_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gate_session_map_mutex, );
	GateSessionMap_t::iterator it = m_gate_session_map.find(gate_session);
	if (it != m_gate_session_map.end())
	{
		m_gate_session_map.erase(it);
	}
}

bool RouteGatePacket::existGateSession(GateSession * gate_session)
{
	bool result = false;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gate_session_map_mutex, false);
	GateSessionMap_t::iterator it = m_gate_session_map.find(gate_session);
	if (it != m_gate_session_map.end())
	{
		result = true;
	}
	return result;
}

void RouteGatePacket::cacheRoleMakeFailed(uint64 role_guid)
{
	RoleInfo * role_info = getPlayerInfo(role_guid);
	if (NULL != role_info)
	{
		role_info->updateState(role_guid, false);
		if (role_info->is_finish())
		{
			if (existGateSession(role_info->gate_session))
			{
				ManageAccountDatabase::instance()->deleteRole(role_guid);
				sendClientErrorInfo(role_info->gate_session, role_guid, 1/*typed::protocol::smsg_error_ecode_EFAILED_MAKE_ROLE*/);
			}

			removeRoleInfo(role_info->player_guid);
		}
	}
}

int RouteGatePacket::svc(void)
{	
//	try
	ACE_Time_Value start_time;
	ACE_Time_Value time_diff;
	ACE_Time_Value pack_start_time;
	ACE_Time_Value pack_time_diff;
	uint64 pack_time = 0;
	uint64 time_interval = 0;
	ACE_Time_Value sleep_time(0, 1000);
	{
		PacketList_t packet_stream_list;
		while (!m_stop)
		{
			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_queqe_mutex, -1);
				std::copy(m_input_queue.begin(), m_input_queue.end(), std::back_inserter(packet_stream_list));
				m_input_queue.clear();
			}

			if (packet_stream_list.size() == 0)
			{
				ACE_OS::sleep(sleep_time);
				continue;
			}

			start_time = ACE_OS::gettimeofday();
			for (PacketList_t::iterator it = packet_stream_list.begin(); it != packet_stream_list.end(); ++it)
			{
				pack_start_time = ACE_OS::gettimeofday();

				Packet * packet = *it;

				doHandlePacket(packet);

				pack_time_diff = ACE_OS::gettimeofday() - pack_start_time;
				pack_time_diff.to_usec(pack_time);
				if (pack_time > 1000)
				{
					DEF_LOG_ERROR("gate pack time error, opcode is <%d>, guid is <%llu>, spend time is <%llu>\n", packet->opcode(), packet->guid(), pack_time);
				}

				delete packet;
			}

			if (packet_stream_list.size() > 100)
			{
				time_diff = ACE_OS::gettimeofday() - start_time;
				time_diff.to_usec(time_interval);

				DEF_LOG_INFO("process gate packet, size is <%d>, spend time is <%llu>\n", packet_stream_list.size(), time_interval);
			}

			packet_stream_list.clear();
		}
	}
//	catch (...)
	//{
	//	DEF_LOG_ERROR("Raise unknown exception in RouteGatePacket::svc, last error is <%d>\n", ACE_OS::last_error());
	//	return 2;
	//}

	DEF_LOG_INFO("Exit thread RouteGatePacket::svc, last error is <%d>\n", ACE_OS::last_error());
	return 0;
}	

void RouteGatePacket::doHandlePacket(Packet * packet_stream)
{
	int op_code = packet_stream->opcode();
	int handle_result = -1;

	if (CMSG_CHECK_PLAYER_NAME == op_code)
	{
		handle_result = handleCheckRoleName(packet_stream);
	}
	else if (CMSG_PLAYER_MAKE == op_code)
	{
		handle_result = handleRoleMake(packet_stream);
	}
	else if (CMSG_REQUEST_PLAYER_LIST == op_code)
	{
		handle_result = handleRequestRoleList(packet_stream);
	}
	else if (CMSG_PLAYER_DELETE == op_code)
	{
		handle_result = handleRoleDelete(packet_stream);
	}
	else if (CMSG_REQUEST_RANDOM_NAME == op_code)
	{
		handle_result = handleRequestRandomName(packet_stream);
	}
	else if (DCMSG_NEW_ENEITY == op_code)
	{
		handle_result = handleNewEneity(packet_stream);
	}
	else if (DCMSG_LOAD_ENTITY == op_code)
	{
		handle_result = handleLoadEneity(packet_stream);
	}
	else if (DCMSG_REMOVE_ENTITY == op_code)
	{
		handle_result = handleRemoveEneity(packet_stream);
	}
	//else if (DCMSG_ERROR_RESPONSE == op_code)
	//{
	//	handle_result = handleCacheError(packet_stream);
	//}

}

int RouteGatePacket::handleCheckRoleName(Packet * packet_stream)
{
	int result = -1;
	int error_number = 0;
	typed::protocol::cmsg_player_make role_make;
	//if (packet_stream->parse_protocol(role_make))
	if (PARSE_PACKET(packet_stream, &role_make))
	{
		if (CheckRoleName::instance()->checkRoleName(role_make.name(), error_number))
		{
			if (!ManageAccountDatabase::instance()->isExistRoleName(role_make.name(), error_number))
			{
				// pass
				result = 0;
			}
			else
			{
//				sendClientErrorInfo((GateSession *)packet_stream->getOwner(), packet_stream->guid(), typed::protocol::smsg_error_ecode_EFAILED_CHECK_ROLE_NAME);
			}
		}
		else
		{
//			sendClientErrorInfo((GateSession *)packet_stream->getOwner(), packet_stream->guid(), typed::protocol::smsg_error_ecode_EFAILED_CHECK_ROLE_NAME);
		}
	}

	if (0 != result)
	{
		sendClientErrorInfo((GateSession *)packet_stream->getOwner(), packet_stream->guid(), /*(typed::protocol::smsg_error_ecode)*/error_number);
	}

	return 0;
}

int RouteGatePacket::handleRoleMake(Packet * packet_stream)
{
	int result = 0;
	typed::protocol::cmsg_player_make role_make;
	int error_number = 0;
	//if (packet_stream->parse_protocol(role_make))
	if (PARSE_PACKET(packet_stream, &role_make))
	{
		if (isMakingRole(packet_stream->guid()))
		{
			DEF_LOG_ERROR("the client <%s> is already making role\n", role_make.account().c_str());
			return -1;
		}

		if (CheckRoleName::instance()->checkRoleName(role_make.name(), error_number))
		{
			if (!ManageAccountDatabase::instance()->isExistRoleName(role_make.name(), error_number))
			{
				int account_number = 0;
				if (ManageAccountDatabase::instance()->getAccountRoleNumber(role_make.account(), account_number, error_number)
					&& (account_number < m_max_role_number))
				{
					uint64 player_guid = 0;
					if (ManageAccountDatabase::instance()->insertRoleName(role_make.account(), role_make.name(), player_guid, error_number))
					{
						const char * ns = role_make.name().c_str();
						DEF_LOG_INFO("make role to database , name is <%s>\n", ns);
						RoleInfo role_info;
						role_info.account = role_make.account();
						role_info.packet_guid = packet_stream->guid();
						role_info.player_guid = player_guid;
						if (!ManageGuid::instance()->get_guid(ET_ROLE, role_info.role_guid))
						{
							return -1;
						}

						//role_info.op_type = COT_ADD;
						role_info.setOpType(COT_ADD);
//						role_info.faith = role_make.faith();
						role_info.gate_session = (GateSession *)packet_stream->getOwner();

						collectMakeRoleInfo(role_info);

						Packet * ps = new Packet(packet_stream->opcode(), player_guid, std::string(packet_stream->ch_body(), packet_stream->body_size()));
						RouteCachePacket::instance()->handleClientPacket(ps);
						result = 0;
					}
				}
				else
				{
					result = 0;
				}
			}
			else
			{
				result = 16;
			}
		}
		else
		{
			result = 17;
		}
	}

	if (0 != result)
	{
		sendClientErrorInfo((GateSession *)packet_stream->getOwner(), packet_stream->guid(), /*(typed::protocol::smsg_error_ecode)*/result);
	}

	return result;
}

int RouteGatePacket::handleRequestRoleList(Packet * packet_stream)
{
	int result = -1;
	int error_number = 1; //typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
	typed::protocol::cmsg_request_player_list request_role_list;
	//if (packet_stream->parse_protocol(request_role_list))
	if (PARSE_PACKET(packet_stream, &request_role_list))
	{
		AccountInfo account_info;
		account_info.packet_guid = packet_stream->guid();
		account_info.gate_session = (GateSession *)packet_stream->getOwner();
		account_info.account = request_role_list.account_id();
		if (ManageAccountDatabase::instance()->getRoleList(account_info, error_number))
		{
			if (account_info.role_map.size() == 0)
			{
				DEF_LOG_DEBUG("get request role list zero, account is <%s>, guid is <%llu>\n", request_role_list.account_id().c_str(), packet_stream->guid());
				replayRoleList(account_info);
				result = 0;
			}
			else
			{
				DEF_LOG_DEBUG("get request role list not zero, account is <%s>, guid is <%llu>, role size is <%d>\n", request_role_list.account_id().c_str(), packet_stream->guid(), account_info.role_map.size());
				cleanAccountInfo(account_info.account);
				m_account_info_map[account_info.account] = account_info;

				for (AccountInfo::RoleMap_t::iterator it = account_info.role_map.begin(); it != account_info.role_map.end(); ++it)
				{
					RoleInfo role_info;
					role_info.account = account_info.account;
					role_info.player_guid = it->first;
					role_info.packet_guid = packet_stream->guid();
					//role_info.op_type = COT_LOAD;
					role_info.setOpType(COT_LOAD);
					role_info.gate_session = (GateSession *)packet_stream->getOwner();
					{
						ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_role_info_map_mutex, -1);
						m_role_info_map[it->first] = role_info;
					}
					Packet * ps = new Packet(DCMSG_LOAD_ENTITY, it->first, "");
					RouteCachePacket::instance()->handleClientPacket(ps);
				}
				result = 0;
			}
		}
	}
	else
	{
		error_number = 1; //typed::protocol::smsg_error_ecode_EFAILED_PARSE_PROTOCOL;
	}
	
	if (0 != result)
	{
		sendClientErrorInfo((GateSession *)packet_stream->getOwner(), packet_stream->guid(), /*(typed::protocol::smsg_error_ecode)*/error_number);
	}

	return result;
}

int RouteGatePacket::handleRoleDelete(Packet * packet_stream)
{
	typed::protocol::cmsg_player_delete role_delete;
	//if (packet_stream->parse_protocol(role_delete))
	if (PARSE_PACKET(packet_stream, &role_delete))
	{
		if (!sendRemoteLog(role_delete.guid()))
		{
			return 0;
		}

		if (ManageAccountDatabase::instance()->deleteRole(role_delete.guid()))
		{
			//////TRACELogp(LM_INFO, ACE_TEXT("start to delete role, guid is <%s>\n"), boost::lexical_cast<string>(packet_stream->guid()).c_str());
			//{
			//	Packet * ps = new Packet(packet_stream->opcode(), packet_stream->guid(), role_delete.SerializeAsString());
			//	RouteCachePacket::instance()->handleClientPacket(ps);
			//}

			typed::protocol::smsg_player_delete smsg_delete;
			smsg_delete.set_guid(role_delete.guid());
			Packet * ps = new Packet(SMSG_PLAYER_DELETE, packet_stream->guid(), smsg_delete.SerializeAsString());
			GateSession * gate_session = (GateSession *)packet_stream->getOwner();
			if (existGateSession(gate_session))
			{
				gate_session->handleOutputStream(ps);
			}
			else
			{
				delete ps;
			}
		}
		else
		{
			//error
			DEF_LOG_ERROR("Failed to delete form role table\n");
		}
	}
	else
	{
		DEF_LOG_ERROR("Failed to parse delete message protocol\n");
	}
	return 0;
}

int RouteGatePacket::handleRequestRandomName(Packet * packet_stream)
{
	typed::protocol::cmsg_request_random_name random_name;
	if (PARSE_PACKET(packet_stream, &random_name))
	{
		string new_name;
		ManageRoundomName::instance()->getNewName(random_name.sex(), new_name);
		typed::protocol::smsg_request_random_name smsg_random_name;
		smsg_random_name.set_random_name(new_name);
		Packet * ps = new Packet(SMSG_REQUEST_RANDOM_NAME, packet_stream->guid(), smsg_random_name.SerializeAsString());
		GateSession * gate_session = (GateSession *)packet_stream->getOwner();
		if (existGateSession(gate_session))
		{
			gate_session->handleOutputStream(ps);
		}
		else
		{
			delete ps;
		}
	}
	else
	{
		DEF_LOG_ERROR("failed to parse request random name\n");
	}

	return 0;
}

int RouteGatePacket::handleNewEneity(Packet * packet_stream)
{
	int result = -1;
	RoleInfo * role_info = getPlayerInfo(packet_stream->guid());
	if (NULL == role_info)
	{
		// error
		DEF_LOG_ERROR("failed to get player info while handle new entity by guid : <%s>\n", boost::lexical_cast<string>(packet_stream->guid()).c_str());
		return -1;
	}

	if (packet_stream->opcode() == DCMSG_NEW_ENEITY)
	{
		protocol::OpResult cache_result;
		if (PARSE_PACKET(packet_stream, &cache_result)
			&& (cache_result.status() == protocol::OpResult_status_t_STATE_NORMAL))
		{
			// success
			role_info->updateState(packet_stream->guid(), true);

			result = 0;

			if (role_info->is_success())
			{
				if (ManageAccountDatabase::instance()->updateRoleStatus(role_info->player_guid, 1))
				{
					if (existGateSession(role_info->gate_session))
					{
						typed::protocol::smsg_player_make srm;
						srm.set_guid(packet_stream->guid());
						srm.set_map_id(ManagePlayerInit::instance()->getPlayerInitCfg().map_id);
						srm.set_line_id(ManageConfig::instance()->getLineId());
						Packet * ps = new Packet(SMSG_PLAYER_MAKE, role_info->packet_guid, srm.SerializeAsString());
						role_info->gate_session->handleOutputStream(ps);
						result = 0;
					}
					else
					{
						DEF_LOG_ERROR("the gate session does not exist, opcode is <%d>, guid is <%llu>, gate session is <%x>\n", packet_stream->opcode(), packet_stream->guid(), role_info->gate_session);
					}
				}
			}
		}
		else
		{
			DEF_LOG_ERROR("failed to parse packet, the opcode is <%d>, the op result is <%d>, guid is <%llu>\n", packet_stream->opcode(), cache_result.status(), packet_stream->guid());
		}
	}

	if (0 != result)
	{
		role_info->updateState(packet_stream->guid(), false);
		if (role_info->is_failed())
		{
			ManageAccountDatabase::instance()->deleteRole(role_info->player_guid);
			sendClientErrorInfo(role_info->gate_session, role_info->packet_guid, 1/*typed::protocol::smsg_error_ecode_EFAILED_MAKE_ROLE*/);
		}
	}

	if (role_info->is_finish())
	{
		removeRoleInfo(role_info->player_guid);
	}

	return 0;
}

int RouteGatePacket::handleLoadEneity(Packet * packet_stream)
{
	RoleInfo * role_info = getPlayerInfo(packet_stream->guid());
	if (NULL == role_info)
	{
		DEF_LOG_ERROR("failed to get player info while handle load entity by guid : <%s>\n", boost::lexical_cast<string>(packet_stream->guid()).c_str());
		return -1;
	}

	int result = -1;
	protocol::OpResult cache_result;
	//if (packet_stream->parse_protocol(cache_result)
	if (PARSE_PACKET(packet_stream, &cache_result)
		&& (cache_result.status() == protocol::OpResult_status_t_STATE_NORMAL))
	{
		// success
		role_info->updateState(packet_stream->guid(), true);
		if (role_info->is_success())
		{
			AccountInfo * account_info = getAccountInfo(role_info->account);
			if (NULL != account_info)
			{
				typed::entity::Player role;
				if (role.ParseFromString(cache_result.message()))
				{
					AccountInfo::RoleMap_t::iterator it = account_info->role_map.find(role.guid());
					if (it != account_info->role_map.end())
					{
						it->second = new typed::entity::Player(role);
						replayRoleList(*account_info);
					}
					result = 0;
				}
			}
		}
	}

	if (0 != result)
	{
		role_info->updateState(packet_stream->guid(), false);
		if (role_info->is_failed())
		{
			AccountInfo * account_info = NULL;
			Packet * ps = NULL;
			AccountInfo::RoleMap_t::iterator it;
			account_info = getAccountInfo(role_info->account);
			if (NULL != account_info)
			{
				it = account_info->role_map.find(packet_stream->guid());
				if (it != account_info->role_map.end())
				{
					it->second = (typed::entity::Player *)1;
					replayRoleList(*account_info);
				}
			}
		}
	}

	if (role_info->is_finish())
	{
		removeRoleInfo(role_info->player_guid);
	}

	return 0;
}

int RouteGatePacket::handleRemoveEneity(Packet * packet_stream)
{
	return 0;
}

void RouteGatePacket::collectMakeRoleInfo(const RoleInfo & role_info)
{
	DEF_LOG_DEBUG("start to collect role info, packet guid is <%llu>, player guid is <%llu>\n", role_info.packet_guid, role_info.player_guid);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_role_info_map_mutex, );
	m_role_info_map[role_info.player_guid] = role_info;
}

void RouteGatePacket::removeRoleInfo(uint64 player_guid)
{
	DEF_LOG_DEBUG("start to remove role info, player guid is <%llu>\n", player_guid);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_role_info_map_mutex, );
	RoleInfoMap_t::iterator it = m_role_info_map.find(player_guid);
	if (it != m_role_info_map.end())
	{
		m_role_info_map.erase(it);
	}
}

RoleInfo * RouteGatePacket::getPlayerInfo(uint64 role_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_role_info_map_mutex, NULL);
	if (IS_PLAYER_GUID(role_guid))
	{
		RoleInfoMap_t::iterator it = m_role_info_map.find(role_guid);
		return m_role_info_map.end() != it ? &it->second : NULL;
	}
	else if (IS_ROLE_GUID(role_guid))
	{
		for (RoleInfoMap_t::iterator it = m_role_info_map.begin(); it != m_role_info_map.end(); ++it)
		{
			if (role_guid == it->second.role_guid)
			{
				return &it->second;
			}
		}
	}

	return NULL;
}

void RouteGatePacket::sendClientErrorInfo(GateSession * gate_session, uint64 packet_guid, int  error_code)
{
	if (0 == error_code)
	{
		//error_code = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
	}

	typed::protocol::smsg_error_message msg_error;
	msg_error.set_msg_code(error_code);
	gate_session->handleOutputStream(new Packet(SMSG_ERROR_MESSAGE, packet_guid, msg_error.SerializeAsString()));
}

void RouteGatePacket::replayRoleList(const AccountInfo & account_info)
{
	bool load_all = true;
	for (AccountInfo::RoleMap_t::const_iterator it = account_info.role_map.begin(); it != account_info.role_map.end(); ++it)
	{
		if (NULL == it->second)
		{
			load_all = false;
			break;
		}
	}
	if (load_all)
	{
		int load_role_number = 0;
		if (existGateSession(account_info.gate_session))
		{
			typed::protocol::smsg_request_player_list role_list;
			for (AccountInfo::RoleMap_t::const_iterator it = account_info.role_map.begin(); it != account_info.role_map.end(); ++it)
			{
				if ((typed::entity::Player *)1 == it->second)
					continue;
				++load_role_number;
				typed::protocol::smsg_request_one_player * one_role = role_list.add_player_list();
				typed::entity::Player * role = it->second;
				one_role->set_guid(role->guid());
				one_role->set_name(role->name());
				one_role->set_profession(role->profession());
				one_role->set_sex(role->sex());
				//one_role->set_image(role->image());
				one_role->set_level(role->level());
				one_role->set_map_id(role->map_id());
				one_role->set_line_id(role->line_no());
				//one_role->set_map_instance(role->map_instance());
				//one_role->set_faith(role->faith());

				//for (int i = 0; i < role->aspects().size(); ++i)
				//{
				//	one_role->add_aspects(role->aspects(i));
				//}

				//for (int i = 0; i < role->aspects_level().size(); ++i)
				//{
				//	one_role->add_aspects_level(role->aspects_level(i));
				//}
			}
			role_list.set_player_num(load_role_number);
			Packet * ps = new Packet(SMSG_REQUEST_PLAYER_LIST, account_info.packet_guid, role_list.SerializeAsString());
			account_info.gate_session->handleOutputStream(ps);
		}
		cleanAccountInfo(account_info.account);
	}
}

AccountInfo * RouteGatePacket::getAccountInfo(const string & account)
{
	AccountInfoMap_t::iterator it = m_account_info_map.find(account);
	if (it != m_account_info_map.end())
	{
		return &it->second;
	}
	else
	{
		return NULL;
	}
}

void RouteGatePacket::cleanAccountInfo(const string & account)
{
	AccountInfoMap_t::iterator it = m_account_info_map.find(account);
	if (it != m_account_info_map.end())
	{
		for (AccountInfo::RoleMap_t::iterator it_role = it->second.role_map.begin(); it_role != it->second.role_map.end(); ++it_role)
		{
			if (NULL != it_role->second)
			{
				if ((typed::entity::Player *)1 != it_role->second)
				{
					delete it_role->second;
				}
				it_role->second = NULL;
			}
		}
		m_account_info_map.erase(it);
	}
}

bool RouteGatePacket::sendRemoteLog(uint64 role_guid)
{
	return true;

	//bool result = false;
	//if (NULL == m_remote_logger)
	//{
	//	////TRACELog(LM_ERROR, ACE_TEXT("romote logger is not init\n"));
	//}
	//else
	//{
	//	////TRACELog(LM_INFO, ACE_TEXT("start to remote log\n"));
	//	typed::protocol::msg_log_delete_role log_delete_role;
	//	log_delete_role.set_role_guid(role_guid);
	//	int log_error = m_remote_logger->remote(log_delete_role, hiso::Logger::LOGL_BASIC);
	//	if (log_error == 0)
	//	{
	//		result = true;
	//	}
	//	else
	//	{
	//		//////TRACELogp(LM_ERROR, ACE_TEXT("failed to send remote log message, error code is <%d>\n"), log_error);
	//		// error
	//	}
	//	////TRACELog(LM_INFO, ACE_TEXT("end to remote log\n"));
	//}
	//return result;
}

bool RouteGatePacket::isMakingRole(uint64 packet_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_role_info_map_mutex, true);
	for (RoleInfoMap_t::iterator it = m_role_info_map.begin(); it != m_role_info_map.end(); ++it)
	{
		if (packet_guid == it->second.packet_guid)
		{
			return true;
		}
	}

	return false;
}