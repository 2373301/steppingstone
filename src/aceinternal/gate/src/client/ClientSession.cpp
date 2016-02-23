
#include <ace/OS_NS_netdb.h>
#include <ace/os_include/netinet/os_tcp.h>
#include "GateLogger.h"
#include "ClientSession.h"
//#include "ManageDatabase.h"
#include "ManageConfig.h"
#include "RouteClientPackage.h"
#include "ManageStat.h"
#include "ManageClientSessionCooler.h"
#include "ManageClientConnectionTimeout.h"
#include "ManageClientStreamEx.h"
#include "ClientStrategyCenter.h"
//#include "EncryptionFactory.h"
#include "protocol/msg_gate.pb.h"
#include "opcode.h"
#include "RouteGSPacket.h"
#include "ManageClientBeat.h"
#include "ManageClientConnectionTimeout.h"
#include "ManageClientStreamLimit.h"
#include "ManageClientValidation.h"

STAT_NUMBER_IMP(ClientSession);

enum etypes
{
	et_null            = 0x00,       /// 空类型
	et_player          = 0x01,       /// 玩家类型
	et_npc             = 0x02,       /// npc类型
	et_monster         = 0x03,       /// 怪物类型
	et_team            = 0x04,       /// 团队类型
	et_item            = 0x05,       /// 物品类型  
	et_mail            = 0x07,       /// 邮件类型
	et_pet             = 0x08,       /// 宠物类型
	et_cookie          = 0x09,       /// 玩家临时数据

	et_dropbox         = 0x0A,       /// 掉落类型
	et_task            = 0x0B,       /// 任务类型  

	et_map             = 0x0C,       /// 地图类型
	et_scene           = 0x0D,       /// 场景类型
	et_guild           = 0x0E,       /// 工会类型
	et_guild_record    = 0x0F,       /// 工会活动记录
	et_guild_list      = 0x10,       /// 工会管理类型
	et_stall           = 0x11,       /// 摊位类型
	et_gather          = 0x12,       /// 采集物类型
	et_convoy		   = 0x13,       /// 护送物
	et_all   
};

char random_char(int rand_num)
{
	if (rand_num % 35 < 9)
	{
		return '1' + rand_num % 9;
	}
	else
	{
		return 'a' + rand_num % 26;
	}
}

void random_string(string & result, int str_len = 20)
{
	srand((unsigned int)ACE_OS::gettimeofday().sec());

	result.resize(str_len + 1, '\0');
	char * str = (char * )result.data();
	for (int i = 0; i < str_len; ++i)
	{
		str[i] = random_char(rand());
	}
}

inline etypes type_guid (uint64 guid)
{
	uint8 et = (uint8)((uint64(guid) >> 56) & 0x00000000000000FF);
	return (et > et_null && et < et_all)? etypes(et) : et_null;
}

AccountInfo::AccountInfo()
: role_location(0)
, role_guid(0)
, gm_level(1)
, account_level(1)
, account_state(AS_CONNECTED)
, last_beat_time(0)
, socket_state(SS_UNDEFINE)
, real_role_guid(0)
, time_openapi_call_last(0)
{
	last_beat_time = ACE_OS::gettimeofday().sec();
}

void AccountInfo::collectRoleInfo(uint64 role_guid, uint32 map_id, uint32 line_id)
{
	account_role_info_map.insert(std::make_pair(role_guid, AccountRoleInfo(role_guid, map_id, line_id)));
}

AccountRoleInfo * AccountInfo::getAccountRoleInfo(uint64 guid)
{
	AccountRoleInfoMap_t::iterator it = account_role_info_map.find(guid);
	if (it != account_role_info_map.end())
	{
		return &it->second;
	}

	return NULL;
}

//void encode_packet(Encryption * encryption, PacketStream * packet)
//{
//	try
//	{
//		if (NULL == encryption)
//		{
//			return ;
//		}
//
//		if (packet->body_size() == 0)
//			return ;
//
//		string dst_string;
//		if (encryption->encode(packet->pe_body(), packet->body_size(), dst_string))
//		{
//			packet->resetBody(dst_string.c_str(), dst_string.size());
//		}
//		else
//		{
//			// never happen
//		}
//	}
//	catch (std::exception * e)
//	{
//		GATELogp(LM_ERROR, ACE_TEXT("get exception while encoe_packet, error is <%s>\n"), e->what());
//	}
//	catch (...)
//	{
//		GATELogp(LM_ERROR, ACE_TEXT("get unknown exception while encoe_packet, last error is <%d>\n"), ACE_OS::last_error());
//	}
//}

