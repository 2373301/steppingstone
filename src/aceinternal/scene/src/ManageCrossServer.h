#ifndef MANAGE_CROSS_SERVER_HPP
#define Manage_Cross_Server_HPP

#include "Line.h"
#include "Packet.h"
#include "CrossServerSessionPool.h"
#include "CrossServerConnect.h"

struct OnlineServerInfo 
{
	OnlineServerInfo()
	{}

	string platform;
	string line_id;
};

class ManageCrossServer : public CrossServer, public CrossServerInput
{
public:
	ManageCrossServer();
	~ManageCrossServer();
public:
	virtual void crossServerInput(Packet * packet);

public:
	// for normal server
	virtual void sendCrossServerMsg(Packet * packet);

	// for cross server
	virtual void sendLineServerMsg(const string & line_id, const string & platform, Packet * packet);

	virtual void broadLineServerMsg(Packet * packet);

	virtual bool isOnline(const string & platform, const string & line_id);

	//virtual void addPlayerRoute(const string & line_id, const string & platform, uint64 player_guid);
public:
	int init(const CrossServerCfg & cs_cfg, const ServerCfg & server_cfg, const string & security_key, Scene * line);
protected:
	void handleCrossServerRegister(Packet * packet);

	void handleCrossServerDisconnect(Packet * packet);

	void collectOnlineServer(const string & platform, const string & line_id);

	void removeOnlineServer(const string & platform, const string & line_id);

private:
	CrossServerSessionPool * m_cross_server_session_pool;

	CrossServerConnect * m_cross_server_connect;

	Scene * m_line;

	typedef define_unordered_map<string, OnlineServerInfo *> OnlineServerInfoMap_t;

	OnlineServerInfoMap_t m_online_server_info_map;

	ACE_Thread_Mutex m_online_server_info_map_mutex;
};
#endif