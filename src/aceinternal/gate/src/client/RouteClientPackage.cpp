
#include <ace/OS.h>
#include "GateLogger.h"
#include "RouteClientPackage.h"
#include "ManageConfig.h"
#include "RouteGSPacket.h"
#include "opcode.h"
#include "protocol/msg_gate.pb.h"
#include "protocol/msg_player.pb.h"
//#include "protocol/msg_test.pb.h"
#include "ManageClientValidation.h"
//#include "ManageDatabase.h"
#include "ManageStat.h"
#include "ClientStrategyCenter.h"
#include "RouteRolePacket.h"
#include "ManageClientSessionCooler.h"
#include "ManageMasterSession.h"
#include "ManageReduplication.h"
#include "ManageClientConnectionTimeout.h"
#include "ManageRoleGsState.h"

#define GET_ROLE_CLIENT_SESSION(rc) \
	ClientSession * client_session = this->getRoleClientSession(packet_stream->guid());	\
	if (NULL == client_session)	\
		return rc;


#define MAKE_GUID(type, seed, id) (((uint64(type) << 56) & 0xFF00000000000000) | ((uint64(seed) << 48) & 0x00FF000000000000) | (uint64(id) & 0x0000FFFFFFFFFFFF))

///////////////////     map guid          ////////////////////////////////
#define MAKE_MAP_GUID(line_no, repduplicate_index, template_id)			MAKE_GUID(0xC, 15, (uint64)0 << 40 | repduplicate_index << 24 | template_id)


//#define MAKE_GUID(type, ggen, id) (((uint64(type) << 56) & 0xFF00000000000000) | ((uint64(ggen) << 48) & 0x00FF000000000000) | (uint64(id) & 0x0000FFFFFFFFFFFF))
//
//#define MAKE_MAP_GUID(line, id) MAKE_GUID(0xC, 15, (((uint64)(line) << 40) | uint64(id)))

uint64 map_start_instance = 0xC15000000000001;
int    map_line_step_number = 0x100;

RouteClientPackage::RouteClientPackage()
: m_stop(false)
, m_wait(true)
, m_max_role_number(5)
, m_thread_packet_stream_index(0)
{
}

RouteClientPackage::~RouteClientPackage()
{
	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_mutex, );
	//for (PackageStreamList::iterator it = m_packet_stream_list.begin(); it != m_packet_stream_list.end(); )
	//{
	//	delete *it;
	//}
	//m_packet_stream_list.clear();
	for (ThreadPacketListInfoList::iterator it = m_thread_packet_list_info_list.begin(); it != m_thread_packet_list_info_list.end(); ++it)
	{
		delete *it;
	}
}

int RouteClientPackage::svc(void)
{
	FUNCTIONTRACE(RouteClientPackage::svc);
	REPORT_THREAD_INFO("RouteClientPackage::svc", ACE_OS::thr_self());

	ThreadPacketListInfo * thread_pli = new ThreadPacketListInfo();

	//PackageStreamList * thread_packet_stream_list = new PackageStreamList();
	//ACE_Thread_Mutex * thread_packet_stream_mutex = new ACE_Thread_Mutex();
	this->collectThreadPacketStreamList(thread_pli);

	// sleep while idle
	ACE_Time_Value sleep_time(0, 1 * 1000);

	PackageStreamList packet_stream_list_input;
	PackageStreamList packet_stream_list_output;

	Uint64Set_t wobble_guid;

	uint32 last_time_update_qq_request = ACE_OS::gettimeofday().sec();

	bool input_list_empty = false;
	bool output_list_empty = false;
	//try
	{
		while (!m_stop)
		{
			// process input packet
			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, *thread_pli->thread_packet_stream_input_mutex, 1);
				std::copy(thread_pli->thread_packet_stream_input_list->begin(), thread_pli->thread_packet_stream_input_list->end(), std::back_inserter(packet_stream_list_input));
				thread_pli->thread_packet_stream_input_list->clear();
			}

			input_list_empty = packet_stream_list_input.size() == 0;

			if (packet_stream_list_input.size() >= 1000)
			{
				GATE_LOG_INFO(ACE_TEXT("In RouteClientPackage::svc getting input packet stream list size is <%d>\n"), packet_stream_list_input.size());
			}

			for (PackageStreamList::iterator it = packet_stream_list_input.begin(); it != packet_stream_list_input.end(); ++it)
			{
				handleInputPackage(*it);
			}
			packet_stream_list_input.clear();

			// handle wobble client
			if (wobble_guid.size() > 0)
			{
				handleWobbleStream(wobble_guid);
			}

			//////////////////////////////////////////////////////////////////////////
			// process output packet
			//////////////////////////////////////////////////////////////////////////
			{
				ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, *thread_pli->thread_packet_stream_output_mutex, 1);
				std::copy(thread_pli->thread_packet_stream_output_list->begin(), thread_pli->thread_packet_stream_output_list->end(), std::back_inserter(packet_stream_list_output));
				thread_pli->thread_packet_stream_output_list->clear();
			}

			output_list_empty = packet_stream_list_output.size() == 0;

			if (input_list_empty && output_list_empty)
			{
				ACE_OS::sleep(sleep_time);
			}

			if (packet_stream_list_output.size() >= 1000)
			{
				GATE_LOG_INFO(ACE_TEXT("In RouteClientPackage::svc getting output packet stream list size is <%d>\n"), packet_stream_list_output.size());
			}

			int output_result = 0;
			for (PackageStreamList::iterator it = packet_stream_list_output.begin(); it != packet_stream_list_output.end(); ++it)
			{
				uint64 cl_guid = (*it)->guid();
				output_result = handleOutputPackage(*it);
				if (1 == output_result)
				{
					// send not finish 
					wobble_guid.insert(cl_guid);
				}
			}
			packet_stream_list_output.clear();


			//请求openapi
			if (last_time_update_qq_request > (ACE_OS::gettimeofday().sec() + 10))
			{
				last_time_update_qq_request = ACE_OS::gettimeofday().sec();
				std::list<PacketStream *> list_request;
				{
					ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_map_mutex, NULL);
					for (ClientSessionRoleMap::iterator iter_sess = m_client_session_map_id_map.begin();
						iter_sess != m_client_session_map_id_map.end();
						++iter_sess)
					{
						ClientSession* session = iter_sess->second;
						if (session != NULL && session->get_openapi_call_time() != 0)
						{
							//接近2个小时请求一次
							if ((last_time_update_qq_request - session->get_openapi_call_time()) > 7000)
							{
								PacketStream * packet_stream = new PacketStream(GTOG_QUERY_QQ_IS_LOGIN, session->getRoleGuid(), "");
								list_request.push_back(packet_stream);
							}
						}
					}
				}
				//
				if (!list_request.empty())
				{
					//ManageQQRequest::instance()->inputPacket(list_request);
				}
			}
		}
	}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in RouteClientPackage::svc, last error is <%d>\n"), ACE_OS::last_error());
	//	return 2;
	//}

	m_wait = false;
	GATE_LOG_INFO(ACE_TEXT("Exit thread RouteClientPackage::svc, last error is <%d>\n"), ACE_OS::last_error());
	return 0;
}

