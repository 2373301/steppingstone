
#include <ace/OS.h>
#include "RouteGSPacket.h"
#include "ManageConfig.h"
#include "RouteClientPackage.h"
#include "opcode.h"
#include "protocol/msg_gate.pb.h"
#include "protocol/msg_player.pb.h"
#include "ClientStrategyCenter.h"
#include "ManageGSStream.h"
#include "ManageTranscriptStream.h"
//#include "ManageTranscriptCondition.h"
#include "ManageGSSessionCooler.h"
#include "ManageClientValidation.h"
#include "ManageRoleGsState.h"

#define MAKE_GUID(type, seed, id) (((uint64(type) << 56) & 0xFF00000000000000) | ((uint64(seed) << 48) & 0x00FF000000000000) | (uint64(id) & 0x0000FFFFFFFFFFFF))

#define MAKE_MAP_GUID(line_no, repduplicate_index, template_id)			MAKE_GUID(0xC, 15, (uint64)0 << 40 | repduplicate_index << 24 | template_id)

//#include "protocol/msg_test.pb.h"

//#define MAKE_GUID(type, ggen, id) (((uint64(type) << 56) & 0xFF00000000000000) | ((uint64(ggen) << 48) & 0x00FF000000000000) | (uint64(id) & 0x0000FFFFFFFFFFFF))
//
//#define MAKE_MAP_GUID(line, id) MAKE_GUID(0x0C, 15, ((((uint64)(line) << 40) | uint64(id))))

RouteGSPacket::RouteGSPacket()
: m_stop(false)
, m_wait(true)
, m_thread_packet_stream_index(0)
{

}

RouteGSPacket::~RouteGSPacket()
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_mutex, );
	{
		for (PackageStreamList::iterator it = m_packet_stream_list.begin(); it != m_packet_stream_list.end(); )
		{
			delete *it;
		}
		m_packet_stream_list.clear();
	}

	for (ThreadPacketListInfoList::iterator it = m_thread_packet_list_info_list.begin(); it != m_thread_packet_list_info_list.end(); ++it)
	{
		//for (ThreadPacketListInfo::PackageStreamList::iterator it_sub = (*it)->thread_packet_stream_list->begin(); it_sub != (*it)->thread_packet_stream_list->end(); ++it_sub)
		//{
		//	delete *it_sub;
		//}
		//delete (*it);
	}
}

int RouteGSPacket::svc(void)
{
	FUNCTIONTRACE(RouteGSPacket::svc);
	REPORT_THREAD_INFO("RouteGSPacket::svc", ACE_OS::thr_self());

	ThreadPacketListInfo * thread_pli = new ThreadPacketListInfo();

	this->collectThreadPacketStreamList(thread_pli);

//	try
	{
		PackageStreamList packet_stream_list;
		while(!m_stop)
		{
			ACE_OS::sleep(1);
		}
	}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in RouteGSPacket::svc, last error is <%d>\n"), ACE_OS::last_error());
	//	return 2;
	//}

	GATE_LOG_INFO(ACE_TEXT("Exit thread RouteGSPacket::svc, last error is <%d>\n"), ACE_OS::last_error());
	return 0;
}

int RouteGSPacket::init(int argc, ACE_TCHAR * argv[])
{
	int r = this->activate(THR_JOINABLE, ManageConfig::instance()->getGSCfg().handle_packet_thread_pool_number);
	if (r < 0)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call RouteGSPacket::init, the last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init RouteGSPacket.\n"));
		return 0;
	}
}

int RouteGSPacket::fini()
{
	m_stop = true;
	int result = 0;

	return result;
}

int RouteGSPacket::wait()
{
	m_wait = true;
	super::wait();
	return 0;
}

void RouteGSPacket::putPackage(PacketStream * packet_stream)
{
	int op_code = packet_stream->opcode();
	if (GCMSG_REGISTER == op_code)
	{
		handleRegisterMapID(packet_stream);
	}
	else if (SMSG_MEMBER_CHARGE_CONFIRM == op_code)
	{
		// todo
		handleMemberChargeConfirm(packet_stream);
	}
	else if (SMSG_MEMBER_GIVE_GIFT == op_code)
	{
		handleMemberGiveGift(packet_stream);
	}
	else if(STOG_QUERY_QQ_VIP_INFO == op_code 
			|| STOG_QUERY_QQ_APP_FRIENDS == op_code)
	{
		GATE_LOG_DEBUG("qq msg opcode is <%d>!\n", op_code);
		//ManageQQRequest::instance()->inputPacket(packet_stream);
	}
	else
	{
		if (SMSG_CREATE_SELF == packet_stream->opcode())
		{
			ManageRoleGsState::instance()->enterGameSuccess(packet_stream->guid());
		}
		else if (SMSG_PLAYER_JUMP_MAP == packet_stream->opcode())
		{
			typed::protocol::smsg_player_jump_map spj_map;
			if (packet_stream->parse_protocol(spj_map))
			{
				uint64 map_id = MAKE_MAP_GUID(0, 0, spj_map.target_map_id());

				ManageRoleGsState::instance()->jumpMapSuccess(packet_stream->guid(), map_id);
			}
		}
		else if (SMSG_PLAYER_LEAVE_MAP == op_code)
		{
			ManageRoleGsState::instance()->leaveGameSuccess(packet_stream->guid());
		}

		RouteClientPackage::instance()->putOutputPackage(packet_stream);
	}
}

