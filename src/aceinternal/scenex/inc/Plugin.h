
#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <boost/lexical_cast.hpp>
#include "typedef.h"
#include "Packet.h"
#include "Logger.h"
#include "Scenex_def.h"
//#include "Pool.h"
#include "Message.h"
//#include "Line.h"
#include "logger_category.h"

/////////////////////	input msg map		/////////////////////////////////////////////////////
#define BEGING_INPUT_MSG_MAP() \
	const Input_Msg_Type_Map & getInputMsgMap()	\
	{	\
		if (m_input_msg_type_map.size() == 0)	\
		{

#define INPUT_HANDLE_MSG(op_code, fun, msg_type_ins)	\
			m_input_msg_type_map[op_code] = InputMsgInfo((Input_Msg_Fun)fun, this);	\
			m_message_type_map[op_code] = msg_type_ins;

#define TRANSFER_MSG_TO_LS(op_code, msg_type_ins)	\
	m_input_msg_type_map[op_code] = InputMsgInfo((Input_Msg_Fun)&Plugin::transferMsgToLineScene, this);	\
	m_message_type_map[op_code] = msg_type_ins;

#define TRANSFER_MSG_TO_PVPSCENE(op_code, msg_type_ins)	\
	m_input_msg_type_map[op_code] = InputMsgInfo((Input_Msg_Fun)&Plugin::transferMsgToPVPScene, this);	\
	m_message_type_map[op_code] = msg_type_ins;

#define END_INPUT_MSG_MAP() \
		}	\
		return m_input_msg_type_map;	\
	}

/////////////////////	request msg map		/////////////////////////////////////////////////////
#define BEGIN_REQUEST_MSG_MAP()	\
	const Request_Msg_Type_Map & getRequestMsgMap()	\
	{	\
	if (m_request_msg_type_map.size() == 0)	\
		{

#define REQUEST_HANDLE_MSG(op_code, fun)	\
	m_request_msg_type_map[op_code] = RequestMsgInfo((Request_Msg_Fun)fun, this);

#define END_REQUEST_MSG_MAP()	\
		}	\
		return m_request_msg_type_map;	\
	}

/////////////////////	notify msg map		/////////////////////////////////////////////////////
#define BEGING_NOTIFY_MSG_MAP() \
	const Notify_Msg_Type_Map & getNofityMsgMap()	\
	{	\
		if (m_notify_msg_type_map.size() == 0)	\
		{

#define NOTIFY_HANDLE_MSG(op_code, fun)	\
			m_notify_msg_type_map[op_code].push_back(NotifyMsgInfo((Notify_Msg_Fun)fun, this));

#define END_NOTIFY_MSG_MAP()	\
		}	\
		return m_notify_msg_type_map;	\
	}

//////////////////// gm command msg map////////////////////////////////////////
#define BEGING_GMCMD_MSG_MAP() \
	const Gmcmd_Msg_Type_Map & getGmcmdMsgMap()	\
	{	\
	if (m_gmcmd_msg_type_map.size() == 0)	\
		{

#define GMCMD_HANDLE_MSG(op_code, fun)	\
	m_gmcmd_msg_type_map[op_code] = GmcmdMsgInfo((Gmcmd_Msg_Fun)fun, this);

#define END_GMCMD_MSG_MAP()	\
		}	\
		return m_gmcmd_msg_type_map;	\
	}

///////////////		message op			///////////////////////////////////////////////////////////
#define SEND_PLAYER_MESSAGE(message, packet)  message->IMessage_player(packet)

#define MAKE_AND_SEND_PLAYER_MESSAGE(op_code, guid, stream_value)	\
	{	\
		MAKE_NEW_PACKET(ps, op_code, guid, stream_value);	\
		SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);	\
	}

#define NOTIFY_MSG_TO_PLUGINS(message, pack_info)	message->IMessage_notifyToPlugins(pack_info)

#define MAKE_AND_NOTIFY_MSG_TO_PLUGINS(op_code, guid, msg_entity)	\
	{	\
		PackInfo pi(op_code, guid, msg_entity);	\
		NOTIFY_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);	\
	}

#define REQUEST_MSG_TO_PLUGINS(message, pack_info)	message->IMessage_requestToPlugins(pack_info)

#define GMCMD_MSG_TO_PLUGINS(message, gm_name, gm_param, target_guid)	message->IMessage_gmcmdToPlugins(gm_name, gm_param, target_guid)

#define PLUGIN_SCHEME_TIMER(scene, interval_time, call_back_fun)	scene->schemeTimer(interval_time, call_back_fun)

#define GET_RESOURCE_CONTENT(scene, resource_name, resource_content)	scene->loadResource(resource_name, resource_content)

///////////////		manage grid op			////////////////////////////////////////////////////
#define MANAGE_GRID_ADD(manage_grid, player)							manage_grid->add(player)
#define MANAGE_GRID_READD(manage_grid, player)							manage_grid->readd(player)
#define MANAGE_GRID_REMOVE(manage_grid, player)							manage_grid->remove(player)
#define MANAGE_GRID_SEARCE(manage_grid, entity_type, map_id, guids)		manage_grid->search(entity_type, map_id, guids)

