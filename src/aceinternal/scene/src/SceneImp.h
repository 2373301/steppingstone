
#ifndef SCENE_IMP_HPP
#define SCENE_IMP_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "Scend_def.h"
#include "Scene.h"
#include "PluginDepot.h"
#include "ManageTimer.h"
#include "ManageGridOptimize.h"
#include "PacketConvert.h"
#include "MakeGuid.h"
#include "ManageRandom.h"
#include "ManageResourceImp.h"
#include "ManageExchange.h"
//#include "ManageBattleReport.h"

struct OnlinePlayerInfo 
{
	OnlinePlayerInfo()
	: already_online(false)
	, level(1)
	{}

	OnlinePlayerInfo(bool already_online_v, uint32 level_v)
	: already_online(already_online_v)
	, level(level_v)
	{}

	bool already_online;
	uint32 level;
};

class SCENE_EXOPRT SceneImp : public Scene, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	SceneImp();

	~SceneImp();
public:
	virtual int init(const SceneCfg & scene_cfg);

	virtual int startup();

	virtual int stop();

	virtual int finit();

	//virtual void allPlayersFlushedAfterLostGateConnection();

	virtual bool isShutdownSuccess();

	virtual bool isStartupSuccess();

	virtual bool isLineScene();

public:
	// HandleInput's interface
	virtual void input(Packet * packet);

	// CacheHandleInput's interface
	virtual void cacheInput(Packet * packet, uint64 map_id, uint64 request_id);

	void packInput(PackInfo * pack_info);
public:
	/// svc of task
	virtual int svc (void);

public:
	virtual Pool * getPool();

	virtual Message * getMessage();

	virtual ManageResource * getManageResource();
public:
	// timer's interface
	virtual long ITimer_scheme(int interval_value, TimerCallBack timer_callback);

	virtual long ITimer_cancel(long timer_id);
public:
	virtual bool get_guid(EntityType entity_type, uint64 & guid);

	virtual int get_random(int max_no, int min_no = 0);

	virtual int getRoleAttr(typed::entity::Role * role, RoleAttrType role_attr);

	virtual bool modifyRoleAttr(typed::entity::Role * role, RoleAttrType role_attr, int attr_value, bool notify_client);

	virtual bool modifyRoleAttrBySchemeId(typed::entity::Role * role, int scheme_id, int attr_value, bool notify_client);

	virtual bool modifyPlayerAttrBySchemeId(typed::entity::Player * player, typed::entity::Role * role, int scheme_id, int attr_value, bool notify_client);

	virtual int calcRoleFightPower(typed::entity::Role * role);

	virtual bool savePluginConfig(PluginType plugin_type, int index, char * buffer_stream, int buffer_size);

	virtual bool loadPluginConfig(PluginType plugin_type, int index, char * * buffer_stream, int & buffer_size);

	virtual void collectPlayerLevel(uint64 player, uint32 level);

	virtual void playerOnline(uint64 player);

	virtual void playerOffline(uint64 player);

	virtual Uint64Set_t & getNeedUpdatePowerPlayer();
protected:
	virtual void playerMsg(Packet * packet);

	virtual void inlineBroadMsg(Packet * packet);

	virtual void notifyMsgToPlugins(const PackInfo & pack_info);

	virtual bool requestMsgToPlugins(const PackInfo & pack_info);

	virtual bool gmcmdMsgToPlugins(const string & gm_name, const vector<string> & gm_param, uint64 target_guid);

protected:
	PropertyResInfo * getPropertyRes(int scheme_id);

	void savePacket(Packet * packet);

protected:
	typedef vector<CachePackInfo> CachePackInfoVec_t;

	typedef define_unordered_map<uint64, OnlinePlayerInfo *> OnlinePlayerInfoMap_t;

	Pool *	m_pool;

	PluginDepot * m_plugin_depot;

	WarInitResInfo * m_war_init_res_info;

	SystemResInfo * m_system_res_info;

	PropertyResInfoMap_t * m_property_res_info;

	uint32 m_save_packet_index;

	SceneCfg m_scene_cfg;

	// input packet
	ACE_Thread_Mutex	m_input_packet_vec_mutex;

	vector<PackInfo *>	m_input_packet_vec;

	// input cache packet
	ACE_Thread_Mutex	m_cache_input_packet_vec_mutex;

	CachePackInfoVec_t	m_cache_input_packet_vec;

	ManageTimer m_manage_timer;

	typedef map<long, TimerCallBack> TimerCallBackMap_t;

	TimerCallBackMap_t m_timer_callback_map;

	ManageGridOptimize m_manage_grid;

	PacketConvert m_packet_convert;

	MakeGuid m_make_guid;

	ManageRandom m_manage_random;

	static ManageResourceImp m_manage_resource;

	bool m_all_players_flushed_after_lost_gate_connection;

	OnlinePlayerInfoMap_t m_online_player;

	bool m_is_startup_success;

	bool m_is_stop;

	bool m_is_shutdown_success;

	Uint64Set_t m_need_update_power_player;

	//ManageBattleReport m_manage_battle_report;
};
#endif