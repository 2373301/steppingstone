

#ifndef LINE_SCENE_HPP
#define LINE_SCENE_HPP

#include <ace/Thread_Mutex.h>
#include "SceneImp.h"
#include "Line.h"
#include "SceneRequest.h"
#include "ManageExchange.h"
#include "DataRecordImp.h"
#include "ManageGmSession.h"
#include "ManageCrossServer.h"
#include "ManageBattleReport.h"
#include "ManageContainerImp.h"
#include "ManageCrossPlayerData.h"

struct SCENE_EXOPRT PlayerBriefInfoEx
{
	PlayerBriefInfoEx();

	PlayerBriefInfoEx(uint64 pguid, typed::entity::Short * short_info, uint32 s_id, bool on_line);

	~PlayerBriefInfoEx();

	uint64	player;
	TransferPlayerInfo * transfer_player_info;
	PlayerBriefInfo * player_bi;
};


class SCENE_EXOPRT LineScene : public SceneImp, public Line
{
public:
	LineScene();
	~LineScene();
public:
	virtual int init(const SceneCfg & scene_cfg);

	virtual int stop();

	virtual bool isLineScene();

	// manage scene
	int addScene(uint32 scene_id, Scene * scene);

	Scene * getScene(uint32 scene_id);

	// player route
	virtual void addPlayerShortInfo(uint64 player, typed::entity::Short * short_info, uint32 map_id, bool is_online);

	virtual void updatePlayerShortInfo(uint64 player, typed::entity::Short * short_info);

	virtual bool isCrossServer();

	//virtual void playerLogin(uint64 player, uint32 map_id, int curr_level);

	//virtual void playerLogoff(uint64 player);

	//virtual void playerLevelup(uint64 player, int curr_level);

	//virtual void modifyPlayerRoute(uint64 player, uint32 map_id);

	virtual PlayerBriefInfo * getPlayerBriefInfo(uint64 player);
	virtual uint32 getAllVipShort(Uint64Vec_t &vec64);

	virtual bool playerEnteredWorld(uint64 player);

	virtual PlayerBriefInfo * getPlayerBriefInfoByName(const string & player_name);

	virtual void playerLogin(uint64 player_guid);

	virtual void playerLogoff(uint64 player_guid);

	virtual Uint64Set_t getOnlinePlayers();

	// transfer player between scenes
	virtual void transferPlayer(TransferPlayerInfo * transfer_player_info);

	virtual TransferPlayerInfo * obtainPlayer(uint64 player, uint32 map_id);

	virtual const Uint64Set_t & getTransferPlayerSet();

	// get player fight data, return player scene id
	virtual uint32 getPlayerFightData(uint64 player_guid, TransferPlayerInfo ** trans_player_info);

	// 发送消息到指定场景
	virtual bool sendSceneMsg(uint32 scene_id, PackInfo * pack_info);

	// 发送指定消息到player所在场景
	virtual bool sendScenePlayerMsg(uint64 player, PackInfo * pack_info);

	virtual bool broadCastSceneMsg(PackInfo * pack_info);

	virtual void scenePackInput(PackInfo * pack_info);

	virtual void transmitPlayerMsg(PackInfo * pack_info);

	// collect data of requesting scene player data
	virtual void collectMessage(uint64 player_guid, CollectMsgInfo * collect_msg_info);

	// get and remove 
	virtual CollectMsgInfo * getCollectMessage(uint64 player_guid);

	virtual int sendExchangeMsg(Packet * packet);

	//
	SceneRequest * getSceneRequest();

	DataRecord * getDataRecord();

	// 收集所有在线玩家门派信息
	virtual void collectPlayerSect(uint32 sect_id, uint64 player_guid);

	virtual void removePlayerSect(uint32 sect_id, uint64 player_guid);

	virtual Uint64Set_t getPlayerBySect(uint32 sect_id);

	virtual void setPlayerMapQualification(uint32 map_id, uint64 player_guid);
	// 验证玩家是否可以进入帮会战地图
	virtual bool validatePlayerMapQualification(uint32 map_id, uint64 player_guid);
	// 清除玩家帮会战地图信息
	virtual void clearPlayerMapQualification(uint32 map_id, uint64 player_guid);
	virtual void clearPlayerMapAllQualification(uint32 map_id);