#define PLUGIN_LOG_DEBUG(log_info, ...)			m_plugin_cfg.logger->log(LL_DEBUG, log_info, ##__VA_ARGS__);
#define PLUGIN_LOG_INFO(log_info, ...)			m_plugin_cfg.logger->log(LL_INFO, log_info, ##__VA_ARGS__);
#define PLUGIN_LOG_WARNING(log_info, ...)		m_plugin_cfg.logger->log(LL_WARNING, log_info, ##__VA_ARGS__);
#define PLUGIN_LOG_TRACE(log_info, ...)			m_plugin_cfg.logger->log(LL_TRACE, log_info, ##__VA_ARGS__);
#define PLUGIN_LOG_ERROR(log_info, ...)			m_plugin_cfg.logger->log(LL_ERROR, log_info, ##__VA_ARGS__);
#define PLUGIN_LOG_FATAL(log_info, ...)			m_plugin_cfg.logger->log(LL_FATAL, log_info, ##__VA_ARGS__);


#define MAKE_NEW_GUID(scene, entity_type, guid)	scene->get_guid(entity_type, guid)
#define MAKE_MONSTER_GUID(enemy_id, monster_id, monster_pos)	MAKE_GUID(ET_MONSTER, 1, ((uint64(enemy_id) << 28) | (monster_id << 8) | monster_pos))


#define ERROR_TO_PRINT_FUNCTION_LINE(str_info)	\
	PLUGIN_LOG_ERROR("occur error, function is <%s>, line is <%d>, info is <%s>\n", __FUNCTION__, __LINE__, str_info)

#define DEBUG_TO_PRINT_FUNCTION_LINE(str_info)	\
	PLUGIN_LOG_DEBUG("debug info, function is <%s>, line is <%d>, info is <%s>\n", __FUNCTION__, __LINE__, str_info)

#define CHECK_NULL_POINT_RETURN(p, result)	\
	if (NULL == (p))	\
		return result;