int RouteClientPackage::init(int argc, ACE_TCHAR * argv[])
{
	int r = this->activate(THR_JOINABLE, sMsgCfg->getClientCfg().handle_packet_thread_pool_number);
	if (r < 0)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call RouteClientPackage::init, the last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init RouteClientPackage.\n"));
		return 0;
	}
}

int RouteClientPackage::fini()
{
	m_stop = true;
	return 0;
}

int RouteClientPackage::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for RouteClientPackage\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

void RouteClientPackage::putInputPackage(PacketStream * packet_stream)
{
	ClientSession * client_session = getRoleClientSession(packet_stream->guid());
	if (NULL != client_session)
	{
		client_session->putInputPacket(packet_stream);
	}
	else
	{
		// error, todo 
		delete packet_stream;
	}
	//if (NULL == packet_stream)
	//{
	//	GATELog(LM_ERROR, ACE_TEXT("Getting null point of PacketStream in RouteGSPacket::putPackage\n"));
	//	return ;
	//}

	////ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_mutex, );
	////m_packet_stream_list.push_back(packet_stream);

	//ThreadPacketListInfo * thread_packet_list_info = NULL;

	//if (packet_stream->guid() != 0)
	//{
	//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_belong_thread_packet_mutex, );
	//	ClientBelongThreadPacketMap::iterator it = m_client_belong_thread_packet_map.find(packet_stream->guid());
	//	if (it != m_client_belong_thread_packet_map.end())
	//	{
	//		thread_packet_list_info = it->second;
	//	}
	//}

	//if (NULL == thread_packet_list_info)
	//{
	//	thread_packet_list_info = *m_thread_packet_list_info_list.begin();
	//}

	//if (NULL != thread_packet_list_info)
	//{
	//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, *thread_packet_list_info->thread_packet_stream_mutex, );
	//	thread_packet_list_info->thread_packet_stream_list->push_back(packet_stream);
	//}
	//else
	//{
	//	delete packet_stream;
	//}
}

void RouteClientPackage::putOutputPackage(PacketStream * packet_stream)
{
	ClientSession * client_session = this->getRoleClientSession(packet_stream->guid());
	if (NULL == client_session)
	{
		delete packet_stream;
		return ;
	}

	client_session->putOutputPacket(packet_stream);
}

//void RouteClientPackage::handleGSPackage(PacketStream * packet_stream)
//{
//	if (NULL == packet_stream)
//	{
//		GATELog(LM_ERROR, ACE_TEXT("Getting null point of PacketStream in RouteClientPackage::handleGSPackage\n"));
//		return ;
//	}
//	ClientSession * cs = getRoleClientSession(packet_stream->guid());
//	if (NULL != cs)
//	{
//		cs->handleOutputStream(packet_stream);
//	}
//	else
//	{
//		delete packet_stream;
//	}
//}

void RouteClientPackage::handleRolePackage(PacketStream * packet_stream)
{
	//	FUNCTIONTRACE(RouteClientPackage::handleGSPackage);
	if (NULL == packet_stream)
	{
		GATE_LOG_ERROR(ACE_TEXT("Getting null point of PacketStream in RouteClientPackage::handleGSPackage\n"));
		return ;
	}
	ClientSession * cs = getRoleClientSession(packet_stream->guid());
	if (NULL != cs)
	{
		cs->handleOutputStream(packet_stream);
	}
	else
	{
		/// this should never happen
		/// todo log 
		//		GATELogp(LM_ERROR, ACE_TEXT("Failed to get client session, guid is <%d>\n"), packet_stream->guid());
		delete packet_stream;
	}
}

void RouteClientPackage::collectRoleClientSession(uint64 role_guid, ClientSession * cs_session)
{
//	FUNCTIONTRACE(RouteClientPackage::collectRoleClientSession);
	//GATELogp(LM_INFO, ACE_TEXT("enter*** RouteClientPackage::collectRoleClientSession, role guid is <%s>, pointer is <%x>\n"), boost::lexical_cast<string>(role_guid).c_str(), (void *)cs_session);

	cs_session->setRoleGuid(role_guid);

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_map_mutex, );
	ClientSessionRoleMap::iterator it = m_client_session_map_id_map.find(role_guid);
	if (it != m_client_session_map_id_map.end())
	{
		GATE_LOG_ERROR(ACE_TEXT("role online again, account id is <%s>, guid is <%llu>\n"), cs_session->getAccountID().c_str(), role_guid);
		it->second->roleOnlineAgain();
		GATE_LOG_DEBUG("start to cooler cs : <%x> <%s>\n", it->second, __FUNCTION__);
		ManageClientSessionCooler::instance()->putClientSession(it->second);
		m_client_session_map_id_map.erase(it);
	}
	m_client_session_map_id_map[role_guid] = cs_session;

	//std::pair<ClientSessionRoleReverseMap::iterator, bool> reverse_result = m_client_session_reverse_map.insert(std::make_pair(cs_session, role_guid));
//	BOOST_ASSERT (reverse_result.second);

	// todo
	//if (! result.second)
		//m_client_session_map_id_map[role_guid] = cs_session;
}

ClientSession * RouteClientPackage::getRoleClientSession(uint64 role_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_map_mutex, NULL);
	ClientSessionRoleMap::iterator it = m_client_session_map_id_map.find(role_guid);
	if (it != m_client_session_map_id_map.end())
	{
		return it->second;
	}
	else
	{
		string str_guid = boost::lexical_cast<std::string>(role_guid);
		//GATELogp(LM_ERROR, ACE_TEXT("Failed to get client session by guid in RouteClientPackage::getRoleClientSession, guid is <%s>\n"), str_guid.c_str());
		return NULL;
	}
}

