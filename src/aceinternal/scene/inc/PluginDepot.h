
#ifndef PLUGIN_DEPOT_HPP
#define PLUGIN_DEPOT_HPP

#include "Scend_def.h"
#include "Plugin.h"
#include "LoadDll.h"
#include "Pool.h"
#include "Scene.h"

class IScene;

struct SCENE_EXOPRT PluginDepotCfg 
{
	PluginDepotCfg()
		: scene(NULL)
		, cache_type(0)
		, line_no(0)
		, template_id(0)
		, pool(NULL)
		, handle_output(NULL)
		, manage_grid(NULL)
		, plugin_param_vec(NULL)
		, logger(NULL)
		, line_scene(NULL)
		, scene_request(NULL)
		, push_client_error_msg(false)
		, is_first_launch(false)
		, enable_gm(false)
		, data_record(NULL)
		, cross_server(NULL)
	{}

	Scene * scene;
	int	cache_type;
	int line_no;
	int template_id;
	Pool * pool;
	HandleOutput * handle_output;
	ManageGrid * manage_grid;
	string plugin_dir;
	PluginParamConfigVec_t * plugin_param_vec;
	Logger * logger;
	Line * line_scene;
	SceneRequest * scene_request;
	bool push_client_error_msg;
	bool is_first_launch;
	bool enable_gm;
	DataRecord * data_record;		// 数据日志接口
	DllInfoVec_t plugin_dll_vec;
	ServerCfg server_cfg;
	CrossServerCfg cross_server_cfg;
	CrossServer * cross_server;
};

class SCENE_EXOPRT PluginDepot
{
public:
	PluginDepot();

	virtual ~PluginDepot();

	virtual int init(const PluginDepotCfg & plugin_depot_cfg);

public:
	virtual int input(Packet * packet) = 0;

	virtual int input(const PackInfo & pack_info) = 0;

	virtual int notify(const PackInfo & pack_info) = 0;

	virtual int request(const PackInfo & pack_info) = 0;

	virtual int gmcmd(const string & gm_name, const vector<string> & gm_param, uint64 target_guid) = 0;

	virtual bool isStartupSuccess() = 0;

	virtual bool isShutdownSuccess() = 0;

protected:
	virtual int initing() = 0;

protected:
	typedef map<PluginType, Plugin *> PluginMap_t;

	PluginDepotCfg	m_plugin_depot_cfg;

	PluginMap_t		m_plugin_map;
};
#endif
