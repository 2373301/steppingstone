
#ifndef LINE_HPP
#define LINE_HPP

#include "typedef.h"
#include "coredef.h"
#include "Packet.h"
#include "Scend_def.h"
#include "Pool.h"
#include "entity/short.pb.h"
#include "share_fun.h"

struct PluginCfg;

struct LunchInfo;

class Scene;

typedef define_unordered_map<uint64, MSG_TYPE *> GuidMsgMap_t;

struct SCENE_EXOPRT TransferPlayerInfo
{
	TransferPlayerInfo()
		: player_guid(0)
		, player(NULL)
		, target_map_id(0)
	{}

	void putMsg(uint64 guid, MSG_TYPE * msg)
	{
		msg_map.insert(std::make_pair(guid, msg));
	}

	uint64 player_guid;
	MSG_TYPE * player;
	uint32 target_map_id;
	GuidMsgMap_t msg_map;
};

struct SCENE_EXOPRT PlayerBriefInfo
{
	PlayerBriefInfo()
		: player(0)
		, is_online(false)
		, scene_id(0)
		, is_jumpmap(false)
		, short_info(NULL)
	{}

	PlayerBriefInfo(uint64 pguid, typed::entity::Short * short_info, uint32 s_id, bool on_line)
		: player(pguid)
		, is_online(on_line)
		, scene_id(s_id)
		, is_jumpmap(false)
		, short_info(short_info)
	{}

	uint64	player;
	bool	is_online;
	uint32  scene_id;
	bool	is_jumpmap;
	typed::entity::Short * short_info;
};

struct CollectMsgInfo 
{
	CollectMsgInfo()
		: player_guid(0)
	{}

	~CollectMsgInfo()
	{
		cleanMapSecondContent(msg_map);
	}

	void addMsg(uint64 guid, MSG_TYPE * msg)
	{
		msg_map.insert(std::make_pair(guid, msg));
		guid_vec.push_back(guid);
	}

	void clear()
	{
		msg_map.clear();
		guid_vec.clear();
	}

	uint64 player_guid;
	GuidMsgMap_t msg_map;
	Uint64Vec_t guid_vec;
};

class SCENE_EXOPRT CrossServer
{
public:
	// for normal server
	virtual void sendCrossServerMsg(Packet * packet) = 0;

	// for cross server
	virtual void sendLineServerMsg(const string & line_id, const string & platform, Packet * packet) = 0;

	// broadcast message from cross server to other line sever
	virtual void broadLineServerMsg(Packet * packet) = 0;

	virtual bool isOnline(const string & platform, const string & line_id) = 0;

	//virtual void addPlayerRoute(const string & line_id, const string & platform, uint64 player_guid) = 0;
};

class SCENE_EXOPRT ManageContainer
{
public:
	typedef define_unordered_set<MSG_TYPE *> MsgSet_t;

	virtual ~ManageContainer()
	{};
	virtual bool isRegister(EntityType et_type) = 0;
	virtual bool registerEntityType(EntityType et_type) = 0;
	virtual bool addGuid(EntityType et_type, uint64 entity_guid) = 0;
	virtual bool removeGuid(EntityType et_type, uint64 guid) = 0;

	// call this function only once when startup
	virtual MsgSet_t getEntityMessages(EntityType et_type) = 0;
	virtual Uint64Vec_t getContainerGuids(EntityType et_type) = 0;
	virtual bool isAddedGuid(EntityType et_type, uint64 entity_guid) = 0;

	virtual bool addContainer(EntityType et_type, MSG_TYPE * ent_msg) = 0;
	virtual bool addContainerEntity(EntityType et_type, MSG_TYPE * ent_msg) = 0;
	virtual Uint64Set_t getUpdateGuids() = 0;
	virtual MsgSet_t getAddedContainer() = 0;
};

class SCENE_EXOPRT Line
{
public:
	virtual ~Line()
	{}

	// manage scene
	virtual int addScene(uint32 scene_id, Scene * scene) = 0;

	virtual Scene * getScene(uint32 scene_id) = 0;

	// player route
	virtual void addPlayerShortInfo(uint64 player, typed::entity::Short * short_info, uint32 map_id, bool is_online) = 0;

	virtual void updatePlayerShortInfo(uint64 player, typed::entity::Short * short_info) = 0;

	virtual bool isCrossServer() = 0;

	//virtual void playerLogin(uint64 player, uint32 map_id, int curr_level) = 0;

	//virtual void playerLogoff(uint64 player) = 0;

	//virtual void playerLevelup(uint64 player, int curr_level) = 0;

	// get by guid
	virtual PlayerBriefInfo * getPlayerBriefInfo(uint64 player) = 0;

	// get all vip players.
	virtual uint32 getAllVipShort(Uint64Vec_t &vec64) = 0;
	// return true if player already entered world
	virtual bool playerEnteredWorld(uint64 player) = 0;

	// get by name
	virtual PlayerBriefInfo * getPlayerBriefInfoByName(const string & player_name) = 0;

	virtual void playerLogin(uint64 player_guid) = 0;

	virtual void playerLogoff(uint64 player_guid) = 0;

	virtual Uint64Set_t getOnlinePlayers() = 0;