void RouteClientPackage::notFindGS(uint64 guid)
{
	// todo
	ClientSession * ci = getRoleClientSession(guid);
	if (NULL != ci)
	{
		ci->setNotFindGS(true);
		GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
		ManageClientSessionCooler::instance()->putClientSession(ci);
	}
}

void RouteClientPackage::removeRoleClientSession(uint64 role_guid, ClientSession * cs_session)
{

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_map_mutex, );
	
	{
		ClientSessionRoleMap::iterator it = m_client_session_map_id_map.find(role_guid);
		if (it != m_client_session_map_id_map.end())
		{
			GATE_LOG_INFO(ACE_TEXT("remove client session by guid in RouteClientPackage::removeRoleClientSession, guid is <%llu>, cs_session is <%x>, point is <%x>\n"), role_guid, cs_session, it->second);
			if (cs_session == it->second)
			{
				GATE_LOG_INFO(ACE_TEXT("real to remove client session by guid in RouteClientPackage::removeRoleClientSession, guid is <%llu>, point is <%x>\n"), role_guid, it->second);
				//this->accountOffline(it->second->getAccountID());
				m_client_session_map_id_map.erase(it);
			}
			else
			{
				// do nothing, logon again
			}
		}
		else
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to get client session by guid in RouteClientPackage::removeRoleClientSession, guid is <%llu>\n"), role_guid);
		}	
	}
}

void RouteClientPackage::collectThreadPacketStreamList(ThreadPacketListInfo * thread_packet_li)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_thread_packet_list_info_mutex, );
	m_thread_packet_list_info_list.push_back(thread_packet_li);
}

void RouteClientPackage::assignClientThreadPacketListInfo(ClientSession * cs_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_belong_thread_packet_mutex, );
	m_thread_packet_stream_index = ++m_thread_packet_stream_index % m_thread_packet_list_info_list.size();
	cs_session->setThreadPacketInfo(m_thread_packet_list_info_list[m_thread_packet_stream_index]);
	//m_client_belong_thread_packet_map[role_guid] = m_thread_packet_list_info_list[m_thread_packet_stream_index];
}

void RouteClientPackage::removeClientThreadPacketListInfo(uint64 role_guid)
{
	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_belong_thread_packet_mutex, );
	//ClientBelongThreadPacketMap::iterator it = m_client_belong_thread_packet_map.find(role_guid);
	//if (it != m_client_belong_thread_packet_map.end())
	//{
	//	m_client_belong_thread_packet_map.erase(it);
	//}
}

int RouteClientPackage::handleInputPackage(PacketStream * packet_stream)
{
	//try
	{
		int op_code = packet_stream->opcode();
		int handle_result = 0;
		GATE_LOG_DEBUG(ACE_TEXT("handle client packet opcode is :  <%d>, guid is <%llu>, body size is <%d>\n"), op_code, packet_stream->guid(), packet_stream->body_size());

		if (packet_stream->opcode() < 900)
		{
			// error
			delete packet_stream;
			GATE_LOG_ERROR("get unknown opcode, player is <%llu>, opcode is <%d>, size is <%d>\n", packet_stream->guid(), packet_stream->opcode(), packet_stream->body_size());
			return 0;
		}

		if (CMSG_VALIATION_ACCOUNT == op_code)
		{
			handle_result = handleValidationAccountPacket(packet_stream);
		}
		else if (CMSG_CHECK_PLAYER_NAME == op_code)
		{
			RouteRolePacket::instance()->handleClientPackage(packet_stream);
			handle_result = 0;
		}
		else if (CMSG_PLAYER_MAKE == op_code)
		{
			std::auto_ptr<PacketStream> auto_ps(packet_stream);
			typed::protocol::cmsg_player_make role_make;
			if (packet_stream->parse_protocol(role_make))
			{
				ClientSession * cs = getRoleClientSession(packet_stream->guid());
				if (NULL != cs)
				{
					role_make.set_gm_level(cs->getGMLevel());
					role_make.set_account(cs->getAccountID());
					PacketStream * ps = new PacketStream(packet_stream->opcode(), packet_stream->guid(), role_make.SerializeAsString());
					RouteRolePacket::instance()->handleClientPackage(ps);
					handle_result = 0;
				}
			}
		}
		else if (CMSG_REQUEST_RANDOM_NAME == op_code)
		{
			RouteRolePacket::instance()->handleClientPackage(packet_stream);
			handle_result = 0;
		}
		else if (CMSG_PLAYER_DELETE == op_code)
		{
			//RouteRolePacket::instance()->handleClientPackage(packet_stream);
			delete packet_stream;
			handle_result = 0;
		}
		else if (CMSG_PLAYER_ENTER_MAP == op_code)
		{
			handle_result = handleEnterMap(packet_stream);
		}
		else if (CMSG_REQUEST_PLAYER_LIST == op_code)
		{
			RouteRolePacket::instance()->handleClientPackage(packet_stream);
			handle_result = 0;
		}
		else if (CMSG_LOOPBACK == op_code)
		{
			handle_result = handleCmsgLoopback(packet_stream);
		}
		else if (CMSG_PLAYER_BEAT == op_code)
		{
			handle_result = handleRoleBeat(packet_stream);
		}
		//else if (GMSG_CLIENT_CLOSE == op_code)
		//{
		//	handle_result = handleClientClose(packet_stream);
		//}
		else if (CMSG_PLAYER_LEAVE_MAP == op_code)
		{
			handleRoleLogout(packet_stream);
		}
		else if (GMSG_CLIENT_READ_ERROR == op_code)
		{
			handleClientReadError(packet_stream);
		}
		else if (GMSG_CLIENT_WRITE_ERROR == op_code)
		{
			handleClientWriteError(packet_stream);
		}
		else if (GMSG_CLIENT_WOBBLE == op_code)
		{
			handleClientWobbleError(packet_stream);
		}
		else if (GMSG_CLIENT_BEAT_TIMEOUT == op_code)
		{
			handleClientBeatTimeout(packet_stream);
		}
		else
		{
			int hr = RouteGSPacket::instance()->handleClientPackage(packet_stream, packet_stream->getRoleLocation());
			if (-1 == hr)
			{
				// not find gs
				notFindGS(packet_stream->guid());
			}
			handle_result = 0;
		}
	}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in RouteClientPackage::handlePackage, last error is <%d>\n"), ACE_OS::last_error());
	//	return 1;
	//}
	return 0;
}