//void decode_packet(Encryption * encryption, PacketStream * packet)
//{
//
//	try
//	{
//		if (NULL == encryption)
//		{
//			return ;
//		}
//
//		if (packet->body_size() == 0)
//			return ;
//
//		string dst_string;
//		if (encryption->decode(packet->pe_body(), packet->body_size(), dst_string))
//		{
//			packet->resetBody(dst_string.c_str(), dst_string.size());
//		}
//		else
//		{
//			// never happen
//		}
//	}
//	catch (std::exception * e)
//	{
//		GATELogp(LM_ERROR, ACE_TEXT("get exception while decode_packet, error is <%s>\n"), e->what());
//	}
//	catch (...)
//	{
//		GATELogp(LM_ERROR, ACE_TEXT("get unknown exception while decode_packet, last error is <%d>\n"), ACE_OS::last_error());
//	}
//}

ClientSession::ClientSession()
: m_line_no(0)
, m_addiction_flag(0)
, m_wait_logout_from_gs(false)
, m_thread_packet_list_info(NULL)
, m_output_wobble_time(0)
, m_login_again(false)
, m_in_coolder(false)
, m_enter_game_role_guid(0)
, m_pre_role_location(0)
//, m_encryption(NULL)
, m_last_online_time(0)
{
	m_handle_close_time = ACE_OS::gettimeofday();
	m_connection_time = ACE_OS::gettimeofday();
	m_input_msg_block.init(sMsgCfg->getClientCfg().recv_buffer_length);
	m_output_msg_block.init(sMsgCfg->getClientCfg().socket_buffer_length);

	//m_encryption_passphrase = "1234test";

	STAT_NUMBER_INC_NEW();

	GATE_LOG_INFO(ACE_TEXT("enter*** ClientSession::ClientSession, pointer is <%x>\n"), (void *)this);
}

ClientSession::~ClientSession()
{
	GATE_LOG_INFO(ACE_TEXT("enter*** ClientSession::~ClientSession, account id is <%s>, pointer is <%x>\n"), m_account_info.account_id.c_str(), (void *)this);

	STAT_NUMBER_INC_DEL();
}

int ClientSession::open(void * p)
{
	GATE_LOG_DEBUG(ACE_TEXT("Enter ClientSession::open\n"));

	if (super::open(p) == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call super::open in ClientSession::open, the last error is : <%d>\n"), ACE_OS::last_error());
		return -1;
	}

	GATE_LOG_INFO(ACE_TEXT("Getting new client session, the client session pointer is <%x>\n"), (void *)this);

//	m_session_client_state.success_connected = true;

	m_connection_time = ACE_OS::gettimeofday();

	this->peer().enable(ACE_NONBLOCK);

	int flag = 1;
	this->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

	m_account_info.role_guid = Header::getTempGuid();
	RouteClientPackage::instance()->collectRoleClientSession(m_account_info.role_guid, this);

	RouteClientPackage::instance()->assignClientThreadPacketListInfo(this);

	if (this->reactor() != NULL)
	{
		this->reactor()->remove_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
	}

	ManageClientStreamEx::instance()->handleClientStream(this);

	ClientStrategyCenter::instance()->clientConnected(this);

	ACE_TCHAR peer_name[MAXHOSTNAMELEN] = {0};
	ACE_INET_Addr peer_addr;
	if (this->peer().get_remote_addr(peer_addr) == 0 &&
		peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN) == 0)
	{
		GATE_LOG_INFO(ACE_TEXT("Get new client connection, the ip is : %s\n"), peer_name);
		m_client_ip = peer_name;
	}
	return 0;
}

int ClientSession::handle_input(ACE_HANDLE fd)
{
	if (rd_stream() == -1)
	{
		PacketStream * ps = new PacketStream(GMSG_CLIENT_READ_ERROR, m_account_info.role_guid, "");
		this->putInputPacket(ps);
		return -1;
	}
	else
	{
		return 0;
	}
}