	// transfer player between scenes
	virtual void transferPlayer(TransferPlayerInfo * transfer_player_info) = 0;

	virtual TransferPlayerInfo * obtainPlayer(uint64 player, uint32 map_id) = 0;

	virtual const Uint64Set_t & getTransferPlayerSet() = 0;

	// get player fight data, return player scene id
	virtual uint32 getPlayerFightData(uint64 player_guid, TransferPlayerInfo ** trans_player_info) = 0;

	// 发送消息到指定场景
	virtual bool sendSceneMsg(uint32 scene_id, PackInfo * pack_info) = 0;


	// 发送指定消息到player所在场景
	virtual bool sendScenePlayerMsg(uint64 player, PackInfo * pack_info) = 0;

	// 广播消息到所有地图场景，不包括line scene
	virtual bool broadCastSceneMsg(PackInfo * pack_info) = 0;

	// 
	virtual void scenePackInput(PackInfo * pack_info) = 0;

	//
	virtual void transmitPlayerMsg(PackInfo * pack_info) = 0;

	virtual void collectMessage(uint64 player_guid, CollectMsgInfo * collect_msg_info) = 0;

	virtual CollectMsgInfo * getCollectMessage(uint64 player_guid) = 0;

	virtual int sendExchangeMsg(Packet * packet) = 0;

	// 收集所有在线玩家门派信息
	virtual void collectPlayerSect(uint32 sect_id, uint64 player_guid) = 0;

	virtual void removePlayerSect(uint32 sect_id, uint64 player_guid) = 0;

	virtual Uint64Set_t getPlayerBySect(uint32 sect_id) = 0;

	// for guild battle
	// 设置玩家可进入帮会战地图
	virtual void setPlayerMapQualification(uint32 map_id, uint64 player_guid) = 0;
	// 验证玩家是否可以进入帮会战地图
	virtual bool validatePlayerMapQualification(uint32 map_id, uint64 player_guid) = 0;
	// 清除玩家帮会战地图信息
	virtual void clearPlayerMapQualification(uint32 map_id, uint64 player_guid) = 0;

	virtual void clearPlayerMapAllQualification(uint32 map_id) = 0;

	// 
	virtual void sendMasterMsg(Packet * packet) = 0;

	// for battle ground
	virtual void collectBattleGroundPlayer(uint64 player_guid, CollectMsgInfo * collect_msg_info) = 0;

	virtual CollectMsgInfo * getBattleGroundPlayer(uint64 player_guid) = 0;

	virtual bool removeBattleGroundPlayer(uint64 player_guid) = 0;

	virtual void removeBattleGroundPlayers() = 0;

	// for battle report
	virtual void sendBattleReportMsg(Packet * packet) = 0;

	// 
	virtual uint64 getBattleReportId() = 0;

	//
	virtual string getVipInitSecurityKey() = 0;

	//
	virtual string getRobotSecurityKey() = 0;

	// 
	virtual CollectMsgInfo * collectPlayerFightDataMsgInfo(PluginCfg & plugin_cfg, typed::entity::Player * player) = 0;

	// for forbid speak
	virtual void addForbidPlayer(uint64 player_guid) = 0;

	virtual void removeForbidPlayer(uint64 player_guid) = 0;

	// 返回true，表示禁止玩家说话
	virtual bool isForbidSpeak(uint64 player_guid) = 0;

	// 获取当前开服日
	virtual LunchInfo & getLunchInfo() = 0;

	virtual ManageContainer * getManageContainer() = 0;

	virtual void broadcastPacketToAllOnlinePlayers(Packet * packet) = 0;

	// 
	virtual void collectCrossPlayerData(const string & line_id, const string & platform, uint64 player_guid, uint64 entity_guid, const string & entity_stream) = 0;

	virtual void collectCrossPlayerDataFinish(const string & line_id, const string & platform, uint64 player_guid) = 0;

	virtual void deliveryPlayerDataToAllGS() = 0;

	virtual void deliveryPlayerDataToGS(const string & platform, const string & line_id) = 0;

	virtual void getCrossPlayerDataInfo(uint64 player_guid, map<uint64, MSG_TYPE *> & msg_map) = 0;

protected:
private:
};

struct SceneRequestCfg 
{
	SceneRequestCfg()
	: scene(NULL)
	, line(NULL)
	, pool(NULL)
	, logger(NULL)
	{}

	Scene * scene;
	Line * line;
	Pool * pool;
	Logger * logger;
};

class SCENE_EXOPRT SceneRequest
{
public:
	~SceneRequest(){}

	// the param is request id
	typedef boost::function<void(uint32, bool)>	FightDataRequestCallBack;

	virtual int init(SceneRequestCfg scene_request_cfg) = 0;

	// about player fight player
	// return request id, it is failed of returning 0
	virtual uint32 requestPlayerFightData(const Uint64Vec_t &  player_guid, FightDataRequestCallBack fight_data_request_callback) = 0;		// return request id

	virtual void playerFightDataBackFromScene(uint64 player_guid, bool is_success) = 0;

	virtual void playerFightDataBackFromCache(uint64 player_guid, bool load_success, CollectMsgInfo * collect_msg_info) = 0;
};

#endif