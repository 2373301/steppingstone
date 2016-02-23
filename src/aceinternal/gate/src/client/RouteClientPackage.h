#ifndef _ROUTECLIENTPACKAGE_HPP
#define _ROUTECLIENTPACKAGE_HPP

#include <map>
#include <vector>
#include <ace/Task.h>
#include "typedef.h"
#include "Singleton.h"
#include "PacketStream.h"
#include "AppNotifyInterface.h"
#include "ClientSession.h"

class RouteClientPackage : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	RouteClientPackage();

	~RouteClientPackage();
public:
	static RouteClientPackage * instance()
	{
		return Singleton<RouteClientPackage>::instance();
	}
public:
	typedef vector<PacketStream *> PackageStreamList;					///存放接收的客户端的PacketStream列表

	typedef std::map<uint64, ClientSession *> ClientSessionRoleMap;  ///角色与ClientSession之间的映射关系

	typedef std::map<ClientSession *, uint64> ClientSessionRoleReverseMap;  ///角色与ClientSession之间的映射关系

	typedef std::map<uint64, int32>			RoleLocationMap;

	virtual int  svc(void);
public:
	/// AppNotifyInterface init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// AppNotifyInterface fini, stop
	virtual int fini();

	/// AppNotifyInterface wait for finish
	virtual int wait();

public:
	/// push back, recv from client
	void putInputPackage(PacketStream * packet_stream);

	void putOutputPackage(PacketStream * packet_stream);

	void handleRolePackage(PacketStream * packet_stream);

	void collectRoleClientSession(uint64 role_guid, ClientSession * cs_session);

	void removeRoleClientSession(uint64 role_guid, ClientSession * cs_session = NULL);

	void collectThreadPacketStreamList(ThreadPacketListInfo * thread_packet_li);

	void assignClientThreadPacketListInfo(ClientSession * cs_session);

	void removeClientThreadPacketListInfo(uint64 role_guid);

//	void clientSessionCloseNotify(ClientSession * client_session);

	ClientSession * getRoleClientSession(uint64 role_guid);

	//void accountOffline(const string & account_id);

	void cleanPlayerInfo(const string & player_account, uint64 player_guid);

	void roleLogoffSuccess(uint64 guid);
protected:

	void notFindGS(uint64 guid);
private:
	// input packet
	int handleInputPackage(PacketStream * packet_stream);

	//int handleTestTimeInterval(PacketStream * packet_stream);

	int handleValidationAccountPacket(PacketStream * packet_stream);

	//int handleChooseLinePacket(PacketStream * packet_stream);

	int handleCheckRoleName(PacketStream * packet_stream);

	int handleRoleMake(PacketStream * packet_stream);

	int handleEnterMap(PacketStream * packet_stream);

	int handleCmsgLoopback(PacketStream * packet_stream);

	int handleRoleBeat(PacketStream * packet_stream);

	int handleRoleLogout(PacketStream * packet_stream);

	int handleRoleLeaveMap(PacketStream * packet_stream);

	int handleClientClose(PacketStream * packet_stream);

	int handleClientReadError(PacketStream * packet_stream);

	int handleClientWriteError(PacketStream * packet_stream);

	int handleClientWobbleError(PacketStream * packet_stream);

	int handleClientBeatTimeout(PacketStream * packet_stream);

	bool handlePlayerLeaveMap(PacketStream * packet_stream);

	// output packet
	int handleOutputPackage(PacketStream * packet_stream);

	int handleSmsgPlayerMake(PacketStream * packet_stream);

	int handleSmsgRequestPlayerList(PacketStream * packet_stream);

	int handlePlayerJumpmap(PacketStream * packet_stream);

	int handleSmsgLoopback(PacketStream * packet_stream);

	int handlePlayerKickoff(PacketStream * packet_stream);

	// 
	void handleWobbleStream(Uint64Set_t & wobble_guid);
private:
	//void accountOnline(const string & account_id, ClientSession * client_session);

	void changeClientRoute(ClientSession * cs_session, uint64 temp_guid, uint64 role_guid);

	void clientEnterMap(uint64 client_guid, uint64 role_guid, bool make_route);

	bool lastOnlineTimeout(ClientSession * cs_session);

	void cleanRoleInfo(uint64 player_guid);

	void cleanRoleAndLogoff(uint64 role_guid);

	void roleLeaveMap(uint64 role_guid, uint64 map_id);

	//void cleanAccountInfo(const string & player_account);

private:
	typedef vector<ThreadPacketListInfo *> ThreadPacketListInfoList;

	typedef map<uint64, ThreadPacketListInfo *>  ClientBelongThreadPacketMap;

	typedef map<string, ClientSession *>  ClientAccountSessionMap;

	bool m_stop;

	bool m_wait;

	int m_max_role_number;

	uint32 m_thread_packet_stream_index;

	//ACE_Thread_Mutex	m_packet_mutex;

	//PackageStreamList	m_packet_stream_list;

	ACE_Thread_Mutex	m_client_session_map_mutex;

	ClientSessionRoleMap	m_client_session_map_id_map;

	ClientSessionRoleReverseMap m_client_session_reverse_map;

	ACE_Thread_Mutex	m_thread_packet_list_info_mutex;

	ThreadPacketListInfoList m_thread_packet_list_info_list;

	ACE_Thread_Mutex	m_client_belong_thread_packet_mutex;

	ClientBelongThreadPacketMap m_client_belong_thread_packet_map;

	ClientAccountSessionMap m_client_account_session;

	ACE_Thread_Mutex m_client_account_session_mutex;
};

#endif