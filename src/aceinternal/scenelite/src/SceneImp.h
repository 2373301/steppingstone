
#ifndef SCENE_IMP_HPP
#define SCENE_IMP_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "Scend_def.h"
#include "Scene.h"
#include "PluginDepot.h"
#include "ManageTimer.h"
#include "ManageGridOptimize.h"
#include "MakeGuid.h"
#include "ManageRandom.h"
//#include "ManageBattleReport.h"


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

	virtual Message * getMessage();

public:
	// timer's interface
	virtual long schemeTimer(int interval_value, TimerCallBack timer_callback);

	virtual long cancelTimer(long timer_id);
public:
	virtual bool get_guid(EntityType entity_type, uint64 & guid);

	virtual int get_random(int max_no, int min_no = 0);

	virtual bool savePluginConfig(PluginType plugin_type, int index, char * buffer_stream, int buffer_size);

	virtual bool loadPluginConfig(PluginType plugin_type, int index, char * * buffer_stream, int & buffer_size);

protected:
	virtual void playerMsg(Packet * packet);

	virtual void inlineBroadMsg(Packet * packet);

	virtual void notifyMsgToPlugins(const PackInfo & pack_info);

	virtual bool requestMsgToPlugins(const PackInfo & pack_info);

	virtual bool gmcmdMsgToPlugins(const string & gm_name, const vector<string> & gm_param, uint64 target_guid);

protected:
	void savePacket(Packet * packet);

protected:
	typedef vector<CachePackInfo> CachePackInfoVec_t;

	PluginDepot * m_plugin_depot;


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


	bool m_all_players_flushed_after_lost_gate_connection;


	bool m_is_startup_success;

	bool m_is_stop;

	bool m_is_shutdown_success;

	Uint64Set_t m_need_update_power_player;

};
#endif