int RouteGSPacket::handleClientPackage(PacketStream * packet_stream, uint64 map_id)
{
	std::auto_ptr<PacketStream> auto_pack(packet_stream);

	// todo
	GSSession * gs_session = NULL;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gs_session_map_mutex, -2);
		gs_session = getGSSession(map_id);
	}

	if (NULL != gs_session)
	{
		if (packet_stream->opcode() == CMSG_PLAYER_ENTER_MAP)
		{
			if (!ManageRoleGsState::instance()->canEnterGame(packet_stream->guid()))
			{
				GATE_LOG_INFO(ACE_TEXT("Client login gs, notify gs, can not enter, guid is <%llu>, map id is <%llu>\n"), packet_stream->guid(), map_id);
				ManageRoleGsState::instance()->enterGame(packet_stream->guid(), map_id);
				return 0;
			}
			else
			{
				GATE_LOG_INFO(ACE_TEXT("Client login gs, notify gs, can enter, guid is <%llu>, map id is <%llu>\n"), packet_stream->guid(), map_id);
				ManageRoleGsState::instance()->enterGame(packet_stream->guid(), map_id);
			}
		}
		else if (packet_stream->opcode() == CMSG_PLAYER_LEAVE_MAP)
		{
			if (!ManageRoleGsState::instance()->canLeaveGame(packet_stream->guid()))
			{
				GATE_LOG_INFO(ACE_TEXT("Client logout gs, notify gs, can not leave, guid is <%llu>, map id is <%llu>\n"), packet_stream->guid(), map_id);
				return 0;
			}
			else
			{
				GATE_LOG_INFO(ACE_TEXT("Client logout gs, notify gs, can leave, guid is <%llu>, map id is <%llu>\n"), packet_stream->guid(), map_id);
				ManageRoleGsState::instance()->leaveGame(packet_stream->guid(), map_id);
			}
		}
		gs_session->handleOutputStream(auto_pack.release());
		return 0;
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get GS Session by map id, the map id is <%llu>, opcode is <%d>\n"), map_id, packet_stream->opcode());
		//ClientSession * client_session = RouteClientPackage::instance()->getRoleClientSession(packet_stream->guid());
		//if (NULL != client_session)
		//{
		//	ClientStrategyCenter::instance()->clientNotFindGS(client_session);
		//}
		return -1;
	}
}

//void RouteGSPacket::handleMemberPackage(PacketStream * packet_stream, )
//{
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gs_session_map_mutex, );
//	if (m_gs_session_map_id_map.size() > 0)
//	{
//		
//	}
//}
//
void RouteGSPacket::registerGSMapID(uint64 map_id, GSSession * gs_session)
{
	bool new_session = true;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gs_session_map_mutex, );
		for (GSSessionMapIDMap::iterator it = m_gs_session_map_id_map.begin(); it != m_gs_session_map_id_map.end(); ++it)
		{
			if (it->second == gs_session)
			{
				new_session = false;
				break;
			}
		}
		m_gs_session_map_id_map[map_id] = gs_session;
		gs_session->setMapID(map_id);
	}

	if (new_session)
	{
		GATE_LOG_INFO(ACE_TEXT("Getting new gs session, map id is <%llu>\n"), map_id);

		if (gs_session->reactor() != NULL)
		{
			gs_session->reactor()->remove_handler(gs_session, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
			gs_session->reactor(NULL);
		}

		ManageGSStream::instance()->handleStream(gs_session, ACE_Event_Handler::READ_MASK);
		ManageGSStream::instance()->handleStream(gs_session, ACE_Event_Handler::WRITE_MASK);
		ManageGSStream::instance()->activeThread(2);
	}
}

void RouteGSPacket::registerTranscriptMapID(uint64 map_id, GSSession * gs_session)
{
	registerGSMapID(map_id, gs_session);
}

