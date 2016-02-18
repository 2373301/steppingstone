
#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <boost/lexical_cast.hpp>
#include "typedef.h"
#include "Packet.h"
#include "Logger.h"
#include "Scend_def.h"
#include "Pool.h"
#include "Message.h"
#include "Line.h"
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
#define SEND_PLAYER_MESSAGE(message, packet)  message->playerMsg(packet)

#define MAKE_AND_SEND_PLAYER_MESSAGE(op_code, guid, stream_value)	\
	{	\
		MAKE_NEW_PACKET(ps, op_code, guid, stream_value);	\
		SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);	\
	}

#define NOTIFY_MSG_TO_PLUGINS(message, pack_info)	message->notifyMsgToPlugins(pack_info)

#define MAKE_AND_NOTIFY_MSG_TO_PLUGINS(op_code, guid, msg_entity)	\
	{	\
		PackInfo pi(op_code, guid, msg_entity);	\
		NOTIFY_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);	\
	}

#define REQUEST_MSG_TO_PLUGINS(message, pack_info)	message->requestMsgToPlugins(pack_info)

#define GMCMD_MSG_TO_PLUGINS(message, gm_name, gm_param, target_guid)	message->gmcmdMsgToPlugins(gm_name, gm_param, target_guid)

#define PLUGIN_SCHEME_TIMER(scene, interval_time, call_back_fun)	scene->schemeTimer(interval_time, call_back_fun)

#define GET_RESOURCE_CONTENT(scene, resource_name, resource_content)	scene->loadResource(resource_name, resource_content)

///////////////		manage grid op			////////////////////////////////////////////////////
#define MANAGE_GRID_ADD(manage_grid, player)							manage_grid->add(player)
#define MANAGE_GRID_READD(manage_grid, player)							manage_grid->readd(player)
#define MANAGE_GRID_REMOVE(manage_grid, player)							manage_grid->remove(player)
#define MANAGE_GRID_SEARCE(manage_grid, entity_type, map_id, guids)		manage_grid->search(entity_type, map_id, guids)

//////////////////////////////////////////////////////////////////////////
#define PLUGIN_LOG_DEBUG(log_info, ...)			m_plugin_cfg.logger->log(LL_DEBUG, log_info, ##__VA_ARGS__);
#define PLUGIN_LOG_INFO(log_info, ...)			m_plugin_cfg.logger->log(LL_INFO, log_info, ##__VA_ARGS__);
#define PLUGIN_LOG_WARNING(log_info, ...)		m_plugin_cfg.logger->log(LL_WARNING, log_info, ##__VA_ARGS__);
#define PLUGIN_LOG_TRACE(log_info, ...)			m_plugin_cfg.logger->log(LL_TRACE, log_info, ##__VA_ARGS__);
#define PLUGIN_LOG_ERROR(log_info, ...)			m_plugin_cfg.logger->log(LL_ERROR, log_info, ##__VA_ARGS__);
#define PLUGIN_LOG_FATAL(log_info, ...)			m_plugin_cfg.logger->log(LL_FATAL, log_info, ##__VA_ARGS__);

//////////////////////////////////////////////////////////////////////////
#define GET_ROLE_ATTR(role, role_attr)	getRoleAttr(role, role_attr)

//#define SET_ROLE_ATTR(role, role_attr, attr_value)	setRoleAttr(role, role_attr, attr_value)

#define MODIFY_ROLE_ATTR(scene, role, role_attr, attr_value, notify_client)	scene->modifyRoleAttr(role, role_attr, attr_value, notify_client)

#define MAKE_NEW_GUID(scene, entity_type, guid)	scene->get_guid(entity_type, guid)

//////////////////////////////////////////////////////////////////////////

#define MAKE_MONSTER_GUID(enemy_id, monster_id, monster_pos)	MAKE_GUID(ET_MONSTER, 1, ((uint64(enemy_id) << 28) | (monster_id << 8) | monster_pos))

//////////////////////////////////////////////////////////////////////////

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

#define IS_NOT_LINE_SCENE_RETURN()	\
	if (m_plugin_cfg.template_id != 0)	\
	{	\
		return 0;	\
	}

#define IS_LINE_SCENE()	(0 == m_plugin_cfg.template_id)

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

