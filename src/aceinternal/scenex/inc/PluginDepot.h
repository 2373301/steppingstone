
#ifndef PLUGIN_DEPOT_HPP
#define PLUGIN_DEPOT_HPP

#include "Scenex_def.h"
#include "Plugin.h"
#include "LoadDll.h"
#include "Pool.h"
#include "Scene.h"

class IScene;

struct SCENEX_EXOPRT PluginDepotCfg
{
	IScene * scene = NULL;
	Pool * pool = NULL;
	//HandleOutput * handle_output;
	//ManageGrid * manage_grid;
	string plugin_dir;
	PluginParamConfigVec_t * plugin_param_vec = NULL;
	Logger * logger = NULL;
	//SceneRequest * scene_request;
	bool push_client_error_msg;
	bool is_first_launch;
	bool enable_gm;
	DllInfoVec_t plugin_dll_vec;
	//ServerCfg server_cfg;
};

class SCENEX_EXOPRT IPluginDepot
{
public:
	IPluginDepot();

	virtual ~IPluginDepot();

	virtual int IPluginDepot_init(const PluginDepotCfg & plugin_depot_cfg);

public:
	virtual int IPluginDepot_input(Packet * packet) = 0;

	virtual int IPluginDepot_input(const PackInfo & pack_info) = 0;

	virtual int IPluginDepot_notify(const PackInfo & pack_info) = 0;

	virtual int IPluginDepot_request(const PackInfo & pack_info) = 0;

	virtual int IPluginDepot_gmcmd(const string & gm_name, const vector<string> & gm_param, uint64 target_guid) = 0;

	virtual bool IPluginDepot_isStartupSuccess() = 0;

	virtual bool IPluginDepot_isShutdownSuccess() = 0;

protected:
	virtual int IPluginDepot_initing() = 0;

protected:
	typedef map<PluginType, IPlugin *> PluginMap_t;

	PluginDepotCfg	m_plugin_depot_cfg;

	PluginMap_t		m_plugin_map;
};
#endif