GSSession * RouteGSPacket::getGSSession(uint64 map_id)
{
	//if (m_gs_session_map_id_map.size() == 0)
	//{
	//	return NULL;
	//}
	// todo 
//	int index  = map_id % m_gs_session_map_id_map.size();
	//GSSessionMapIDMap::iterator get_it = m_gs_session_map_id_map.begin();
	//for (int i = 0; i < index; ++i)
	//{
	//	++get_it;
	//}
	//return get_it->second;

	GSSessionMapIDMap::iterator it = m_gs_session_map_id_map.find(map_id);
	if (it != m_gs_session_map_id_map.end())
	{
		return it->second;
	}
	else
	{

		// todo
		//GATELogp(LM_ERROR, ACE_TEXT("Failed to get gs session, the map id is <%s>\n"), boost::lexical_cast<string>(map_id).c_str());
		return NULL;
	}
}

void RouteGSPacket::removeGSMapID(uint64 map_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gs_session_map_mutex, );
	GSSessionMapIDMap::iterator it = m_gs_session_map_id_map.find(map_id);
	if (it != m_gs_session_map_id_map.end())
	{
		m_gs_session_map_id_map.erase(it);
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get gs session in RouteGSPacket::removeGSMapID, the map id is <%llu>\n"), map_id);
	}
}

void RouteGSPacket::assignClientThreadPacketListInfo(uint64 role_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_belong_thread_packet_mutex, );
	ClientBelongThreadPacketMap::iterator it = m_client_belong_thread_packet_map.find(role_guid);
	if (m_client_belong_thread_packet_map.end() == it)
	{
		m_thread_packet_stream_index = ++m_thread_packet_stream_index % m_thread_packet_list_info_list.size();
		m_client_belong_thread_packet_map[role_guid] = m_thread_packet_list_info_list[m_thread_packet_stream_index];
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("assign client thread packet again, role guid is <%llu>\n"), role_guid);
	}
}

void RouteGSPacket::removeClientThreadPacketListInfo(uint64 role_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_client_belong_thread_packet_mutex, );
	ClientBelongThreadPacketMap::iterator it = m_client_belong_thread_packet_map.find(role_guid);
	if (it != m_client_belong_thread_packet_map.end())
	{
		m_client_belong_thread_packet_map.erase(it);
	}
}

//int RouteGSPacket::handlePackage(PacketStream * packet_stream)
//{
//	int result = 0;
//	try
//	{
//		int op_code = packet_stream->opcode();
//		GATE_LOG_DEBUG(ACE_TEXT("handle gs packet opcode is :  <%d>, guid is <%llu>, body size is <%d>\n"), op_code, packet_stream->guid(), packet_stream->body_size());
//		if (GCMSG_REGISTER == op_code)
//		{
//			result = handleRegisterMapID(packet_stream);
//		}
//		else if (CMSG_LOOPBACK == op_code)
//		{
//			result = handleMsgTest(packet_stream);
//		}
//		else if (SMSG_PLAYER_JUMP_MAP == op_code)
//		{
//			result = handlePlayerJumpmap(packet_stream);
//		}
//		//else if (SMSG_KICK_PLAYER == op_code)
//		//{
//		//	result = handleKickPlayer(packet_stream);
//		//}
//		//else if (SMSG_PLAYER_LEAVE_WORLD == op_code)
//		//{
//		//	result = handlePlayerLeaveWorld(packet_stream);
//		//}
//		//else if (SMSG_PLAYER_SWITCH_MAP == op_code)
//		//{
//		//	result = handlePlayerSwitchMap(packet_stream);
//		//}
//		else
//		{
//			//if (packet_stream->opcode() == SMSG_CREATE_SELF)
//			//{
//			//	std::string str_guid = boost::lexical_cast<std::string>(packet_stream->guid());
//			//	GATELogp(LM_INFO, ACE_TEXT("Recv GS Player Create Self Message, guid is <%s>\n"), str_guid.c_str());
//			//}
//			RouteClientPackage::instance()->putOutputPackage(packet_stream);
//			result = 0;
//		}
//
//		if (0 != result)
//		{
//			delete packet_stream;
//		}
//	}
//	catch (...)
//	{
//		GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in RouteGSPacket::handlePackage, lase error is <%d>\n"), ACE_OS::last_error());
//		result = 1;
//	}
//	return result;
//}