bool SCENE_EXOPRT extractPacket(Packet * packet, GOOGLE_MESSAGE_TYPE *& protobuf_msg);

struct SCENE_EXOPRT InputMsgInfo 
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

struct SCENE_EXOPRT RequestMsgInfo 
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

struct SCENE_EXOPRT NotifyMsgInfo 
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

struct SCENE_EXOPRT GmcmdMsgInfo 
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

struct SCENE_EXOPRT ServerCfg
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

typedef define_unordered_map<int, GOOGLE_MESSAGE_TYPE *> MessageTypeMap;


class SCENE_EXOPRT ParamConfig
{
public:
	void append(const string & param_name, const string & param_value);

	bool getStr(const string & param_name, string & param_value);

	bool getInt(const string & param_name, int & param_value);

	bool getUint64(const string & param_name, uint64 & param_value);

	void clear();
protected:
private:
	typedef map<string, string> ParamPair_t;

	ParamPair_t m_param_pair;
};

typedef vector<ParamConfig> PluginParamConfigVec_t;

struct SCENE_EXOPRT PluginCfg 
{
	PluginCfg()
	: cache_type(0)
	, line_no(0)
	, template_id(0)
	, scene(0)
	, pool(0)
	, manage_grid(0)
	, message(0)
	, logger(NULL)
	, line_scene(NULL)
	, scene_request(NULL)
	, is_first_launch(false)
	, enable_gm(false)
	, data_record(NULL)
	, cross_server(NULL)
	, manage_container(NULL)
	{}

	int cache_type;
	int line_no;
	int template_id;
	Scene *	scene;
	Pool *	pool;
	ManageGrid * manage_grid;
	Message * message;
	Logger * logger;
	Line * line_scene;
	SceneRequest * scene_request;
	bool is_first_launch;
	bool enable_gm;
	DataRecord * data_record;		// 数据日志接口
	ParamConfig plugin_config;
	ServerCfg server_cfg;
	CrossServer * cross_server;
	ManageContainer * manage_container;
};