#define CHECK_NULL_POINT_WITH_LOG_RETURN(p, result)	\
	if (NULL == (p))	\
	{	\
		PLUGIN_LOG_ERROR("failed to check point <%s>, function is <%s>, line is <%d>\n", #p, __FUNCTION__, __LINE__);	\
		return result;	\
	}

#define CHECK_NULL_POINT_WITH_LOG_CMD(p, command)	\
	if (NULL == (p))	\
	{	\
		PLUGIN_LOG_ERROR("failed to check point <%s>, function is <%s>, line is <%d>\n", #p, __FUNCTION__, __LINE__);	\
		command;	\
	}

#define CHECK_RESULT_WITH_LOG_RETURN(res, result)	\
	if (-1 == (res))	\
	{	\
	PLUGIN_LOG_ERROR("failed to check result <%s>, function is <%s>, line is <%d>\n", #res, __FUNCTION__, __LINE__);	\
	return result;	\
	}

#define CHECK_BOOLEAN_WITH_LOG_RETURN(res, result)	\
	if (!(res))	\
	{	\
		PLUGIN_LOG_ERROR("failed to check boolean <%s>, function is <%s>, line is <%d>\n", #res, __FUNCTION__, __LINE__);	\
		return result;	\
	}

#define MAKE_AND_SEND_ERROR_MESSAGE(guid, error_code)	\
	{	\
		typed::protected::smsg_error_message b_e_msg_;	\
		b_e_msg_.set_msg_code(error_code);	\
		MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_ERROR_MESSAGE, guid, b_e_msg_.SerializeAsString());	\
	}

class PluginDepot;
class Scene;
class Plugin;

typedef Plugin * (*MAKE_PLUGIN_INSTANCE)();


typedef int (Plugin::*Input_Msg_Fun)(const PackInfo & pack_info);

typedef int (Plugin::*Request_Msg_Fun)(const PackInfo & pack_info);

typedef int (Plugin::*Notify_Msg_Fun)(const PackInfo & pack_info);

typedef int (Plugin::*Gmcmd_Msg_Fun)(const vector<string> & gm_param, uint64 target_guid);

bool SCENEX_EXOPRT extractPacket(Packet * packet, MSG_TYPE *& protobuf_msg);

struct SCENEX_EXOPRT InputMsgInfo 
{
	InputMsgInfo()
	: msg_fun(NULL)
	, plugin_object(NULL)
	{}

	
	InputMsgInfo(Input_Msg_Fun mf, Plugin * po)
	: msg_fun(mf)
	, plugin_object(po)
	{}

	Input_Msg_Fun msg_fun;

	Plugin * plugin_object;
};

struct SCENEX_EXOPRT RequestMsgInfo 
{
	RequestMsgInfo()
		: msg_fun(NULL)
		, plugin_object(NULL)
	{}


	RequestMsgInfo(Request_Msg_Fun mf, Plugin * po)
		: msg_fun(mf)
		, plugin_object(po)
	{}

	Request_Msg_Fun msg_fun;

	Plugin * plugin_object;
};

struct SCENEX_EXOPRT NotifyMsgInfo 
{
	NotifyMsgInfo()
		: msg_fun(NULL)
		, plugin_object(NULL)
	{}


	NotifyMsgInfo(Notify_Msg_Fun mf, Plugin * po)
		: msg_fun(mf)
		, plugin_object(po)
	{}

	Notify_Msg_Fun msg_fun;

	Plugin * plugin_object;
};

struct SCENEX_EXOPRT GmcmdMsgInfo 
{
	GmcmdMsgInfo()
		: msg_fun(NULL)
		, plugin_object(NULL)
	{}


	GmcmdMsgInfo(Gmcmd_Msg_Fun mf, Plugin * po)
		: msg_fun(mf)
		, plugin_object(po)
	{}

	Gmcmd_Msg_Fun msg_fun;

	Plugin * plugin_object;
};

struct SCENEX_EXOPRT ServerCfg
{
	ServerCfg()
		: platform_id(0)
		, report_id(0)
	{}

	int   platform_id;
	int	  report_id;
	string server_id;
	string platform;
	string curr_version;
};

typedef define_unordered_map<int, InputMsgInfo> Input_Msg_Type_Map;

typedef define_unordered_map<int, RequestMsgInfo> Request_Msg_Type_Map;

typedef define_unordered_map<int, vector<NotifyMsgInfo> > Notify_Msg_Type_Map;

typedef define_unordered_map<string, GmcmdMsgInfo> Gmcmd_Msg_Type_Map;

typedef define_unordered_map<int, MSG_TYPE *> MessageTypeMap;


class SCENEX_EXOPRT ParamConfig
{
public:
	void append(const string & param_name, const string & param_value);

	bool getStr(const string & param_name, string & param_value);

	bool getInt(const string & param_name, int & param_value);

	bool getUint64(const string & param_name, uint64 & param_value);

	void clear();

private:
	typedef map<string, string> ParamPair_t;

	ParamPair_t m_param_pair;
};

typedef vector<ParamConfig> PluginParamConfigVec_t;

struct SCENEX_EXOPRT PluginCfg 
{
	PluginCfg()
	: cache_type(0)
	, line_no(0)
	, template_id(0)
	//, scene(0)
	//, pool(0)
	//, manage_grid(0)
	, message(0)
	, logger(NULL)
	//, line_scene(NULL)
	//, scene_request(NULL)
	, is_first_launch(false)
	, enable_gm(false)
	//, data_record(NULL)
	//, cross_server(NULL)
	//, manage_container(NULL)
	{}

	int cache_type;
	int line_no;
	int template_id;
	//Scene *	scene;
	//Pool *	pool;
	//ManageGrid * manage_grid;
	IMessage * message;
	Logger * logger;
	//Line * line_scene;
	//SceneRequest * scene_request;
	bool is_first_launch;
	bool enable_gm;
	//DataRecord * data_record;		// 数据日志接口
	ParamConfig plugin_config;
	ServerCfg server_cfg;
	//CrossServer * cross_server;
	//ManageContainer * manage_container;
};

struct SCENEX_EXOPRT LunchInfo 
{
	LunchInfo()
	: first_lunch_time(0)
	, current_lunch_time(0)
	, lunch_day(0)
	{}

	uint64 first_lunch_time;		// 第一次开服时间，秒
	uint64 current_lunch_time;		// 当前开服时间，秒
	uint64 lunch_day;				// 当前开服第几天
	uint64 first_lunch_day_zero_time;	// 开服第一天零点时间，主要用于跟开服有关的时间计算
};

enum PluginSceneType
{
	PST_BOTH			= 0,
	PST_ONLY_SCENE		= 1,
	PST_ONLY_LINE		= 2,
};

enum PluginType
{
	PT_BASE = 0,
	PT_MANAGE_PLAYER,
	PT_MANAGE_MOVE,
	PT_MANAGE_CHAR,
};

class SCENEX_EXOPRT Plugin
{
public:
	Plugin();
	virtual ~Plugin();

	virtual int init(const PluginCfg & plugin_cfg);
	virtual bool isStartupSuccess();
	virtual bool isShutdownSuccess();
protected:
	virtual int initing() = 0;

public:
	PluginCfg & getPluginCfg();
	PluginType getType();
	PluginSceneType getSceneType();

	int transferMsgToLineScene(const PackInfo & pack_info);
	int transferMsgToPVPScene(const PackInfo & pack_info);

	static bool getProtobufMsgByOpcode(int op_code, MSG_TYPE *& protobuf_msg);

protected:
	static MessageTypeMap m_message_type_map;

	PluginType	m_plugin_type;
	PluginSceneType m_plugin_scene_type;
	PluginCfg m_plugin_cfg;

	Input_Msg_Type_Map m_input_msg_type_map;
	Request_Msg_Type_Map m_request_msg_type_map;
	Notify_Msg_Type_Map m_notify_msg_type_map;
	Gmcmd_Msg_Type_Map m_gmcmd_msg_type_map;

	bool m_startup_success;
	bool m_shutdown_success;
};

#endif