int RouteGSPacket::handleRegisterMapID(PacketStream * packet_stream)
{
	int result = -1;
	std::auto_ptr<PacketStream> auto_ps(packet_stream);
	typed::protocol::gcmsg_register input_proto;
	if (!packet_stream->parse_protocol(input_proto))
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call ParseFromString in RouteGSPacket::handleRegisterMapID, the last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}

	GSSession * gs_session = (GSSession * )packet_stream->getEventHandler();

	if (existGS(input_proto.zone_id()))
	{
		GATE_LOG_ERROR(ACE_TEXT("the gs already exists, map id is <%llu>\n"), input_proto.zone_id());

		if (gs_session->reactor() != NULL)
		{
			gs_session->reactor()->remove_handler(gs_session, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
			gs_session->reactor(NULL);
		}

		ManageGSSessionCooler::instance()->putGSSession(gs_session);
		return 0;
	}

	if (input_proto.zone_type() == typed::protocol::gcmsg_register_zone_t_zt_zone)
	{
		registerGSMapID(input_proto.zone_id(), gs_session);

		result = 0;
	}
	else if (input_proto.zone_type() == typed::protocol::gcmsg_register_zone_t_zt_transcript)
	{
		// ¸±±¾
		registerTranscriptMapID(input_proto.zone_id(), gs_session);
		gs_session->setTranscript(true);

		if (gs_session->reactor() != NULL)
		{
			gs_session->reactor()->remove_handler(gs_session, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
			gs_session->reactor(NULL);
		}

		ManageTranscriptStream::instance()->handleTranscriptStream(gs_session);

		result = 0;
	}
	else
	{
		GATE_LOG_ERROR(ACE_TEXT("Unknown map_type of gcmsg_register, type is <%d>\n"), (int)input_proto.zone_type());
	}
	return result;
}

int RouteGSPacket::handleMemberChargeConfirm(PacketStream * packet_stream)
{
	ManageClientValidation::instance()->handleMemberChargeConfirm(packet_stream);

	return 0;
}

int RouteGSPacket::handleMemberGiveGift(PacketStream * packet_stream)
{
	ManageClientValidation::instance()->handleMemberGiveGiftReturn(packet_stream);
	return 0;
}

//int RouteGSPacket::handleMsgTest(PacketStream * packet_stream)
//{
//	int result = -1;
//	PacketStream * ps = NULL;
//	typed::protocol::cmsg_loopback msg_test;
//	if (packet_stream->parse_protocol(msg_test))
//	{
//		ACE_Time_Value current_time = ACE_OS::gettimeofday();
//		ACE_UINT64 usec=0;
//		current_time.to_usec(usec);
//		msg_test.set_gs_run_elapse(usec - msg_test.gs_run_elapse());
//		ps = new PacketStream(packet_stream->opcode(), packet_stream->guid(), msg_test.SerializeAsString());
//		RouteClientPackage::instance()->putOutputPackage(ps);
//		delete packet_stream;
//		result = 0;
//	}
//	else
//	{
//		GATE_LOG_ERROR(ACE_TEXT("Failed to parse Msg Test packet, last error is <%d>\n"), ACE_OS::last_error());
//	}
//	return result;
//}
//
//int RouteGSPacket::handlePlayerJumpmap(PacketStream * packet_stream)
//{
//	RouteClientPackage::instance()->putInputPackage(packet_stream);
//	return 0;
//}
//
//int RouteGSPacket::handleKickPlayer(PacketStream * packet_stream)
//{
//	ClientStrategyCenter::instance()->kickPlayer(packet_stream->guid());
//	delete packet_stream;
//	return 0;
//}
//
//int RouteGSPacket::handlePlayerLeaveWorld(PacketStream * packet_stream)
//{
//	typed::protocol::cmsg_player_leave_map leave_map;
//	if (packet_stream->parse_protocol(leave_map))
//	{
//		ClientStrategyCenter::instance()->playerLogoutFromGS(packet_stream->guid(), (int)leave_map.leave_type());
//	}
//	else
//	{
//		GATE_LOG_ERROR(ACE_TEXT("failed to parse protocol : cmsg_player_leave_world, player guid is <%llu>\n"), packet_stream->guid());
//	}
//	delete packet_stream;
//	return 0;
//}

//int RouteGSPacket::handlePlayerSwitchMap(PacketStream * packet_stream)
//{
//	//RouteClientPackage::instance()->putPackage(packet_stream);
//	return 0;
//
//	//int result = -1;
//	//typed::protocol::msg_player_jumpmap player_jumpmap;
//	//if (packet_stream->parse_protocol(player_jumpmap))
//	//{
//	//	RouteClientPackage::instance()->switchPlayerMap(player_jumpmap.player_guid(), player_jumpmap.map_template());
//	//	delete packet_stream;
//	//	result = 0;
//	//}
//	//else
//	//{
//	//	GATELog(LM_ERROR, ACE_TEXT("Failed to parse protocol in RouteGSPacket::handlePlayerSwitchMap\n"));
//	//}
//	//return result;
//}

void RouteGSPacket::collectThreadPacketStreamList(ThreadPacketListInfo * thread_pli)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_thread_packet_list_info_mutex, );
	m_thread_packet_list_info_list.push_back(thread_pli);
}

uint64 RouteGSPacket::makeMapID(int line_no, uint64 map_template)
{
	return MAKE_MAP_GUID_(line_no, map_template);
}

bool RouteGSPacket::existGS(uint64 map_id)
{
	GSSessionMapIDMap::iterator it = m_gs_session_map_id_map.find(map_id);
	return it != m_gs_session_map_id_map.end() ? true : false;
}