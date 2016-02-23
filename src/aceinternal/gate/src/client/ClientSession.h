#ifndef _CLIENT_SESSION_HPP
#define _CLIENT_SESSION_HPP

#include <list>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <vector>
#include "PacketStream.h"
//#include "Encryption.h"
#include "Logger.h"

#define MAKE_GUID_(type, ggen, id) (((uint64(type) << 56) & 0xFF00000000000000) | ((uint64(ggen) << 48) & 0x00FF000000000000) | (uint64(id) & 0x0000FFFFFFFFFFFF))

#define MAKE_MAP_GUID_(line, id) MAKE_GUID_(0x0C, 15, (((uint64)(0) << 40) | uint64(id)))

class ClientSession;

typedef ACE_Acceptor<ClientSession, ACE_SOCK_ACCEPTOR> ClientAcceptor;

enum SockState
{
	SS_UNDEFINE		= 0,
	SS_CONNECTED,
	SS_ABORT,
	SS_CLOSE,
};

enum AccountState
{
	AS_CONNECTED			= 0,
	AS_CONNECTION_TIMEOUT	= 0x1,		// 从连接到进入游戏timeout时间
	AS_VALIDATE_FAILED		= 0x2,
	AS_VALIDATE_SUCCESS		= 0x4,
	AS_GET_ROLE_LIST		= 0x8,
	AS_ENTER_GAME			= 0x10,
	AS_NOT_FIND_GS			= 0x20,
	AS_ENTER_GAME_SUCCESS	= 0x40,
	AS_BEAT_TIMEOUT			= 0x80,
	AS_KICK_OFF				= 0x100,
	AS_KICK_OFF_SUCCESS		= 0x200,
	AS_LEAVE_GAME			= 0x400,
	AS_LEAVE_GAME_SUCCESS	= 0x800,
	AS_ONLINE_AGAIN			= 0x1000,
	AS_PRE_ONLINE			= 0x2000,
};

struct AccountRoleInfo 
{
	AccountRoleInfo()
	: guid(0)
	, map_id(0)
	, line_id(0)
	{}

	AccountRoleInfo(uint64 role_guid, uint32 map_id_, uint32 line_id_)
	: guid(role_guid)
	, map_id(map_id_)
	, line_id(line_id_)
	{}

	uint64 guid;
	uint32 map_id;
	uint32 line_id;
};

class AccountInfo 
{
public:
	AccountInfo();

	void collectRoleInfo(uint64 role_guid, uint32 map_id, uint32 line_id);

	AccountRoleInfo * getAccountRoleInfo(uint64 guid);
public:
	uint64 role_location;

	uint64 role_guid;

	string account_id;

	uint32 gm_level;

	uint32 account_level;

	uint32 account_state;

	uint32 last_beat_time;

	SockState socket_state;

	uint64 real_role_guid;

	typedef map<uint64, AccountRoleInfo> AccountRoleInfoMap_t;

	AccountRoleInfoMap_t account_role_info_map;

	string open_id;
	string open_key;
	string pf;
	uint32 time_openapi_call_last;
};

struct ThreadPacketListInfo
{
	typedef vector<PacketStream *> PackageStreamList;

	ThreadPacketListInfo()
	{
		thread_packet_stream_input_list = new PackageStreamList();
		thread_packet_stream_input_mutex = new ACE_Thread_Mutex();

		thread_packet_stream_output_list = new PackageStreamList();
		thread_packet_stream_output_mutex = new ACE_Thread_Mutex();
	}

	~ThreadPacketListInfo()
	{
		delete thread_packet_stream_input_list;
		delete thread_packet_stream_input_mutex;

		delete thread_packet_stream_output_list;
		delete thread_packet_stream_output_mutex;
	}

	PackageStreamList * thread_packet_stream_input_list;
	ACE_Thread_Mutex * thread_packet_stream_input_mutex;

	PackageStreamList * thread_packet_stream_output_list;
	ACE_Thread_Mutex * thread_packet_stream_output_mutex;
};

//void encode_packet(Encryption * encryption, PacketStream * packet);
//
//void decode_packet(Encryption * encryption, PacketStream * packet);

class ClientSession : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	ClientSession();
	~ClientSession();
