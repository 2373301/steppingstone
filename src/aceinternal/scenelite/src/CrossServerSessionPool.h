
#ifndef CROSS_SERVER_SESSION_POOL_HPP
#define CROSS_SERVER_SESSION_POOL_HPP

#include <ace/Thread_Mutex.h>
#include "SessionPool.h"
#include "Session.h"
#include "Logger.h"
#include "typedef.h"
#include "Scene.h"
#include "Packet.h"

using namespace netstream;

struct CrossSessionInfo 
{
	CrossSessionInfo()
		: session(NULL)
		, validated(false)
	{}

	Session_t session;
	bool validated;
	string line_id;
	string platform;
};

struct CrossServerPlayerRouteInfo 
{
	CrossServerPlayerRouteInfo()
	: player_guid(0)
	{}

	uint64 player_guid;
	string line_id;
	string platform;
};

class CrossServerSessionPool : public HandleSessionEvent, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	CrossServerSessionPool();
	~CrossServerSessionPool();
public:
	int init(const CrossServerCfg & corss_sever_cfg, const string & security_key, CrossServerInput * cross_server_input);

	void output(const string & line_id, const string & platform, Packet * packet);

	//void addPlayerRoute(const string & line_id, const string & platform, uint64 player_guid);
public:
	int svc();

public:
	virtual void newConnection(Session_t session);

	virtual void connectionClosed(Session_t session, int trigger_source);

	virtual void handleInputStream(Session_t session, ACE_Message_Block & msg_block);
public:
	
protected:
	bool handleCrossServerRegister(Session_t session, Packet * packet);

	bool isValidatedSession(Session_t session);

	bool checkServerValidatedInfo(Session_t session, const string & platform, const string & line_id, const string & security_key);

	void collectServerRegisterInfo(Session_t session, const string & platform, const string & line_id, const string & security_key);

	Session_t getSessionByPlayer(uint64 player_guid);

	Session_t getSession(const string & platform, const string & line_id);
private:
	typedef define_unordered_map<string, CrossSessionInfo *> CrossSessionInfoMap_t;

	typedef define_unordered_set<Session_t> SessionSet_t;

	typedef define_unordered_map<uint64, CrossServerPlayerRouteInfo *> CrossServerPlayerRouteInfoMap_t;

	SessionPool * m_session_pool;

	CrossServerInput * m_cross_server_input;

	CrossSessionInfoMap_t m_cross_session_info_map;

	ACE_Thread_Mutex m_cross_session_info_map_mutex;

	CrossServerCfg m_corss_sever_cfg;

	string m_security_key;

	SessionSet_t m_validated_session_set;

	CrossServerPlayerRouteInfoMap_t m_cross_server_player_route_map;

	ACE_Thread_Mutex m_cross_server_player_route_map_mutex;

	PacketQue_t m_output_packet_que;

	ACE_Thread_Mutex m_output_packet_que_mutex;
};
#endif