int ClientSession::handle_output(ACE_HANDLE fd)
{
	return 0;

	//int result = 0;
	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_mutex, -1);
	//if (m_output_msg_block.length() > 0)
	//{
	//	int r = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
	//	if (r > 0)
	//	{
	//		m_output_msg_block.rd_ptr(r);
	//		if (m_output_msg_block.length() == 0)
	//		{
	//			this->reactor()->remove_handler(this, ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
	//			ManageStat::instance()->statClientWobbleNetwork(-1);
	//		}
	//	}
	//	else
	//	{
	//		if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
	//		{
	//			// ok
	//		}
	//		else
	//		{
	//			// error
	//			result = -1;
	//		}
	//	}
	//}
	//else
	//{
	//	this->reactor()->remove_handler(this, ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
	//	ManageStat::instance()->statClientWobbleNetwork(-1);
	//}

	//if (0 != result)
	//{
	//	return -1;
	//}
	//else
	//{
	//	return 0;
	//}
}

int ClientSession::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	if (0 != close_mask)
	{
		// clean by ManageClientSessionCooler
		return 0;
	}

	GATE_LOG_INFO(ACE_TEXT("enter ClientSession::handle_close, account id is <%s>\n"), m_account_info.account_id.c_str());

	if (this->reactor() != NULL)
	{
		this->reactor()->remove_handler(this, ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL);
		this->reactor(NULL);
	}

	this->peer().close();

	m_handle_close_time = ACE_OS::gettimeofday();

	if (!getRoleOnlineAgain())
	{
		if (enterGameState() && (!leaveGameState()))
		{
			uint64 role_guid = m_account_info.role_guid;
			if (0 != m_account_info.real_role_guid)
			{
				role_guid = m_account_info.real_role_guid;
			}
			typed::protocol::cmsg_player_leave_map leave_map;
			leave_map.set_leave_type(1);
			PacketStream * ps = new PacketStream(CMSG_PLAYER_LEAVE_MAP, role_guid, leave_map.SerializeAsString());
			RouteGSPacket::instance()->handleClientPackage(ps, this->getRoleLocation());
		}
		RouteClientPackage::instance()->removeRoleClientSession(m_account_info.role_guid, this);
//		RouteClientPackage::instance()->accountOffline(m_account_info.account_id);
		ManageClientValidation::instance()->accountLogout(this);
	}
	else
	{
		// role guid is temp guid, so must remove it
		if (m_account_info.role_guid < 0xFFFFFFFF)
		{
			RouteClientPackage::instance()->removeRoleClientSession(m_account_info.role_guid, this);
		}
	}

	ManageStat::instance()->statClientDisconnection(1);

	ManageClientBeat::instance()->removeClient(this);

	ManageClientConnectionTimeout::instance()->removeClientSession(this);

	//if ((!this->getValidation()) && (!this->getConnectionTimeout()))
	//{
	//}

	ManageClientStreamLimit::instance()->remove_stream(m_account_info.role_guid);

	return 0;
}

int ClientSession::handleOutputStream(PacketStream * output_stream, bool be_encryption)
{
	if (NULL == output_stream)
	{
		return 0;
	}

	std::auto_ptr<PacketStream> auto_pack(output_stream);

	ManageStat::instance()->statClientOutputTraffic(output_stream->stream_size(), 1);

	int result = 0;

	int serialize_result = 0;

	int r = 0;

	serialize_result = serializeToMsgBlock(output_stream);

	if (m_output_msg_block.length() == 0)
	{
		return 0;
	}

	r = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
	if (r > 0)
	{
		m_output_msg_block.rd_ptr(r);
	}

	if (m_output_msg_block.length() > 0)
	{
		ManageStat::instance()->statClientWobbleNetwork(1);
		result = 1;
	}

	if (r < 0)
	{
		if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
		{
			result = 1;
		}
		else
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to send data in ClientSession::handleOutputStream, last error is <%d>\n"), ACE_OS::last_error());
			PacketStream * ps = new PacketStream(GMSG_CLIENT_WRITE_ERROR, m_account_info.role_guid, "");
			this->putInputPacket(ps);
			result = 0;
		}
	}

	if (-1 == serialize_result)
	{
		result = 0;
	}
	
	//delete output_stream;

	return result;
}

