#ifndef _ROUTE_GS_PACKET_HPP
#define _ROUTE_GS_PACKET_HPP

#include <map>
#include <vector>
#include <ace/Task.h>
#include "typedef.h"
#include "AppNotifyInterface.h"
#include "GateLogger.h"
#include "Singleton.h"
#include "PacketStream.h"
#include "GSSession.h"
#include "ClientSession.h"

class RouteGSPacket : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	RouteGSPacket();
	~RouteGSPacket();
public:
	static RouteGSPacket * instance()
	{
		return Singleton<RouteGSPacket>::instance();
	}
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	typedef vector<PacketStream *> PackageStreamList;

	typedef std::map<uint64, GSSession *> GSSessionMapIDMap;

	typedef vector<ThreadPacketListInfo *> ThreadPacketListInfoList;

	typedef map<uint64, ThreadPacketListInfo *>  ClientBelongThreadPacketMap;

	virtual int svc(void);

public:
	/// AppNotifyInterface init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// AppNotifyInterface fini, stop
	virtual int fini();

	/// AppNotifyInterface wait for finish
	virtual int wait();

public:
	/// push back, recv package from gs 
	void putPackage(PacketStream * packet_stream);

	/// recv package form client, put it to gs session output list
	int handleClientPackage(PacketStream * packet_stream, uint64 map_id);

	//void handleMemberPackage(PacketStream * packet_stream);

	void registerGSMapID(uint64 map_id, GSSession * gs_session);

	void registerTranscriptMapID(uint64 map_id, GSSession * gs_session);

	GSSession * getGSSession(uint64 map_id);

	void removeGSMapID(uint64 map_id);

	void assignClientThreadPacketListInfo(uint64 role_guid);

	void removeClientThreadPacketListInfo(uint64 role_guid);
private:
	/// process package
	//int handlePackage(PacketStream * packet_stream);

	int handleRegisterMapID(PacketStream * packet_stream);

	int handleMemberChargeConfirm(PacketStream * packet_stream);

	int handleMemberGiveGift(PacketStream * packet_stream);

	//int handleMsgTest(PacketStream * packet_stream);

	//int handlePlayerJumpmap(PacketStream * packet_stream);

	//int handleKickPlayer(PacketStream * packet_stream);

	//int handlePlayerLeaveWorld(PacketStream * packet_stream);

	//int handlePlayerSwitchMap(PacketStream * packet_stream);

	void collectThreadPacketStreamList(ThreadPacketListInfo * thread_pli);

	uint64 makeMapID(int line_no, uint64 map_template);

	bool existGS(uint64 map_id);
private:
	bool m_stop;

	bool m_wait;

	ACE_Thread_Mutex	m_packet_mutex;

	PackageStreamList	m_packet_stream_list;

	ACE_Thread_Mutex	m_gs_session_map_mutex;

	GSSessionMapIDMap	m_gs_session_map_id_map;

	ACE_Thread_Mutex	m_thread_packet_list_info_mutex;

	ThreadPacketListInfoList m_thread_packet_list_info_list;

	uint32				m_thread_packet_stream_index;

	ACE_Thread_Mutex	m_client_belong_thread_packet_mutex;

	ClientBelongThreadPacketMap m_client_belong_thread_packet_map;
};

#endif