public:
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;

	typedef std::list<PacketStream *> PacketStreamList;

	virtual int open(void * p=0);

	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_output(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

public:
	// return 0 : normal, 1 : cache buffer, 
	int handleOutputStream(PacketStream * output_stream, bool be_encryption = true);

	int serializeToMsgBlock(PacketStream * output_stream);

	uint64 getRoleLocation();

	void setRoleLocation(uint64 role_location);

	void setRoleGuid(uint64 role_guid);

	uint64 getRoleGuid();

	const ACE_Time_Value & getHandleCloseTime();

	const ACE_Time_Value & getConnectionTime();

	void cleanup();

	void createEncryKey();

	void createEncryKeyObject();

	void sendClientEncryKey();

	void playerLineInfo(uint64 role_guid, uint32 map_id, uint32 line_id);

	void setThreadPacketInfo(ThreadPacketListInfo * thread_packet_list_info);

	void putInputPacket(PacketStream * packet_stream);

	void putOutputPacket(PacketStream * packet_stream);

	const string & getClientIp();

	void setRealRoleGuid(uint64 role_guid);

public:
	// return -1 : error, 1 : read packet finish, 2 : EWOULDBLOCK
	int rd_stream();

	int wt_stream();

public:
	AccountRoleInfo * getAccountRoleInfo(uint64 guid);

	void setValidation(bool validation);

	bool getValidation();

	void acquireRoleList();

	bool acquireRoleListState();

	void enterGame();

	bool enterGameState();

	bool getConnected();

	void setNotFindGS(bool not_find_gs);

	bool getNotFindGS();

	void enterGameSucess();

	bool enterGameSucessState();

	void setConnectionTimeout(bool connection_timeout);

	bool getConnectionTimeout();

	void setBeatTimeout(bool beat_timeout);

	bool getBeatTimeout();

	void gsKickOff();

	bool gsKickOffState();

	void leaveGame();

	bool leaveGameState();

	void onlineAgain();

	bool onlineAgainState();

	void preOnline();

	bool preOnlineStage();

	void clearPreOnline();

public:
	uint32 getAccountState();

	bool canEnterGame();

	bool canDispatchMsgToGS();

	//
	void addWobbleTime();

	int getWobbleTime();

	void clearWobbleTime();

	void roleOnlineAgain();

	void checkLastOnlineTime();

	uint64 getLastOnlineTime();

	bool getRoleOnlineAgain();

	void putInCooler();

	bool alreadyInCooler();

public:
	
	void setAccountID(const string & account_id);

	const string & getAccountID();

	uint32 getGMLevel();

	void setGMLevel(uint32 gm_level);

	void setAccountLevel(uint32 account_level);

	uint32 getAccountLevel();

	void closeHandle();

	//void setClientLogout(bool client_logout);

	bool getClientLogout();

	void setClientClose(bool client_close);

	bool getClientClose();

	//void setClientLogouted(bool client_logouted);

	bool getClientLogouted();

	void setClientLoginGS(bool login_gs);

	bool getClientLoginGS();

	void setWaitLogoutFromGS(bool value);

	bool getWaitLogoutFromGS();

	void updateLastBeatTime();

	time_t getLastBeatTime();

	void setLineNo(int line_no);

	void setAddictionFlag(int addiction_flag);

	int getLineNo();

	void setEnterRoleGuid(uint64 role_guid);

	uint64 getEnterRoleGuid();

	void setPreRoleLocation(uint64 role_location);

	uint64 getPreRoleLocation();

	const string & getOpenId();

	const string & getOpenKey();
	
	const string & getPF();

	void setOpenId(const string& open_id);

	void setOpenKey(const string& open_key);

	void setPF(const string& pf);

	//获取最后一次openapi调用时间
	uint32 get_openapi_call_time();
	//设置openapi调用时间
	void set_openapi_call_time(uint32 time);

private:
	int parseInputPacket();

private:
	//ClientSessionState m_session_client_state;

	AccountInfo	m_account_info;

	int m_line_no;

	int m_addiction_flag;

	bool m_wait_logout_from_gs;

	ThreadPacketListInfo * m_thread_packet_list_info;

	int m_output_wobble_time;

	bool m_login_again;

	bool m_in_coolder;

//	Encryption * m_encryption;

	string m_encryption_passphrase;

	ACE_Message_Block m_input_msg_block;

	ACE_Message_Block m_output_msg_block;

	ACE_Time_Value m_connection_time;

	ACE_Time_Value m_handle_close_time;

	string m_client_ip;

	STAT_NUMBER_DEC();

	uint64 m_enter_game_role_guid;

	uint64 m_pre_role_location;

	uint64 m_last_online_time;
};

#endif