int ClientSession::serializeToMsgBlock(PacketStream * output_stream)
{
	int result = 0;
	int msg_length = m_output_msg_block.length();
	if (m_output_msg_block.space() < output_stream->stream_size())
	{
		if (msg_length * 2 > m_output_msg_block.size())
		{
			ManageStat::instance()->statClientPacketLoss(1);
			GATE_LOG_ERROR("client output buffer almost use up, kick player offline, used buffer size is <%d>\n", m_output_msg_block.size());
			PacketStream * ps = new PacketStream(GMSG_CLIENT_WRITE_ERROR, output_stream->guid(), "");
			this->putInputPacket(ps);
			result = -1;
		}
		m_output_msg_block.crunch();
		//memcpy(m_output_msg_block.base(), m_output_msg_block.rd_ptr(), msg_length);
		//m_output_msg_block.rd_ptr(m_output_msg_block.base());
		//m_output_msg_block.wr_ptr(m_output_msg_block.base() + msg_length);
	}

	if (m_output_msg_block.space() < output_stream->stream_size())
	{
		GATE_LOG_ERROR("get the large package, opcode is <%d>, size is <%d>, output buf curr size is <%d>, size is <%d>\n", output_stream->opcode(), output_stream->body_size(), m_output_msg_block.space(), m_output_msg_block.size());
		return 0;
	}

	ClientHeader client_header(output_stream->opcode(), output_stream->body_size());
	m_output_msg_block.copy((char *)&client_header, sizeof(client_header));
	m_output_msg_block.copy(output_stream->pe_body(), output_stream->body_size());

	return result;
}

uint64 ClientSession::getRoleLocation()
{
	// todo
	return m_account_info.role_location;
}

void ClientSession::setRoleLocation(uint64 role_location)
{
	m_account_info.role_location = role_location;
}

void ClientSession::setRoleGuid(uint64 role_guid)
{
	m_account_info.role_guid = role_guid;
}

uint64 ClientSession::getRoleGuid()
{
	return m_account_info.role_guid;
}

const ACE_Time_Value & ClientSession::getHandleCloseTime()
{
	return m_handle_close_time;
}

const ACE_Time_Value & ClientSession::getConnectionTime()
{
	return m_connection_time;
}

void ClientSession::cleanup()
{
	if (m_output_msg_block.length() > 0)
	{
		ManageStat::instance()->statClientWobbleNetwork(-1);
	}
}

void ClientSession::createEncryKey()
{
	//random_string(m_encryption_passphrase);
}

void ClientSession::createEncryKeyObject()
{
//	m_encryption = EncryptionFactory::createEncryption(z_passphrase);
}

void ClientSession::sendClientEncryKey()
{
	//typed::protocol::smsg_encryption_key encry_key;
	////encry_key.set_encry_type(typed::protocol::smsg_encryption_key_ENCRY_TYPE(EncryptionFactory::getEncryType()));
	//encry_key.set_encry_type(typed::protocol::smsg_encryption_key_ENCRY_TYPE(0));
	//encry_key.set_encry_key(m_encryption_passphrase);
	//PacketStream * ps = new PacketStream(SMSG_ENCRYPTION_KEY, getRoleGuid(), encry_key.SerializeAsString());
	//this->handleOutputStream(ps, false);
}

void ClientSession::playerLineInfo(uint64 role_guid, uint32 map_id, uint32 line_id)
{
	m_account_info.collectRoleInfo(role_guid, map_id, line_id);
}

void ClientSession::setThreadPacketInfo(ThreadPacketListInfo * thread_packet_list_info)
{
	m_thread_packet_list_info = thread_packet_list_info;
}

void ClientSession::putInputPacket(PacketStream * packet_stream)
{
	if (NULL != m_thread_packet_list_info)
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, *m_thread_packet_list_info->thread_packet_stream_input_mutex, );
		m_thread_packet_list_info->thread_packet_stream_input_list->push_back(packet_stream);
	}
}

void ClientSession::putOutputPacket(PacketStream * packet_stream)
{
	if (NULL != m_thread_packet_list_info)
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, *m_thread_packet_list_info->thread_packet_stream_output_mutex, );
		m_thread_packet_list_info->thread_packet_stream_output_list->push_back(packet_stream);
	}
}

