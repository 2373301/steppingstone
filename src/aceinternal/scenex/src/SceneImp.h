
#ifndef SCENE_IMP_HPP
#define SCENE_IMP_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "Scenex_def.h"
#include "Scene.h"
//#include "PluginDepot.h"
#include "ManageTimer.h"
//#include "ManageGridOptimize.h"
//#include "MakeGuid.h"
#include "ManageRandom.h"


class SCENEX_EXOPRT SceneImp
	: public Scene
	, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	SceneImp();
	~SceneImp();

	virtual int init(const SceneCfg & scene_cfg) override;

	virtual int startup() override;
	virtual int stop() override;
	virtual int finit() override;

	virtual bool isShutdownSuccess() override;
	virtual bool isStartupSuccess() override;


	virtual void input(Packet * packet) override;
	virtual void cacheInput(Packet * packet, uint64 map_id, uint64 request_id);

	virtual long schemeTimer(int interval_value, TimerCallBack timer_callback) override;
	virtual long cancelTimer(long timer_id) override;

	virtual bool get_guid(EntityType entity_type, uint64 & guid) override;
	virtual int get_random(int max_no, int min_no = 0) override;


	void packInput(PackInfo * pack_info);

	virtual int svc (void) override;

protected:
	virtual void playerMsg(Packet * packet) override;
	virtual void inlineBroadMsg(Packet * packet) override;
	virtual void notifyMsgToPlugins(const PackInfo & pack_info) override;
	virtual bool requestMsgToPlugins(const PackInfo & pack_info) override;
	virtual bool gmcmdMsgToPlugins(const string & gm_name, const vector<string> & gm_param, uint64 target_guid) override;

protected:
	void savePacket(Packet * packet);

protected:
	//typedef vector<CachePackInfo> CachePackInfoVec_t;

	PluginDepot * m_plugin_depot;


	uint32 m_save_packet_index;

	SceneCfg m_scene_cfg;

	// input packet
	ACE_Thread_Mutex	m_input_packet_vec_mutex;

	vector<PackInfo *>	m_input_packet_vec;

	// input cache packet
	ACE_Thread_Mutex	m_cache_input_packet_vec_mutex;

	//CachePackInfoVec_t	m_cache_input_packet_vec;

	ManageTimer m_manage_timer;

	typedef map<long, TimerCallBack> TimerCallBackMap_t;

	TimerCallBackMap_t m_timer_callback_map;

	//ManageGridOptimize m_manage_grid;

	//PacketConvert m_packet_convert;

	//MakeGuid m_make_guid;

	ManageRandom m_manage_random;
	bool m_is_startup_success;

	bool m_is_stop;

	bool m_is_shutdown_success;

	Uint64Set_t m_need_update_power_player;

};
#endif