//int RouteClientPackage::handleTestTimeInterval(PacketStream * packet_stream)
//{
//	//ClientSession * client_session = (ClientSession *)packet_stream->getEventHandler();
//	GET_ROLE_CLIENT_SESSION(-1);
//	//collectRoleClientSession(packet_stream->guid(), client_session);
//	delete packet_stream;
//	return 0;
//}

int RouteClientPackage::handleValidationAccountPacket(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	int result = -1;
	GET_ROLE_CLIENT_SESSION(-1);
	typed::protocol::cmsg_validate_account validation_account;
	if (packet_stream->parse_protocol(validation_account))
	{
		uint32 gm_level = 1;
		uint32 account_level = 1;
		//ManageClientValidation::instance()->roleChooseLine(validation_account.account_id(), 1);
		ClientAuth * client_auth = NULL;
		if (ManageClientValidation::instance()->validateClientSession(validation_account.session_id(), client_auth, validation_account.account_id()))
		{
			GATE_LOG_INFO("client validate, account is <%s>, ip addr is <%s>\n", validation_account.account_id().c_str(), client_session->getClientIp().c_str());
			if (ManageMasterSession::instance()->isValidatedAccountAndIP(validation_account.account_id(), client_session->getClientIp()))
			{
				//
				if (client_auth != NULL)
				{
					client_session->setOpenId(client_auth->open_id);
					client_session->setOpenKey(client_auth->open_key);
					client_session->setPF(client_auth->pf);
					client_session->set_openapi_call_time(ACE_OS::gettimeofday().sec());
				}

				ManageClientConnectionTimeout::instance()->removeClientSession(client_session);
				client_session->setAccountID(validation_account.account_id());
				client_session->setValidation(true);
				client_session->setLineNo(client_auth->line_id);
				client_session->setAddictionFlag(client_auth->addiction_flag);
				ClientStrategyCenter::instance()->clientAuthSuccess(client_session);
				//accountOnline(validation_account.account_id(), client_session);
				//client_session->createEncryKey();
				//client_session->createEncryKeyObject();
				//client_session->sendClientEncryKey();

				typed::protocol::cmsg_request_player_list request_player_list;
				// todo
				request_player_list.set_account_id(client_session->getAccountID());
				PacketStream * ps = new PacketStream(CMSG_REQUEST_PLAYER_LIST, packet_stream->guid(), request_player_list.SerializeAsString());
				RouteRolePacket::instance()->handleClientPackage(ps);

				result = 0;
			}
			else
			{
				// forbid ip
				GATE_LOG_INFO("the ip address is forbade, account is <%s>, ip is <%s>\n", client_session->getAccountID().c_str(), client_session->getClientIp().c_str());
				ManageClientSessionCooler::instance()->putClientSession(client_session);
			}
		}
		else
		{
			ManageStat::instance()->statLoginFailedAuth(1);
			ClientStrategyCenter::instance()->clientAuthFailed(client_session);
			GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
			ManageClientSessionCooler::instance()->putClientSession(client_session);
			GATE_LOG_ERROR(ACE_TEXT("Failed to validation client session, account is <%s>, session id is <%s>\n"), validation_account.account_id().c_str(), validation_account.session_id().c_str());
		}
	}
	else
	{
		ClientStrategyCenter::instance()->parseProtocolFailed(client_session, packet_stream->opcode());
		GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
		ManageClientSessionCooler::instance()->putClientSession(client_session);
		GATE_LOG_ERROR(ACE_TEXT("Failed to parse validation string, error is <%d>, body size is <%d>\n"), ACE_OS::last_error(), packet_stream->body_size());
	}
	return result;
}

//int RouteClientPackage::handleChooseLinePacket(PacketStream * packet_stream)
//{
//	//int result = -1;
//	//GET_ROLE_CLIENT_SESSION(-1);
//	//typed::protocol::cmsg_choose_line choose_line;
//	//if (packet_stream->parse_protocol(choose_line))
//	//{
//	//	client_session->setLineNo(choose_line.line_no());
//
//	//	typed::protocol::cmsg_request_role_list request_role_list;
//	//	request_role_list.set_account_id(client_session->getAccountID());
//	//	PacketStream * ps = new PacketStream(CMSG_REQUEST_ROLE_LIST, packet_stream->guid(), request_role_list.SerializeAsString());
//	//	RouteRolePacket::instance()->handleClientPackage(ps);
//
//	//	ManageClientValidation::instance()->roleChooseLine(choose_line.account_id(), choose_line.line_no());
//
//	//	delete packet_stream;
//	//	result = 0;
//	//}
//	//else
//	//{
//	//	ClientStrategyCenter::instance()->parseProtocolFailed(client_session, packet_stream->opcode());
//	//	GATELogp(LM_ERROR, ACE_TEXT("Failed to parse choose line string, last error is <%d>\n"), ACE_OS::last_error());
//	//}
//	//return result;
//	return 0;
//}

int RouteClientPackage::handleCheckRoleName(PacketStream * packet_stream)
{
	int result = -1;
	//PacketStream * ps = NULL;
	//typed::protocol::cmsg_check_role_name check_name;
	////ClientSession * client_session = (ClientSession *)packet_stream->getEventHandler();
	//GET_ROLE_CLIENT_SESSION(-1);
	////if (check_name.ParseFromString(std::string(packet_stream->pe_body(), packet_stream->body_size())))
	//if (packet_stream->parse_protocol(check_name))
	//{
	//	// todo
	//	int32 error_number;
	//	if (ManageDatabase::instance()->checkRoleName(check_name.name(), error_number))
	//	{
	//		ps = new PacketStream(SMSG_CHECK_ROLE_NAME, packet_stream->guid(), ACE_TEXT(""));
	//		result = 0;
	//		client_session->handleOutputStream(ps);
	//		delete packet_stream;
	//	}
	//	else
	//	{
	//		ClientStrategyCenter::instance()->checkRoleNameFailed(client_session);
	//	}
	//}
	//else
	//{
	//	//todo currently do nothing
	//	ClientStrategyCenter::instance()->parseProtocolFailed(client_session, packet_stream->opcode());
	//	GATELog(LM_ERROR, ACE_TEXT("Failed to parse check role name package.\n"));
	//}
	return result;
}