const string & ClientSession::getClientIp()
{
	return m_client_ip;
}

void ClientSession::setRealRoleGuid(uint64 role_guid)
{
	m_account_info.real_role_guid = role_guid;
}

int ClientSession::rd_stream()
{
	int result = 0;
	int recv_n = this->peer().recv(m_input_msg_block.wr_ptr(), m_input_msg_block.space());
	if (recv_n > 0)
	{
		m_input_msg_block.wr_ptr(recv_n);
		result = parseInputPacket();
	}
	else if (0 == recv_n)
	{
		// normal close by client
		ManageStat::instance()->statClientActiveClose(1);
		GATE_LOG_ERROR(ACE_TEXT("Remote connection closed in ClientSession::rd_stream, recv number is 0, last error is <%d>\n"), ACE_OS::last_error());
		result = -1;
	}
	else if (recv_n < 0)
	{
		if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
		{
			// it's ok
		}
		else
		{
			ManageStat::instance()->statClientPassiveClose(1);
			GATE_LOG_ERROR(ACE_TEXT("Remote connection closed in ClientSession::rd_stream, last error is <%d>\n"), ACE_OS::last_error());
			result = -1;
		}
	}
	return result;
}

int ClientSession::wt_stream()
{
	if (m_output_msg_block.length() == 0)
	{
		return 0;
	}

	int result = 0;

	int r = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
	if (r > 0)
	{
		m_output_msg_block.rd_ptr(r);
		if (m_output_msg_block.length() != 0)
		{
			result = 1;
		}
		else
		{
			result = 0;
		}

		m_output_msg_block.crunch();
	}
	else
	{
		if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
		{
			result = 1;
		}
		else
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to send data in ClientSession::handleOutputStream, last error is <%d>\n"), ACE_OS::last_error());
			PacketStream * ps = new PacketStream(GMSG_CLIENT_WRITE_ERROR, m_account_info.role_guid, "");
			this->putInputPacket(ps);
			result = 0;
		}
	}
	return result;
}

AccountRoleInfo * ClientSession::getAccountRoleInfo(uint64 guid)
{
	return m_account_info.getAccountRoleInfo(guid);
}

void ClientSession::enterGame()
{
	m_account_info.account_state |= AS_ENTER_GAME;
}

bool ClientSession::enterGameState()
{
	return m_account_info.account_state & AS_ENTER_GAME;
}

void ClientSession::setValidation(bool validation)
{
	//m_session_client_state.validate = validation;
	if (validation)
	{
		m_account_info.account_state = m_account_info.account_state | AS_VALIDATE_SUCCESS;
	}
	else
	{
		m_account_info.account_state = m_account_info.account_state | AS_VALIDATE_FAILED;
	}
}

bool ClientSession::getValidation()
{
	//return m_session_client_state.validate;
	return AS_VALIDATE_SUCCESS & m_account_info.account_state;
}

void ClientSession::acquireRoleList()
{
	m_account_info.account_state |= AS_GET_ROLE_LIST;
}

bool ClientSession::acquireRoleListState()
{
	return m_account_info.account_state & AS_GET_ROLE_LIST;
}

bool ClientSession::getConnected()
{
	return SS_CONNECTED & m_account_info.socket_state;
}

void ClientSession::setConnectionTimeout(bool connection_timeout)
{
	if (connection_timeout)
	{
		m_account_info.account_state |= AS_CONNECTION_TIMEOUT;
	}
//	m_account_info.connection_timeout = connection_timeout;
}

bool ClientSession::getConnectionTimeout()
{
	return AS_CONNECTION_TIMEOUT & m_account_info.account_state;
}

void ClientSession::setBeatTimeout(bool beat_timeout)
{
	if (beat_timeout)
	{
		m_account_info.account_state |= AS_BEAT_TIMEOUT;
	}
	//m_session_client_state.beat_timeout = beat_timeout;
}

bool ClientSession::getBeatTimeout()
{
	return AS_BEAT_TIMEOUT & m_account_info.account_state;
}

void ClientSession::gsKickOff()
{
	m_account_info.account_state |= AS_KICK_OFF;
}