struct SCENE_EXOPRT LunchInfo 
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
	PT_MANAGE_ENEMY,
	PT_MANAGE_WAR,
	PT_MANAGE_FORM,
	PT_MANAGE_BUFFER,
	PT_MANAGE_PARTNER,
	PT_MANAGE_ITEM,
	PT_MANAGE_LAW,
	PT_MANAGE_BAG,
	PT_MANAGE_SECT,
	PT_MANAGE_STHEN,
	PT_MANAGE_QUEST,
	PT_MANAGE_NPC,
	PT_MANAGE_PVP,
	PT_MANAGE_PROXY,
	PT_MANAGE_EDUCATE,
	PT_MANAGE_SHOP,
	PT_MANAGE_SHORT,
	PT_MANAGE_CHAKRA,
	PT_MANAGE_MAIL,
	PT_MANAGE_WORSHIP,
	PT_MANAGE_EGGS,
	PT_MANAGE_CONDOR,
	PT_MANAGE_BOSS,
	PT_MANAGE_VIP,
	PT_MANAGE_ANSWER,
	PT_MANAGE_ADDICTION,
	PT_MANAGE_GIFT,
	PT_MANAGE_HANG,
	PT_MANAGE_COPY,
	PT_MANAGE_FRIEND,
	PT_MANAGE_ACHIEVEMENT,
	PT_MANAGE_PRACTICE,
	PT_MANAGE_TRAINING,
	PT_MANAGE_DAILYPOINTS,
	PT_MANAGE_GUILD,
	PT_MANAGE_TOWER,
	PT_MANAGE_ESCORT,
	PT_MANAGE_TAVERN,
	PT_MANAGE_DODGE,
	PT_MANAGE_GUILDBATTLE,
	PT_MANAGE_BATTLEGROUND,
    PT_MANAGE_MASTER,
	PT_MANAGE_WIFE,
	PT_MANAGE_CROSSDATA,
	PT_MANAGE_QUALIFYBATTLEA,
	PT_MANAGE_QUALIFYBATTLE,
	PT_MANAGE_CDPROXY,
	PT_MANAGE_QUALIFYDATA,
	PT_MANAGE_QPROXY,
	PT_MANAGE_ACTIVE_SEN,			// sen中的activite;
	PT_MANAGE_ACTIVITE_LINE,		// line中的activite.
	PT_MANAGE_QUALIFYBATTLEC,
	PT_MANAGE_GEMSET,
    PT_MANAGE_ARREST,
	PT_MANAGE_PELLET,
	PT_MANAGE_MAZE,					// 秘境奇缘
	PT_MANAGE_QUALIFYBATTLEB,
	PT_MANAGE_AMAH,					// 替身娃娃
    PT_MANAGE_OPENACTIVITY,
	PT_MANAGE_OPENACT,				// 开服活动
	PT_MANAGE_OPENACTLINE,
	PT_MANAGE_QUALIFYBATTLELS,		// 武林大会line
    PT_MANAGE_RANKING,              // 排行榜
	PT_MANAGE_TITLE_SEN,			// 称号 sen.
	PT_MANAGE_TITLE_LINE,			// 称号 line
	PT_MANAGE_EXCHANGELINE,			// 切磋 line 
	PT_MANAGE_JIANZHONG,			// 剑冢
	PT_MANAGE_EXCHANGESCENE,		// 切磋 scene
	PT_MANAGE_SPLENDID,				// 五绝
	PT_MANAGE_PUSH,					// 推送系统
	PT_MANAGE_CHARGEQUEST,			// 充值任务
	PT_MANAGE_OPENACT1,				// 开服活动1（vip活动）
	PT_MANAGE_RUNES,				// 符文系统
	PT_MANAGE_TRIAL_LINE,			// 明教试炼Line插件
	PT_MANAGE_TRIAL_SEN,			// 明教试炼sen插件
    PT_MANAGE_SHENGHUOLING,
	PT_MANAGE_OPENACT_NEWKING,		// 新人王活动
	PT_MANAGE_SOUL,					// 武魂系统
    PT_MANAGE_SOULPVE,              // 武魂PVE
	PT_MANAGE_LOGINGIFT,			// 三日登录领奖
	PT_MANAGE_SUPERPOWER,			// 超级战力
	PT_MANAGE_GOLDCARD,				// 特权卡
	PT_MANAGE_PAIR_SCENCE,			//2v2 scence
	PT_MANAGE_PAIR_LINE,			//2v2 line
	PT_MANAGE_PAIR_LOCAL,			//本服赛
	PT_MANAGE_PAIR_LOGIC,			//抽象比赛过程
	PT_MANAGE_PAIR_GLOBAL,			//跨服赛
    PT_MANAGE_TURNTABLE,
	PT_MANAGE_TALENT,				// 资质
	PT_MANAGE_SHOPLINE,				// line shop
	PT_MANAGE_TAVERNLINE,			// 酒馆line
    PT_MANAGE_HORSE,
	PT_MANAGE_QQSPREADLINE,			// qq推广 Line实体
	PT_MANAGE_QQSPREADPLAYER,		// qq推广player实体
};

class SCENE_EXOPRT Plugin
{
public:
	Plugin();

	virtual ~Plugin();

	virtual int init(const PluginCfg & plugin_cfg);

public:
	//virtual int input(const PackInfo & pack_info) = 0;

	//virtual int notify(const PackInfo & pack_info) = 0;

	//virtual int request(const PackInfo & pack_info) = 0;

	PluginCfg & getPluginCfg();

	PluginType getType();

	// 0: line 和 scene都加载，1：仅仅scene加载，2：仅仅line加载，返回值是m_plugin_scene_type，默认为0
	virtual PluginSceneType getSceneType();

	virtual int transferMsgToLineScene(const PackInfo & pack_info);

	virtual int transferMsgToPVPScene(const PackInfo & pack_info);

	virtual bool isStartupSuccess();

	virtual bool isShutdownSuccess();
public:
	virtual const Input_Msg_Type_Map & getInputMsgMap();

	virtual const Request_Msg_Type_Map & getRequestMsgMap();

	virtual const Notify_Msg_Type_Map & getNofityMsgMap();

	virtual const Gmcmd_Msg_Type_Map & getGmcmdMsgMap();
public:
	static bool getProtobufMsgByOpcode(int op_code, GOOGLE_MESSAGE_TYPE *& protobuf_msg);

protected:
	virtual int initing() = 0;
	
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