int RouteClientPackage::handleRoleMake(PacketStream * packet_stream)
{
	int result = -1;
	////ClientSession * client_session = (ClientSession *)packet_stream->getEventHandler();
	//GET_ROLE_CLIENT_SESSION(-1);
	//typed::protocol::cmsg_role_make role_make;
	////if (role_make.ParseFromString(std::string(packet_stream->pe_body(), packet_stream->body_size())))
	//if (packet_stream->parse_protocol(role_make))
	//{
	//	int32 error_number = 0;
	//	if (ManageDatabase::instance()->checkRoleName(role_make.name(), error_number))
	//	{
	//		int role_number = 0;
	//		if (ManageDatabase::instance()->getAccountRoleNumber(client_session->getAccountID(), role_number))
	//		{
	//			if (role_number < m_max_role_number)
	//			{
	//				uint64 new_guid = 0;
	//				if (ManageDatabase::instance()->makeRole(role_make, client_session->getAccountID(), new_guid))
	//				{
	//					typed::protocol::smsg_role_make srm;
	//					srm.set_guid(new_guid);
	//					PacketStream * ps = new PacketStream(SMSG_ROLE_MAKE, new_guid, srm.SerializeAsString());
	//					client_session->handleOutputStream(ps);
	//					result = 0;
	//					delete packet_stream;
	//				}
	//				else
	//				{
	//					GATELogp(LM_ERROR, ACE_TEXT("Failed to make role from database, last error is <%d>\n"), ACE_OS::last_error());
	//					ClientStrategyCenter::instance()->makeRoleFailed(client_session);
	//				}
	//			}
	//			else
	//			{
	//				ClientStrategyCenter::instance()->reachMaxRoleNumber(client_session);
	//			}
	//		}
	//		else
	//		{
	//			ClientStrategyCenter::instance()->getAccountRoleNumberFailed(client_session);
	//		}
	//	}
	//	else
	//	{
	//		// todo 
	//		ClientStrategyCenter::instance()->checkRoleNameFailed(client_session);
	//	}
	//}
	//else
	//{
	//	// todo
	//	ClientStrategyCenter::instance()->parseProtocolFailed(client_session, packet_stream->opcode());
	//	GATELog(LM_ERROR, ACE_TEXT("Failed to parse role make package"));
	//}
	return result;
}

int RouteClientPackage::handleEnterMap(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	int result = 0;
	GET_ROLE_CLIENT_SESSION(-1);
	if (!client_session->canEnterGame())
	{
		GATE_LOG_ERROR(ACE_TEXT("client session can not enter game, guid is <%llu>, account state is <%x>\n"), packet_stream->guid(), client_session->getAccountState());
		return -1;
	}

	PacketStream * ps = NULL;
	typed::protocol::cmsg_player_enter_map enter_map;
	if (packet_stream->parse_protocol(enter_map))
	{
		cleanRoleAndLogoff(enter_map.guid());
		clientEnterMap(packet_stream->guid(), enter_map.guid(), true);
	}
	//else
	//{
	//	GATELog(LM_ERROR, ACE_TEXT("Failed to parse choose role package.\n"));
	//	ManageClientSessionCooler::instance()->putClientSession(client_session);
	//	result = -1;
	//}
	////client_session->handleOutputStream(ps);
	//return result;
	return 0;
}

int RouteClientPackage::handleCmsgLoopback(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	int result = -1;
	PacketStream * ps = NULL;
	//ClientSession * client_session = (ClientSession *)packet_stream->getEventHandler();
	GET_ROLE_CLIENT_SESSION(-1);
	typed::protocol::cmsg_loopback msg_test;
	//if (msg_test.ParseFromString(std::string(packet_stream->pe_body(), packet_stream->body_size())))
	if (packet_stream->parse_protocol(msg_test))
	{
		ACE_Time_Value current_time = ACE_OS::gettimeofday();
		ACE_UINT64 usec=0;
		current_time.to_usec(usec);
		msg_test.set_gs_run_elapse(usec);
		ps = new PacketStream(packet_stream->opcode(), packet_stream->guid(), msg_test.SerializeAsString());
		//ps->setEventHandler(client_session);
		ps->setRoleLocation(client_session->getRoleLocation());
		ps->setLineNO(client_session->getLineNo());
		RouteGSPacket::instance()->handleClientPackage(ps, client_session->getRoleLocation());
		
		result = 0;
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to parse Msg Test packet, last error is <%d>\n"), ACE_OS::last_error());
		result = -1;
	}
	return result;
}

int RouteClientPackage::handleRoleBeat(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	int result = 0;
	//ClientSession * client_session = (ClientSession *)packet_stream->getEventHandler();
	GET_ROLE_CLIENT_SESSION(-1);
	client_session->updateLastBeatTime();
	
	return result;
}

int RouteClientPackage::handleRoleLogout(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	int result = -1;
	ClientSession * client_session = this->getRoleClientSession(packet_stream->guid());
	if (NULL != client_session)
	{
		//if (client_session->getValidation())
		//{
		//	if ((!client_session->getWaitLogoutFromGS()) || client_session->getNotFindGS())
		//	{
		//		ManageClientValidation::instance()->accountLogout(client_session);
		//	}
		//	else
		//	{
		//		ManageClientValidation::instance()->waitFromGSLogoutMsg(client_session->getAccountID(), client_session->getLineNo(), client_session->getRoleGuid(), client_session->getRoleLocation());
		//	}
		//}

		//client_session->setLogout();
		GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
		ManageClientSessionCooler::instance()->putClientSession(client_session);
	}

	GATE_LOG_INFO(ACE_TEXT("Client logout gs, guid is <%llu>\n"), packet_stream->guid());
	return result;
}

int RouteClientPackage::handleRoleLeaveMap(PacketStream * packet_stream)
{
	int result = 0;
	{
		ClientSession * client_session = this->getRoleClientSession(packet_stream->guid());
		if (NULL != client_session)
		{
			client_session->leaveGame();
			client_session->setClientLoginGS(false);
			client_session->setWaitLogoutFromGS(false);
		}
	}

	GATE_LOG_INFO(ACE_TEXT("Client leave map, guid is <%llu>\n"), packet_stream->guid());
	return result;
}