bool ClientSession::gsKickOffState()
{
	return m_account_info.account_state & AS_KICK_OFF;
}

void ClientSession::leaveGame()
{
	m_account_info.account_state |= AS_LEAVE_GAME;
}

bool ClientSession::leaveGameState()
{
	return m_account_info.account_state & AS_LEAVE_GAME;
}

void ClientSession::onlineAgain()
{
	m_account_info.account_state |= AS_ONLINE_AGAIN;
}

bool ClientSession::onlineAgainState()
{
	return m_account_info.account_state & AS_ONLINE_AGAIN;
}

void ClientSession::preOnline()
{
	m_account_info.account_state |= AS_PRE_ONLINE;
}

bool ClientSession::preOnlineStage()
{
	return m_account_info.account_state & AS_PRE_ONLINE;
}

void ClientSession::clearPreOnline()
{
	m_account_info.account_state = m_account_info.account_state & (~AS_PRE_ONLINE);
}

void ClientSession::setNotFindGS(bool not_find_gs)
{
	if (not_find_gs)
	{
		m_account_info.account_state |= AS_NOT_FIND_GS;
	}
	//m_session_client_state.not_find_gs = not_find_gs;
}

bool ClientSession::getNotFindGS()
{
	return AS_NOT_FIND_GS & m_account_info.account_state;
	//return m_session_client_state.not_find_gs;
}

void ClientSession::enterGameSucess()
{
	m_account_info.account_state |= AS_ENTER_GAME_SUCCESS;
}

bool ClientSession::enterGameSucessState()
{
	return m_account_info.account_state & AS_ENTER_GAME_SUCCESS;
}

uint32 ClientSession::getAccountState()
{
	return m_account_info.account_state;
}

bool ClientSession::canEnterGame()
{
	GATE_LOG_DEBUG("real role guid is <%llu>\n", this->m_account_info.real_role_guid);
	if (this->m_account_info.real_role_guid > 0)
	{
		return true;
	}
	else
	{
		return acquireRoleListState() && (!enterGameState());
	}
}

bool ClientSession::canDispatchMsgToGS()
{
	return enterGameState();
}

void ClientSession::addWobbleTime()
{
	m_output_wobble_time += 1;
}

int ClientSession::getWobbleTime()
{
	return m_output_wobble_time;
}

void ClientSession::clearWobbleTime()
{
	m_output_wobble_time = 0;
}

void ClientSession::roleOnlineAgain()
{
	m_login_again = true;
}

void ClientSession::checkLastOnlineTime()
{
	if (0 == m_last_online_time)
	{
		ACE_OS::gettimeofday().to_usec(m_last_online_time);
	}
}

uint64 ClientSession::getLastOnlineTime()
{
	return m_last_online_time;
}

bool ClientSession::getRoleOnlineAgain()
{
	return m_login_again;
}

void ClientSession::putInCooler()
{
	m_in_coolder = true;
}

bool ClientSession::alreadyInCooler()
{
	return m_in_coolder;
}

void ClientSession::setAccountID(const string & account_id)
{
	m_account_info.account_id = account_id;
}

const string & ClientSession::getAccountID()
{
	return m_account_info.account_id;
}

uint32 ClientSession::getGMLevel()
{
	return m_account_info.gm_level;
}

void ClientSession::setGMLevel(uint32 gm_level)
{
	m_account_info.gm_level = gm_level;
}

void ClientSession::setAccountLevel(uint32 account_level)
{
	m_account_info.account_level = account_level;
}

uint32 ClientSession::getAccountLevel()
{
	return m_account_info.account_level;
}

void ClientSession::closeHandle()
{
}

//void ClientSession::setClientLogout(bool client_logout)
//{
//	m_account_info.role_logout = client_logout;
//}

bool ClientSession::getClientLogout()
{
	// todo
	return false;
	//return m_account_info.role_logout;
}

void ClientSession::setClientClose(bool client_close)
{
	// todo
	//m_account_info.client_close = client_close;
}

bool ClientSession::getClientClose()
{
	// todo
	return false;
	//return m_account_info.client_close;
}

//void ClientSession::setClientLogouted(bool client_logouted)
//{
//	m_account_info.client_logouted = client_logouted;
//}

bool ClientSession::getClientLogouted()
{
	return false;
	//return m_account_info.client_logouted;
}