	// request replay
	virtual void sendMasterMsg(Packet * packet);

	// for battle ground
	virtual void collectBattleGroundPlayer(uint64 player_guid, CollectMsgInfo * collect_msg_info);

	virtual CollectMsgInfo * getBattleGroundPlayer(uint64 player_guid);

	virtual bool removeBattleGroundPlayer(uint64 player_guid);

	virtual void removeBattleGroundPlayers();

	// for battle report
	virtual void sendBattleReportMsg(Packet * packet);

	virtual uint64 getBattleReportId();

	virtual string getVipInitSecurityKey();

	virtual string getRobotSecurityKey();

	virtual CollectMsgInfo * collectPlayerFightDataMsgInfo(PluginCfg & plugin_cfg, typed::entity::Player * player);

	// for forbid speak
	virtual void addForbidPlayer(uint64 player_guid);

	virtual void removeForbidPlayer(uint64 player_guid);

	virtual bool isForbidSpeak(uint64 player_guid);

	CrossServer * getCrossServer();

	virtual LunchInfo & getLunchInfo();

	virtual ManageContainer * getManageContainer();

	void broadcastPacketToAllOnlinePlayers(Packet * packet);

	void collectCrossPlayerData(const string & line_id, const string & platform, uint64 player_guid, uint64 entity_guid, const string & entity_stream);

	void collectCrossPlayerDataFinish(const string & line_id, const string & platform, uint64 player_guid);

	virtual void deliveryPlayerDataToAllGS();

	virtual void deliveryPlayerDataToGS(const string & platform, const string & line_id);

	virtual void getCrossPlayerDataInfo(uint64 player_guid, map<uint64, MSG_TYPE *> & msg_map);

protected:

	void extractFightData(PlayerBriefInfoEx * player_brief_info, TransferPlayerInfo * trans_player_info);

	void cleanTransferPlayerInfo(TransferPlayerInfo * transfer_player_info);

	int initLunchTime();
private:
	typedef define_unordered_map<uint64, PlayerBriefInfoEx *> PlayerBriefInfoExMap_t;

	typedef define_unordered_map<string, PlayerBriefInfoEx *> PlayerBriefInfoExNameMap_t;

	typedef define_unordered_map<uint32, Scene *>	SceneMap_t;

	typedef define_unordered_map<uint64, CollectMsgInfo *> CollectMsgInfoMap_t;

	typedef define_unordered_map<uint32, Uint64Set_t>	PlayerSectMap_t;

	SceneMap_t m_scene_map;

	SceneRequestImp m_scene_request;

	PlayerBriefInfoExMap_t m_player_brief_info_map;

	PlayerBriefInfoExNameMap_t m_player_brief_info_map_byname;

	ACE_Thread_Mutex m_player_brief_info_map_mutex;

	CollectMsgInfoMap_t m_collect_msg_info_map;

	ACE_Thread_Mutex m_collect_msg_info_map_mutex;

	Uint64Set_t		m_transfer_player_set;

	Uint64Set_t		m_online_players;

	ACE_Thread_Mutex m_online_players_mutex;

	ManageExchange m_manage_exchange;

	DataRecordImp m_data_record;

	ManageGmSession m_manage_gm_session;

	PlayerSectMap_t m_player_sect_map;

	ACE_Thread_Mutex m_player_sect_map_mutex;

	typedef define_unordered_map<uint32, define_unordered_set<uint64> > PlayerMapQualificationMap_t;

	PlayerMapQualificationMap_t m_player_map_qualification_map;

	ACE_Thread_Mutex m_player_map_qualification_map_mutex;

	CollectMsgInfoMap_t m_battleground_player_map;

	ACE_Thread_Mutex m_battleground_player_map_mutex;

	ManageCrossServer m_manage_cross_server;

	ManageBattleReport m_manage_battle_report;

	Uint64Set_t m_forbid_speak_player;

	ACE_Thread_Mutex m_forbid_speak_player_mutex;

	LunchInfo m_lunch_info;

	ManageContainerImp m_manage_container;

	ManageCrossPlayerData m_manage_cross_player_data;
};
#endif