int RouteClientPackage::handleClientClose(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	int result = -1;
	GET_ROLE_CLIENT_SESSION(-1);
	//if (client_session->getClientLogouted())
	//{
	//	return result;
	//}

	//client_session->setClientClose(true);

	//if (client_session->getValidation())
	//{
	//	if ((!client_session->getWaitLogoutFromGS()) || client_session->getNotFindGS())
	//	{
	//		ManageClientValidation::instance()->accountLogout(client_session);
	//	}
	//	else
	//	{
	//		ManageClientValidation::instance()->waitFromGSLogoutMsg(client_session->getAccountID(), client_session->getLineNo(), client_session->getRoleGuid(), client_session->getRoleLocation());
	//	}
	//}

	GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
	ManageClientSessionCooler::instance()->putClientSession(client_session);
	return result;	
}

int RouteClientPackage::handleClientReadError(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	GATE_LOG_ERROR("client read error, start to clean role <%llu>\n", packet_stream->guid());
	GET_ROLE_CLIENT_SESSION(-1);
	GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
	ManageClientSessionCooler::instance()->putClientSession(client_session);
	return 0;
}

int RouteClientPackage::handleClientWriteError(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	GATE_LOG_ERROR("client write error, start to clean role <%llu>\n", packet_stream->guid());
	GET_ROLE_CLIENT_SESSION(-1);
	GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
	ManageClientSessionCooler::instance()->putClientSession(client_session);

	return 0;
}

int RouteClientPackage::handleClientWobbleError(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	GATE_LOG_ERROR("client output wobble error, start to clean role <%llu>\n", packet_stream->guid());
	GET_ROLE_CLIENT_SESSION(-1);
	GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
	ManageClientSessionCooler::instance()->putClientSession(client_session);
	return 0;
}

int RouteClientPackage::handleClientBeatTimeout(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	GATE_LOG_ERROR("client beat timeout, start to clean role <%llu>\n", packet_stream->guid());
	GET_ROLE_CLIENT_SESSION(-1);
	GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
	ManageClientSessionCooler::instance()->putClientSession(client_session);
	return 0;
}

bool RouteClientPackage::handlePlayerLeaveMap(PacketStream * packet_stream)
{
	return true;

	//GET_ROLE_CLIENT_SESSION(false);
	//if (client_session->preOnlineStage())
	//{
	//	client_session->clearPreOnline();
	//	clientEnterMap(packet_stream->guid(), client_session->getEnterRoleGuid(), false);
	//	delete packet_stream;
	//	return true;
	//}

	//return false;
}

int RouteClientPackage::handleSmsgPlayerMake(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	GET_ROLE_CLIENT_SESSION(-1);

	typed::protocol::smsg_player_make player_make;
	if (packet_stream->parse_protocol(player_make))
	{
		client_session->acquireRoleList();

		client_session->playerLineInfo(player_make.guid(), player_make.map_id(), player_make.line_id());

		client_session->handleOutputStream(auto_ps.release());
		//clientEnterMap(packet_stream->guid(), player_make.guid());
	}

	return 0;
}

int RouteClientPackage::handleSmsgRequestPlayerList(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	GET_ROLE_CLIENT_SESSION(-1);
	int line_id = client_session->getLineNo();

	typed::protocol::smsg_request_player_list player_list;

	typed::protocol::smsg_request_player_list replay_client_player_list;

	typed::protocol::smsg_request_one_player * available_player = NULL;

	if (packet_stream->parse_protocol(player_list))
	{
		client_session->acquireRoleList();

		for (int i = 0; i < player_list.player_list_size(); ++i)
		{
			typed::protocol::smsg_request_one_player * one_player = player_list.mutable_player_list(i);
			if (NULL == available_player)
			{
				available_player = replay_client_player_list.add_player_list();
			}

			if (one_player->line_id() == line_id)
			{
				available_player->CopyFrom(*one_player);
				break;
			}

			if (0 == i)
			{
				available_player->CopyFrom(*one_player);
			}
		}

		if (available_player != NULL)
		{
			uint32 map_id = ManageReduplication::instance()->getMapId(available_player->map_id());

			if (available_player->map_id() > 99999)
			{
				available_player->set_map_id(available_player->map_id() / 1000);
			}
			else
			{
				available_player->set_map_id(map_id);
			}

			client_session->playerLineInfo(available_player->guid(), map_id, available_player->line_id());
		}

		//if (player_list.player_list_size() == 1)
		//{
		//	typed::protocol::smsg_request_one_player * one_player = player_list.mutable_player_list(0);
		//	clientEnterMap(packet_stream->guid(), one_player->guid());
		//}
		//else
		{
			replay_client_player_list.set_player_num(replay_client_player_list.player_list_size());
			PacketStream * ps = new PacketStream(packet_stream->opcode(), packet_stream->guid(), replay_client_player_list.SerializeAsString());
			client_session->handleOutputStream(ps);
		}
	}

	return 0;
}

int RouteClientPackage::handlePlayerJumpmap(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	int result = -1;
	
	ClientSession * client_session = this->getRoleClientSession(packet_stream->guid());

	// do not delete packet_stream
	typed::protocol::smsg_player_jump_map player_jumpmap;
	if (packet_stream->parse_protocol(player_jumpmap))
	{
		uint64 role_localtion = MAKE_MAP_GUID(player_jumpmap.line_no(), 0, player_jumpmap.target_map_id());

		if (NULL != client_session)
		{
			GSSession * dest_gssession = RouteGSPacket::instance()->getGSSession(role_localtion);
			if (NULL != dest_gssession)
			{
				if (NULL != client_session)
				{
					client_session->setRoleLocation(role_localtion);

					typed::protocol::gmsg_player_jump_map gmsg_jump_map;
					gmsg_jump_map.set_map_id(player_jumpmap.target_map_id());
					gmsg_jump_map.set_line_no(player_jumpmap.line_no());

					PacketStream * ps = new PacketStream(GMSG_PLAYER_JUMP_MAP, packet_stream->guid(), gmsg_jump_map.SerializeAsString());
					dest_gssession->handleOutputStream(ps);

					client_session->handleOutputStream(auto_ps.release());
				}
				else
				{
					// todo
				}

				result = 0;
			}
			else
			{
				// not find target map
				GATE_LOG_ERROR(ACE_TEXT("Failed to find the target GS with map instance : <%d>\n"), player_jumpmap.target_map_id());
				result = -1;
			}
		}
		else
		{
			GSSession * dest_gssession = RouteGSPacket::instance()->getGSSession(role_localtion);
			if (NULL != dest_gssession)
			{
				typed::protocol::gmsg_player_jump_map gmsg_jump_map;
				gmsg_jump_map.set_map_id(player_jumpmap.target_map_id());
				gmsg_jump_map.set_line_no(player_jumpmap.line_no());
				{
					PacketStream * ps = new PacketStream(GMSG_PLAYER_JUMP_MAP, packet_stream->guid(), gmsg_jump_map.SerializeAsString());
					dest_gssession->handleOutputStream(ps);
				}


				typed::protocol::cmsg_player_leave_map leave_map;
				leave_map.set_leave_type(0);
				{
					PacketStream * ps = new PacketStream(CMSG_PLAYER_LEAVE_MAP, packet_stream->guid(), leave_map.SerializeAsString());
					dest_gssession->handleOutputStream(ps);
				}

			}
		}
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to parse smsg player jumpmap, last error is <%d>\n"), ACE_OS::last_error());
		result = -1;
	}
	return result;
}

