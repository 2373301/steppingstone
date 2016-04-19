
#ifndef SCENE_HPP
#define SCENE_HPP

#include <boost/array.hpp>
#include "Packet.h"
#include "Pool.h"
#include "Message.h"
#include "Session.h"
#include "LoadDll.h"
#include "Plugin.h"
#include "entity_ins.h"
#include "entity/player.pb.h"

#include "protocol_ins.h"
#include "protocol/msg_player.pb.h"
#include "ManageTerminal.h"
#include "Resource.h"

class Line;

struct SCENE_EXOPRT ResourceCfg 
{
	ResourceCfg()
	{}

	string resource_dir;
};

struct SCENE_EXOPRT GuidSvrCfg 
{
	GuidSvrCfg()
	{}

	string guid_addr;
};

class LineScene;

struct ExchangeCfg
{
	ExchangeCfg()
		: line_id(0)
		, line_scene(NULL)
	{}

	int line_id;
	LineScene * line_scene;
	string exchange_addr;
	string security_key;
};

struct BattleReportCfg 
{
	BattleReportCfg()
	{}

	string addr;
	string security_key;
};

struct CrossServerCfg
{
	CrossServerCfg()
	: server_type(0)
	{}

	// 1:normal server, 2:cross server
	uint32 server_type;
	string cross_server_addr;
	string security_key;
};

struct SCENE_EXOPRT SceneCfg 
{
	SceneCfg()
		: scene_id(0)
		, map_id(0)
		, original_map_id(0)
		, line_no(0)
		, cache_type(0)
		, manage_terminal(NULL)
		, cache_handle_output(NULL)
		, plugin_param_vec(NULL)
		, save_packet(false)
		, packet_dir("packet")
		, logger(NULL)
		, line_scene(NULL)
		, scene_request(NULL)
		, push_client_error_msg(false)
		, is_first_launch(false)
		, max_sceen_number(100)
		, log_dir("logs")
		, enable_gm(false)
		, data_record(NULL)
		, cross_server(NULL)
	{}

	int scene_id;

	int line_no;

	uint64 map_id;

	uint32 original_map_id;

	int	cache_type;

	ManageTerminal * manage_terminal;

	CacheHandleOutput * cache_handle_output;

	PluginParamConfigVec_t * plugin_param_vec;

	bool save_packet;

	string packet_dir;

	Logger * logger;

	Line * line_scene;

	SceneRequest * scene_request;

	bool push_client_error_msg;

	bool is_first_launch;

	int max_sceen_number;

	string log_dir;

	bool enable_gm;

	DataRecord * data_record;		// 数据日志接口

	ExchangeCfg exchange_cfg;

	DllInfoVec_t plugin_dll_vec;

	ResourceCfg resource_cfg;

	GuidSvrCfg guid_svr_cfg;

	Uint64Vec_t map_id_vec;

	string record_server_addr;

	string gm_server_addr;

	ServerCfg server_cfg;

	CrossServerCfg cross_server_cfg;

	BattleReportCfg battle_report_cfg;

	CrossServer * cross_server;

	string vip_init_security_key;

	string robot_security_key;
};

class CrossServerInput
{
public:
	virtual void crossServerInput(Packet * packet) = 0;
};

class SCENE_EXOPRT Scene : public HandleInput, public CacheHandleInput, public ITimer, public Message
{
public:
	virtual ~Scene()
	{}

	virtual int init(const SceneCfg & scene_cfg) = 0;

	virtual int startup() = 0;

	virtual int stop() = 0;

	virtual int finit() = 0;

	virtual bool isStartupSuccess() = 0;

	virtual bool isShutdownSuccess() = 0;

	virtual bool isLineScene() = 0;
public:
	virtual void input(Packet * packet) = 0;

public:
	virtual Pool * getPool() = 0;

	virtual Message * getMessage() = 0;

	virtual ManageResource * getManageResource() = 0;
public:
	virtual long ITimer_scheme(int interval_value, TimerCallBack timer_callback) = 0;

	virtual long ITimer_cancel(long timer_id) = 0;

public:
	virtual bool get_guid(EntityType entity_type, uint64 & guid) = 0;

	virtual int get_random(int max_no, int min_no = 0) = 0;

	virtual int getRoleAttr(typed::entity::Role * role, RoleAttrType role_attr) = 0;

	virtual bool modifyRoleAttr(typed::entity::Role * role, RoleAttrType role_attr, int attr_value, bool notify_client) = 0;

	virtual bool modifyRoleAttrBySchemeId(typed::entity::Role * role, int scheme_id, int attr_value, bool notify_client) = 0;

	virtual bool modifyPlayerAttrBySchemeId(typed::entity::Player * player, typed::entity::Role * role, int scheme_id, int attr_value, bool notify_client) = 0;

	virtual int calcRoleFightPower(typed::entity::Role * role) = 0;

	// 存储plugin配置，比如世界boss前十名信息等
	virtual bool savePluginConfig(PluginType plugin_type, int index, char * buffer_stream, int buffer_size) = 0;

	// 加载plugin配置信息，如果加载成功，将为buffer_stream分配内存保存数据
	virtual bool loadPluginConfig(PluginType plugin_type, int index, char * * buffer_stream, int & buffer_size) = 0;

	virtual void collectPlayerLevel(uint64 player, uint32 level) = 0;

	virtual void playerOnline(uint64 player) = 0;

	virtual void playerOffline(uint64 player) = 0;

	virtual Uint64Set_t & getNeedUpdatePowerPlayer() = 0;
protected:
private:
};

#endif
