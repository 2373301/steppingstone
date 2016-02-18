
#ifndef SCENE_DEPOT_HPP
#define SCENE_DEPOT_HPP

#include "typedef.h"
#include "Scend_def.h"
#include "Scene.h"
#include "Session.h"
#include "CacheInterface.h"

struct SCENE_EXOPRT SceneDepotCfg
{
	SceneDepotCfg()
	: gate_type(0)
	, cache_type(0)
	, manage_terminal(NULL)
	, cache_handle_output(NULL)
	, save_packet(false)
	, push_client_error_msg(false)
	, is_first_launch(false)
	, startup_notify_port(24000)
	, max_sceen_number(100)
	, log_dir("logs")
	, enable_gm(false)
	{}

	typedef vector<SceneCfg> SceneCfgVec_t;

	int gate_type;

	int cache_type;

	ManageTerminal * manage_terminal;

	CacheHandleOutput * cache_handle_output;

	bool save_packet;

	bool push_client_error_msg;

	bool is_first_launch;

	int startup_notify_port;

	int max_sceen_number;

	LocalAddrCfg local_addr;

	string plugin_dir;

	string log_dir;

	bool enable_gm;


	SceneCfgVec_t scenes_cfg;



	string record_server_addr;

	string gm_server_addr;

	ServerCfg server_cfg;



	string vip_init_security_key;

	string robot_security_key;
};

class SCENE_EXOPRT SceneDepot : public HandleInput, public CacheHandleInput
{
public:
	virtual ~SceneDepot()
	{}

	virtual int init(const SceneDepotCfg & scene_depot_cfg) = 0;

	virtual int stop() = 0;

	virtual int finit() = 0;

protected:
private:
	
};
#endif