int RouteClientPackage::handleSmsgLoopback(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	int result = -1;
	PacketStream * ps = NULL;
	//ClientSession * client_session = (ClientSession *)packet_stream->getEventHandler();
	GET_ROLE_CLIENT_SESSION(-1);
	typed::protocol::cmsg_loopback msg_test;
	//if (msg_test.ParseFromString(std::string(packet_stream->pe_body(), packet_stream->body_size())))
	if (packet_stream->parse_protocol(msg_test))
	{
		ACE_Time_Value current_time = ACE_OS::gettimeofday();
		ACE_UINT64 usec=0;
		current_time.to_usec(usec);
		msg_test.set_gs_run_elapse(usec - msg_test.gs_run_elapse());
		ps = new PacketStream(packet_stream->opcode(), packet_stream->guid(), msg_test.SerializeAsString());
		//ps->setEventHandler(client_session);
		ps->setRoleLocation(client_session->getRoleLocation());
		ps->setLineNO(client_session->getLineNo());
		client_session->handleOutputStream(ps);

		result = 0;
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to parse Msg Test packet, last error is <%d>\n"), ACE_OS::last_error());
		result = -1;
	}
	return result;
}

int RouteClientPackage::handlePlayerKickoff(PacketStream * packet_stream)
{
	std::auto_ptr<PacketStream> auto_ps(packet_stream);

	GET_ROLE_CLIENT_SESSION(-1);

	ManageClientSessionCooler::instance()->putClientSession(client_session);
	return 0;
}

void RouteClientPackage::handleWobbleStream(Uint64Set_t & wobble_guid)
{
	Uint64Set_t guids;
	guids = wobble_guid;
	wobble_guid.clear();

	ClientSession * client_session = NULL;

	int handle_result = 0;

	for (Uint64Set_t::iterator it = guids.begin(); it != guids.end(); ++it)
	{
		client_session = getRoleClientSession(*it);
		if (NULL != client_session)
		{
			handle_result = client_session->wt_stream();
			if (1 == handle_result)
			{
				client_session->addWobbleTime();
				if (client_session->getWobbleTime() == 30)
				{
					PacketStream * ps = new PacketStream(GMSG_CLIENT_WOBBLE, *it, "");
					client_session->putInputPacket(ps);
				}
				else
				{
					client_session->clearWobbleTime();
					wobble_guid.insert(*it);
				}
			}
		}
	}
}

int RouteClientPackage::handleOutputPackage(PacketStream * packet_stream)
{
	// todo
	int op_code = packet_stream->opcode();
	int handle_result = 0;

	if (SMSG_PLAYER_MAKE == op_code)
	{
		handleSmsgPlayerMake(packet_stream);
	}
	else if (SMSG_REQUEST_PLAYER_LIST == op_code)
	{
		handleSmsgRequestPlayerList(packet_stream);
	}
	else if (SMSG_PLAYER_JUMP_MAP == op_code)
	{
		handlePlayerJumpmap(packet_stream);
	}
	else if (CMSG_LOOPBACK == op_code)
	{
		handleSmsgLoopback(packet_stream);
	}
	else if (GMSG_PLAYER_KICK_OFF == op_code)
	{
		handlePlayerKickoff(packet_stream);
	}
	else
	{
		if (SMSG_PLAYER_LEAVE_MAP == op_code)
		{
			handlePlayerLeaveMap(packet_stream);

			return 0;
		}

		ClientSession * client_session = getRoleClientSession(packet_stream->guid());
		if (NULL != client_session)
		{
			handle_result = client_session->handleOutputStream(packet_stream);
		}
		else
		{
			// error
			delete packet_stream;
		}
	}

	return handle_result;
}

//void RouteClientPackage::accountOnline(const string & account_id, ClientSession * client_session)
//{
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_account_session_mutex, );
//	ClientAccountSessionMap::iterator it = m_client_account_session.find(account_id);
//	if (it != m_client_account_session.end())
//	{
//		if (!lastOnlineTimeout(it->second))
//		{
//			it->second->roleOnlineAgain();
//
//			it->second->checkLastOnlineTime();
//
//			if (it->second->enterGameState())
//			{
//				client_session->setRealRoleGuid(it->second->getRoleGuid());
//				client_session->setRoleLocation(it->second->getRoleLocation());
//				client_session->setClientLoginGS(true);
//				client_session->setWaitLogoutFromGS(true);
//
//				if (it->second->enterGameState())
//				{
//					client_session->preOnline();
//					if (client_session->getPreRoleLocation() != 0)
//					{
//						client_session->setPreRoleLocation(it->second->getPreRoleLocation());
//					}
//					else
//					{
//						client_session->setPreRoleLocation(it->second->getRoleLocation());
//					}
//				}
//
//				client_session->enterGame();
//			}
//
//			GATE_LOG_INFO("player online again, account is <%s>, role location is <%llu>\n", account_id.c_str(), client_session->getRoleLocation());
//
//
//			GATE_LOG_DEBUG("start to cooler cs : <%s>\n", __FUNCTION__);
//			ManageClientSessionCooler::instance()->putClientSession(it->second);
//			this->removeRoleClientSession(it->second->getRoleGuid(), it->second);
//		}
//		//m_client_account_session.erase(it);
//	}
//
//	m_client_account_session[account_id] = client_session;
//}