void ClientSession::setClientLoginGS(bool login_gs)
{
	//m_account_info.client_logined_gs = login_gs;
}

bool ClientSession::getClientLoginGS()
{
	return false;
	//return m_account_info.client_logined_gs;
}

void ClientSession::setWaitLogoutFromGS(bool value)
{
	m_wait_logout_from_gs = value;
}

bool ClientSession::getWaitLogoutFromGS()
{
	return m_wait_logout_from_gs;
}

void ClientSession::updateLastBeatTime()
{
	m_account_info.last_beat_time = ACE_OS::gettimeofday().sec();
	//m_session_client_state.last_beat_time = ACE_OS::gettimeofday().sec();
}

time_t ClientSession::getLastBeatTime()
{
	return m_account_info.last_beat_time;
}

void ClientSession::setLineNo(int line_no)
{
	m_line_no = line_no;
}

void ClientSession::setAddictionFlag(int addiction_flag)
{
	m_addiction_flag = addiction_flag;
}

int ClientSession::getLineNo()
{
	return m_line_no;
}

void ClientSession::setEnterRoleGuid(uint64 role_guid)
{
	m_enter_game_role_guid = role_guid;
}

uint64 ClientSession::getEnterRoleGuid()
{
	return m_enter_game_role_guid;
}

void ClientSession::setPreRoleLocation(uint64 role_location)
{
	m_pre_role_location = role_location;
}

uint64 ClientSession::getPreRoleLocation()
{
	return m_pre_role_location;
}

int ClientSession::parseInputPacket()
{
	int result = 0;

	PacketStream * ps = NULL;
	int msg_length = 0;
//	try
	{
		ClientHeader client_header;
		msg_length = m_input_msg_block.length();
		while(true)
		{
			memset(&client_header, 0, sizeof(client_header));
			if (m_input_msg_block.length() < sizeof(client_header))
			{
				break;
			}

			memcpy(&client_header, m_input_msg_block.rd_ptr(), sizeof(client_header));

			if (client_header.size > 20000)
			{
				GATE_LOG_ERROR(ACE_TEXT("packet body size is too big : <%d>, opcode is <%d>, account is <%s>\n"), client_header.size, client_header.opcode, m_account_info.account_id.c_str());
				return -1;
			}

			if (m_input_msg_block.length() >= sizeof(client_header) + client_header.size)
			{
				ps = new PacketStream();
				//ps->setEventHandler(this);
				ps->set_header(client_header.opcode, client_header.size, m_account_info.role_guid);
				ps->setRoleLocation(m_account_info.role_location);
				ps->setLineNO(m_line_no);

				m_input_msg_block.rd_ptr(sizeof(client_header));
				if (ps->body_size() > 0)
				{
					memcpy(ps->re_body(), m_input_msg_block.rd_ptr(), ps->body_size());
					m_input_msg_block.rd_ptr(ps->body_size());
				}
				else
				{
					// do nothing
				}

				ManageStat::instance()->statClientInputTraffic(ps->stream_size(), 1);

				ps->guid(m_account_info.role_guid);

				this->putInputPacket(ps);
				ps = NULL;
				msg_length = m_input_msg_block.length();
			}
			else
			{
				delete ps;
				break;
			}
		}

		m_input_msg_block.crunch();
	}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in ClientSession::parseInputPacket, last error is <%d>\n"), ACE_OS::last_error());
	//	return -1;
	//}

	return result;
}
const string & ClientSession::getOpenId()
{
	return m_account_info.open_id;
}

const string & ClientSession::getOpenKey()
{
	return m_account_info.open_key;
}
const string & ClientSession::getPF()
{
	return m_account_info.pf;
}


void ClientSession::setOpenId(const string& open_id)
{
	m_account_info.open_id = open_id;
}

void ClientSession::setOpenKey(const string& open_key)
{
	m_account_info.open_key = open_key;
}

void ClientSession::setPF(const string& pf)
{
	m_account_info.pf = pf;
}

uint32 ClientSession::get_openapi_call_time()
{
	return m_account_info.time_openapi_call_last;
}
void ClientSession::set_openapi_call_time(uint32 time)
{
	m_account_info.time_openapi_call_last = time;
}