
#ifndef SCENE_DEPOT_IMP_HPP
#define SCENE_DEPOT_IMP_HPP

#include "typedef.h"
#include <ace/Thread_Mutex.h>
#include <ace/Task.h>
#include "SceneDepot.h"
#include "LoadDll.h"
#include "SimpleXML.h"
#include "Plugin.h"

class SceneDepotImp : public SceneDepot, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	SceneDepotImp();
	~SceneDepotImp();
public:
	virtual int init(const SceneDepotCfg & scene_depot_cfg);

	virtual int stop();

	virtual int finit();
public:
	// HandleInput
	virtual void input(Packet * packet);

	// cache handleInput
	virtual void cacheInput(Packet * packet, uint64 map_id, uint64 request_id);

public:
	/// svc of task
	virtual int svc (void);

protected:
	bool loadPlugin();

	bool loadPluginCfg();

	bool loadPluginCfg(const string & config_path, ParamConfig & param_config);

	bool loadPluginCfg(Element * root_element, ParamConfig & param_config);

	bool initScenes();

	bool initScene(const SceneCfg & scene_cfg, Scene * scene);

	void addRoute(uint64 player_guid, uint32 map_id, uint32 line_id);

	void removeRoute(uint64 player_guid);

	void modifyRoute(uint64 player_guid, uint32 map_id, uint32 line_id);

	Scene * getSceneByMap(uint64 map_id);

	Scene * getSceneByPalyer(uint64 player_guid);

	void dispatchTerminalPacket(Packet * packet);

	void dispatchCachePacket(const CachePackInfo & cache_pack_info);

	void lostGateConnection();

	void recvSingleUsr1();

	void notifyAllScenesToFlushPlayerData();

	void checkSceneStateAfterLostGateConnection();

	void checkSceneStateStartupSuccess();

	void allScenesFinishToFlushPlayers();

	void allScenesStartupSuccessNotify();

	void sendNotifyMsg(Packet * packet);
private:
	typedef map<uint64, Scene *> SceneMap_t;

	typedef map<uint64, Scene *> PlayerRouteMap_t;

	typedef vector<CachePackInfo> CacheInputPacketVec_t;

	bool	m_lost_gate_connection;

	bool	m_success_startup_notify;

	bool	m_success_flush_data_notify;

	bool	m_recv_sigusr1;

	bool	m_notify_all_scenes_to_flush_player_data;

	SceneDepotCfg m_scene_deopt_cfg;

	LoadDll m_load_dll;

	PluginParamConfigVec_t m_plugin_config_vec;

	///地图mapping
	ACE_Thread_Mutex m_scene_map_mutex;

	SceneMap_t m_scene_map;

	///玩家到地图mapping
	ACE_Thread_Mutex m_player_route_map_mutex;

	PlayerRouteMap_t m_player_route_map;

	/// 消息包输入队列
	ACE_Thread_Mutex	m_input_packet_vec_mutex;

	PacketVec_t	m_input_packet_vec;

	ACE_Thread_Mutex	m_cache_input_packet_vec_mutex;

	CacheInputPacketVec_t m_cache_input_packet_vec;

	bool m_success_to_init;

	bool m_is_stop;
};
#endif