//void RouteClientPackage::accountOffline(const string & account_id)
//{
//	//if (0 == account_id)
//	//{
//	//	return ;
//	//}
//
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_account_session_mutex, );
//	ClientAccountSessionMap::iterator it = m_client_account_session.find(account_id);
//	if (it != m_client_account_session.end())
//	{
//		m_client_account_session.erase(it);
//	}
//}

void RouteClientPackage::cleanPlayerInfo(const string & player_account, uint64 player_guid)
{
	cleanRoleInfo(player_guid);
	//cleanAccountInfo(player_account);
}

void RouteClientPackage::roleLogoffSuccess(uint64 guid)
{
	ClientSession * client_session = this->getRoleClientSession(guid);
	if (NULL == client_session)
	{
		return ;
	}

	if (client_session->enterGameState())
	{
		// enter game
		typed::protocol::cmsg_player_enter_map enter_map;
		enter_map.set_guid(guid);
		enter_map.set_map_id(client_session->getRoleLocation() & 0x0000000000FFFFFF);

		PacketStream * ps = new PacketStream(CMSG_PLAYER_ENTER_MAP, guid, enter_map.SerializeAsString());
		RouteGSPacket::instance()->handleClientPackage(ps, client_session->getRoleLocation());
	}
}

void RouteClientPackage::changeClientRoute(ClientSession * cs_session, uint64 temp_guid, uint64 role_guid)
{
	cs_session->setRoleGuid(role_guid);

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_map_mutex, );
	// remove temp role guid route
	{
		ClientSessionRoleMap::iterator it = m_client_session_map_id_map.find(temp_guid);
		if (it != m_client_session_map_id_map.end())
		{
			GATE_LOG_DEBUG("start to cooler cs : <%x> <%s>\n", it->second, __FUNCTION__);
			m_client_session_map_id_map.erase(it);
		}
	}

	// remove role guid
	{
		ClientSessionRoleMap::iterator it = m_client_session_map_id_map.find(role_guid);
		if (it != m_client_session_map_id_map.end())
		{
			// error
			GATE_LOG_ERROR("changeClientRoute, but find the role guid in client map, guid <%llu>\n", role_guid);
			ManageClientSessionCooler::instance()->putClientSession(it->second);
		}
		else
		{
			m_client_session_map_id_map[role_guid] = cs_session;
		}
	}
}

void RouteClientPackage::clientEnterMap(uint64 client_guid, uint64 role_guid, bool make_route)
{
	ClientSession * client_session = this->getRoleClientSession(client_guid);
		if (NULL == client_session)
			return ;

	AccountRoleInfo * role_info = client_session->getAccountRoleInfo(role_guid);
	if (NULL != role_info)
	{
		if (make_route)
		{
			changeClientRoute(client_session, client_session->getRoleGuid(), role_guid);
		}

		client_session->setLineNo(role_info->line_id);
		uint32 map_id = role_info->map_id;
		uint64 role_localtion = MAKE_MAP_GUID(role_info->line_id, 0, map_id);
		//RouteGSPacket::instance()->assignClientThreadPacketListInfo(packet_stream->guid());

		client_session->setRoleLocation(role_localtion);
		client_session->setClientLoginGS(true);
		client_session->setWaitLogoutFromGS(true);

		typed::protocol::cmsg_player_enter_map enter_map;
		enter_map.set_guid(role_info->guid);
		enter_map.set_map_id(role_localtion);
		enter_map.set_line_id(role_info->line_id);

		PacketStream * ps = new PacketStream(CMSG_PLAYER_ENTER_MAP, role_guid, enter_map.SerializeAsString());

		int hr = RouteGSPacket::instance()->handleClientPackage(ps, role_localtion);
		if (-1 == hr)
		{
			// not find gs
			notFindGS(client_guid);
		}
		else
		{
			client_session->enterGame();
		}

		GATE_LOG_INFO(ACE_TEXT("Client login gs, guid is <%llu>, point is <%x>\n"), client_guid, client_session);
	}
	else
	{
		GATE_LOG_ERROR("failed to get role info by guid <%llu>\n", client_guid);
		ManageClientSessionCooler::instance()->putClientSession(client_session);
	}
}

bool RouteClientPackage::lastOnlineTimeout(ClientSession * cs_session)
{
	uint64 last_online_time = cs_session->getLastOnlineTime();
	if (last_online_time > 0)
	{
		uint64 curr_time = 0;
		ACE_OS::gettimeofday().to_usec(curr_time);

		if ((curr_time - last_online_time) >= 30 * 1000000)
		{
			return true;
		}
	}

	return false;
}

void RouteClientPackage::cleanRoleInfo(uint64 player_guid)
{
	ClientSession * client_session = NULL;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_map_mutex, );
		ClientSessionRoleMap::iterator it = m_client_session_map_id_map.find(player_guid);
		if (it != m_client_session_map_id_map.end())
		{
			client_session = it->second;

			m_client_session_map_id_map.erase(it);
		}
	}

	if (NULL != client_session)
	{
		roleLeaveMap(player_guid, client_session->getRoleLocation());

		client_session->leaveGame();

		ManageClientSessionCooler::instance()->putClientSession(client_session);
	}
}

void RouteClientPackage::cleanRoleAndLogoff(uint64 role_guid)
{
	ClientSession * client_session = NULL;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_session_map_mutex, );
	ClientSessionRoleMap::iterator it = m_client_session_map_id_map.find(role_guid);
	if (it != m_client_session_map_id_map.end())
	{
		client_session = it->second;
		m_client_session_map_id_map.erase(it);
	}

	if (NULL != client_session)
	{
		roleLeaveMap(role_guid, client_session->getRoleLocation());

		client_session->leaveGame();

		ManageClientSessionCooler::instance()->putClientSession(client_session);
	}
}

void RouteClientPackage::roleLeaveMap(uint64 role_guid, uint64 map_id)
{
	typed::protocol::cmsg_player_leave_map leave_map;
	leave_map.set_leave_type(0);
	PacketStream * ps = new PacketStream(CMSG_PLAYER_LEAVE_MAP, role_guid, leave_map.SerializeAsString());

	RouteGSPacket::instance()->handleClientPackage(ps, map_id);
}

//void RouteClientPackage::cleanAccountInfo(const string & player_account)
//{
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_account_session_mutex, );
//	ClientAccountSessionMap::iterator it = m_client_account_session.find(player_account);
//	if (it != m_client_account_session.end())
//	{
//		m_client_account_session.erase(it);
//	}
//}