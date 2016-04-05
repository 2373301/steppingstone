
#ifndef ROUTE_GATE_PACKET_HPP
#define ROUTE_GATE_PACKET_HPP

#include <map>
#include <ace/Task.h>
#include "NetStream.h"
#include "SingletonInit.h"
#include "Singleton.h"
#include "Packet.h"
#include "player.pb.h"
#include "msg_player.pb.h"
#include "msg_error.pb.h"
//#include "msg_log.pb.h"
#include "GateSession.h"
#include "ManageAccountDatabase.h"
//#include "LogSys.h"

enum CacheOpType
{
	COT_BASE = 0,
	COT_ADD = 1,
	COT_LOAD = 2,
	COT_REMOVE = 3
};

using namespace std;

struct RoleInfo 
{
	RoleInfo()
	: account("")
	, packet_guid(0)
	, player_guid(0)
	, role_guid(0)
	, op_type(COT_BASE)
	, gate_session(NULL)
	, player_state(0)
	, role_state(0)
	{}

	void setOpType(CacheOpType opt)
	{
		op_type = opt;
		if (COT_LOAD == op_type)
		{
			// do not load role entity in this case
			role_state = 1;
		}
	}

	void updateState(uint64 guid, bool success)
	{
		if (IS_PLAYER_GUID(guid))
		{
			player_state = success ? 1 : 2;
		}
		else if (IS_ROLE_GUID(guid))
		{
			role_state = success ? 1 : 2;
		}
	}

	bool is_success()
	{
		return (1 == player_state) && (1 == role_state);
	}

	bool is_finish()
	{
		return (player_state > 0) && (role_state > 0);
	}

	bool is_failed()
	{
		int mx = max(player_state, role_state);
		int mi = min(player_state, role_state);
		return ((mi >= 1) && (mx >= 2));
	}

	string account;
	uint64 packet_guid;
	uint64 player_guid;
	uint64 role_guid;
	CacheOpType op_type;
	GateSession * gate_session;
	int player_state;
	int role_state;
};

class RouteGatePacket : public ACE_Task<ACE_NULL_SYNCH>, public SingletonInit, public HandleNetPacket
{
public:
	RouteGatePacket();
	~RouteGatePacket();
public:
	static RouteGatePacket * instance()
	{
		return Singleton<RouteGatePacket>::instance();
	}
public:
	typedef std::map<GateSession *, int> GateSessionMap_t;
	/// init
	virtual int init();

	/// fini, stop
	virtual int stop();

	/// wait for finish
	virtual int finit();

public:
	typedef std::list<Packet *> PacketList_t;

	virtual int handlePacket(Packet * packet_stream);

	int handleCachePacket(Packet * packet_stream);

	void collectGateSession(GateSession * gate_session);

	void removeGateSession(GateSession * gate_session);

	bool existGateSession(GateSession * gate_session);

	void cacheRoleMakeFailed(uint64 role_guid);
public:
	virtual int svc(void);

protected:
	void doHandlePacket(Packet * packet_stream);

	int handleCheckRoleName(Packet * packet_stream);

	int handleRoleMake(Packet * packet_stream);

	int handleRequestRoleList(Packet * packet_stream);

	int handleRoleDelete(Packet * packet_stream);

	int handleRequestRandomName(Packet * packet_stream);

	int handleNewEneity(Packet * packet_stream);

	int handleLoadEneity(Packet * packet_stream);

	int handleRemoveEneity(Packet * packet_stream);

public:
	RoleInfo * getPlayerInfo(uint64 role_guid);

protected:
	void collectMakeRoleInfo(const RoleInfo & role_info);

	void removeRoleInfo(uint64 player_guid);

	void sendClientErrorInfo(GateSession * gate_session, uint64 packet_guid, int error_code);

	void replayRoleList(const AccountInfo & account_info);

	AccountInfo * getAccountInfo(const string & account);

	void cleanAccountInfo(const string & account);

	bool sendRemoteLog(uint64 role_guid);

	bool isMakingRole(uint64 packet_guid);
private:
	typedef map<uint64, RoleInfo> RoleInfoMap_t;

	typedef map<string, AccountInfo> AccountInfoMap_t;

	bool m_stop;

	int m_max_role_number;

	ACE_Thread_Mutex m_queqe_mutex;

	PacketList_t m_input_queue;

	RoleInfoMap_t m_role_info_map;

	ACE_Thread_Mutex m_role_info_map_mutex;

	ACE_Thread_Mutex m_gate_session_map_mutex;

	GateSessionMap_t m_gate_session_map;

	AccountInfoMap_t m_account_info_map;

//	hiso::Logger * m_remote_logger;
};

#endif