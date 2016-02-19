
#ifndef MANAGE_PLAYER_HPP
#define MANAGE_PLAYER_HPP

#include "typedef.h"
#include "Plugin.h"
#include "Scene.h"
#include "opcode.h"
#include "ManagePlayer_def.h"
#include "PlayerAssistant.h"
#include "entity_ins.h"
#include "protocol_ins.h"
#include "InheritInfoRes.h"
#include "protocol/msg_inherit.pb.h"
#include "DoubleexpRes.h"
#include "SimpleXML.h"

extern "C" MPLAYER_EXOPRT Plugin * make_plugin();

struct PlayerInfo 
{
	PlayerInfo()
	: enter_world_no(0)
	, leave_world_no(0)
	, load_player_request_no(0)
	, load_player_success_no(0)
	, load_player_failed_no(0)
	, flush_player_request_no(0)
	, flush_player_success_no(0)
	, flush_player_failed_no(0)
	{}

	uint32 enter_world_no;
	uint32 leave_world_no;
	uint32 load_player_request_no;
	uint32 load_player_success_no;
	uint32 load_player_failed_no;
	uint32 flush_player_request_no;
	uint32 flush_player_success_no;
	uint32 flush_player_failed_no;
};

enum PlayerOnlineState
{
	POS_BASE = 0,
	POS_LOADING_ENTER_MAP,
	POS_ONLINE,
	POS_FLUSHING_LEAVE_MAP,
	POS_FLUSHING_LEAVE_MAP_WITH_JUMP,
	POS_OFFLINE,
};

struct PlayerAttachInfo
{
	PlayerAttachInfo()
		: player_state(POS_BASE)
		, transfer_info(NULL)
	{}

	PlayerAttachInfo(PlayerOnlineState player_s)
		:player_state(player_s)
		, transfer_info(NULL)
	{}

	PlayerOnlineState player_state;
	TransferPlayerInfo * transfer_info;
};

struct FlushEntityFinish 
{
	FlushEntityFinish()
		: flush_player_finish(false)
		, flush_short_finish(false)
	{}

	bool is_finish(bool is_line_scene)
	{
		return flush_player_finish;
		//if (is_line_scene)
		//{
		//	return flush_player_finish && flush_short_finish;
		//}
		//else
		//{
		//	return flush_player_finish;
		//}
	}

	bool flush_player_finish;
	bool flush_short_finish;
};

class MPLAYER_EXOPRT ManagePlayer : public Plugin
{
public:
	BEGING_INPUT_MSG_MAP()
		INPUT_HANDLE_MSG(CMSG_PLAYER_ENTER_MAP, &ManagePlayer::cmsg_enter_world, new typed::protocol::cmsg_player_enter_map())
		INPUT_HANDLE_MSG(CMSG_PLAYER_LEAVE_MAP, &ManagePlayer::cmsg_leave_world, NULL)
		INPUT_HANDLE_MSG(CMSG_PLAYER_JUMP_MAP, &ManagePlayer::cmsg_jump_map, new typed::protocol::cmsg_player_jump_map())
		INPUT_HANDLE_MSG(GMSG_PLAYER_JUMP_MAP, &ManagePlayer::gmsg_player_jump_map, new typed::protocol::smsg_player_jump_map())
		INPUT_HANDLE_MSG(CMSG_JUMP_MAP_READY, &ManagePlayer::cmsg_jump_map_ready, NULL)
		INPUT_HANDLE_MSG(CMSG_REQUEST_ROLE_ATTR, &ManagePlayer::cmsg_request_role_attr, new typed::protocol::cmsg_request_role_attr())
		INPUT_HANDLE_MSG(CMSG_REQUEST_ROLE_ATTR_RANK, &ManagePlayer::cmsg_request_role_attr_rank, new typed::protocol::cmsg_request_role_attr_rank())

		INPUT_HANDLE_MSG(CMSG_LOOPBACK, &ManagePlayer::cmsg_loop_back, new typed::protocol::cmsg_loopback)

		INPUT_HANDLE_MSG(SMSG_SCENE_INITED, &ManagePlayer::smsg_scene_inited, NULL)

		INPUT_HANDLE_MSG(SMSG_LOST_GATE_CONNECTION, &ManagePlayer::smsg_lost_gate_connection, NULL)

		// for line scene
		// process line scene message in scene
		INPUT_HANDLE_MSG(LMSG_REQUEST_PLAYER_FIGHT_DATA, &ManagePlayer::lmsg_request_player_fight_data, NULL)

		// process line scene message in line
		INPUT_HANDLE_MSG(LMSG_GET_DATA_FROM_OTHER_SCENE_BACK, &ManagePlayer::lmsg_get_data_from_other_scene_back, NULL)

		INPUT_HANDLE_MSG(LMSG_GET_DATA_FROM_OTHER_SCENE_BACK_FAILED, &ManagePlayer::lmsg_get_data_from_other_scene_back_failed, NULL)

		INPUT_HANDLE_MSG(LMSG_DAY_CHANGE, &ManagePlayer::lmsg_day_change, NULL)

		INPUT_HANDLE_MSG(LMSG_WEEK_CHANGE, &ManagePlayer::lmsg_week_change, NULL)

		INPUT_HANDLE_MSG(LMSG_PLAYER_CHARGE, &ManagePlayer::lmsg_player_charge, NULL)

		INPUT_HANDLE_MSG(CMSG_GET_SERVER_TIME, &ManagePlayer::cmsg_get_server_time, NULL)

		INPUT_HANDLE_MSG(CMSG_SPECIAL_ACTION, &ManagePlayer::cmsg_special_action, new typed::protocol::cmsg_special_action())

		INPUT_HANDLE_MSG(CMSG_SPECIAL_ACTION_PERSON, &ManagePlayer::cmsg_special_action_person, new typed::protocol::cmsg_special_action_person())

		INPUT_HANDLE_MSG(LTOS_SHORT_INIT_FINISH, &ManagePlayer::ltos_short_init_finish, NULL)

		INPUT_HANDLE_MSG(CMSG_SELECT_INHERIT_ROLE, &ManagePlayer::cmsg_select_inherit_role, new typed::protocol::cmsg_select_inherit_role())

		INPUT_HANDLE_MSG(CMSG_INHERIT_ROLE, &ManagePlayer::cmsg_inherit_role, new typed::protocol::cmsg_inherit_role())

		INPUT_HANDLE_MSG(LTOS_DELIVERY_PLAYER_DATA, &ManagePlayer::ltos_delivery_player_data, NULL)

		INPUT_HANDLE_MSG(CMSG_SET_FOLLOW_ROLE, &ManagePlayer::cmsg_set_follow_role, new typed::protocol::cmsg_set_follow_role())

		INPUT_HANDLE_MSG(LTOS_INNER_REQUEST_LOGOFF, &ManagePlayer::ltos_inner_request_logoff, NULL)

		INPUT_HANDLE_MSG(GTOS_QUERY_QQ_VIP_INFO, &ManagePlayer::gtos_query_qq_vip_info, new typed::protocol::gtos_query_qq_vip_info)

	END_INPUT_MSG_MAP()

	BEGING_NOTIFY_MSG_MAP()
		NOTIFY_HANDLE_MSG(NMSG_PLAYER_IS_OFFLINE, &ManagePlayer::nmsg_player_is_offline)
		NOTIFY_HANDLE_MSG(NMSG_PLAYER_IS_ONLINE, &ManagePlayer::nmsg_player_is_online)
		NOTIFY_HANDLE_MSG(NMSG_SHORT_FLUSH_FINISH, &ManagePlayer::nmsg_short_flush_finish)
		NOTIFY_HANDLE_MSG(NMSG_PLAYER_ENTER_MAP, &ManagePlayer::nmsg_player_enter_map)
		NOTIFY_HANDLE_MSG(NMSG_PLAYER_ONLINE_DISPATCH_MSG, &ManagePlayer::nmsg_player_online_dispatch_msg)
		NOTIFY_HANDLE_MSG(NMSG_PLAYER_LEVEL_UP, &ManagePlayer::nmsg_player_level_up)
		NOTIFY_HANDLE_MSG(NMSG_USE_ITEM_NOTIFY, &ManagePlayer::nmsg_use_item_notify)
		
	END_NOTIFY_MSG_MAP()

	BEGIN_REQUEST_MSG_MAP()
		REQUEST_HANDLE_MSG(RMSG_ROLE_ADD, &ManagePlayer::rmsg_role_add)
		REQUEST_HANDLE_MSG(RMSG_ROLE_REJOIN, &ManagePlayer::rmsg_role_rejoin)
		REQUEST_HANDLE_MSG(PMSG_UPDATE_REWARD, &ManagePlayer::rmsg_update_reward)
		REQUEST_HANDLE_MSG(PMSG_REDUCE_GOLD, &ManagePlayer::pmsg_reduce_gold)
		REQUEST_HANDLE_MSG(PMSG_PLAYER_JUMP_MAP, &ManagePlayer::pmsg_player_jump_map)
		REQUEST_HANDLE_MSG(PMSG_REDUCE_ENERGY, &ManagePlayer::pmsg_reduce_energy)
		REQUEST_HANDLE_MSG(PMSG_QUERY_ENERGY, &ManagePlayer::pmsg_query_energy)
		REQUEST_HANDLE_MSG(PMSG_ADD_ENERGY, &ManagePlayer::pmsg_add_energy)
		REQUEST_HANDLE_MSG(PMSG_UPDATE_PRESTIGE, &ManagePlayer::pmsg_update_prestige)
		REQUEST_HANDLE_MSG(PMSG_UPDATE_REVERENCE, &ManagePlayer::pmsg_update_reverence)
		REQUEST_HANDLE_MSG(PMSG_REDUCE_REVERENCE, &ManagePlayer::pmsg_reduce_reverence)
		REQUEST_HANDLE_MSG(PMSG_CHANGE_CHAR_ABILITY, &ManagePlayer::pmsg_change_char_ability)
		REQUEST_HANDLE_MSG(PMSG_QUERY_SYSTEM_UNLOCK, &ManagePlayer::pmsg_query_system_unlock)
		REQUEST_HANDLE_MSG(PMSG_UPDATE_VIPEXP, &ManagePlayer::pmsg_update_vipexp)
		
	END_REQUEST_MSG_MAP()

	BEGING_GMCMD_MSG_MAP()
		GMCMD_HANDLE_MSG("levelup", &ManagePlayer::gmcmd_levelup)
		GMCMD_HANDLE_MSG("addexp", &ManagePlayer::gmcmd_addexp)
		GMCMD_HANDLE_MSG("addmoney", &ManagePlayer::gmcmd_addmoney)
		GMCMD_HANDLE_MSG("addcoin", &ManagePlayer::gmcmd_addcoin)
		GMCMD_HANDLE_MSG("addseni", &ManagePlayer::gmcmd_addseni)
		GMCMD_HANDLE_MSG("jumpmap", &ManagePlayer::gmcmd_jumpmap)
		GMCMD_HANDLE_MSG("modifyattr", &ManagePlayer::gmcmd_modifyattr)
		GMCMD_HANDLE_MSG("addenergy", &ManagePlayer::gmcmd_addenergy)
		GMCMD_HANDLE_MSG("addpres", &ManagePlayer::gmcmd_addpres)

		GMCMD_HANDLE_MSG("addvipexp", &ManagePlayer::gmcmd_addvipexp)
		GMCMD_HANDLE_MSG("addrever", &ManagePlayer::gmcmd_addrever)
		GMCMD_HANDLE_MSG("setyellowlevel", &ManagePlayer::gmcmd_setyellowlevel)
	END_GMCMD_MSG_MAP()

public:
	ManagePlayer();

	~ManagePlayer();
public:
	virtual int init(const PluginCfg & plugin_cfg);

	//virtual int input(const PackInfo & pack_info);

	//virtual int notify(const PackInfo & pack_info);

	//virtual int request(const PackInfo & pack_info);
public:
	int cmsg_enter_world(const PackInfo & pack_info);

	int cmsg_leave_world(const PackInfo & pack_info);

	//int cmsg_enter_map(const PackInfo & pack_info);

	int cmsg_jump_map(const PackInfo & pack_info);

	int gmsg_player_jump_map(const PackInfo & pack_info);

	int cmsg_jump_map_ready(const PackInfo & pack_info);

	int cmsg_request_role_attr(const PackInfo & pack_info);

	int cmsg_request_role_attr_rank(const PackInfo & pack_info);

	int cmsg_loop_back(const PackInfo & pack_info);

	int smsg_scene_inited(const PackInfo & pack_info);

	int smsg_lost_gate_connection(const PackInfo & pack_info);

	// for line scene
	int lmsg_request_player_fight_data(const PackInfo & pack_info);

	//
	int lmsg_get_data_from_other_scene_back(const PackInfo & pack_info);

	int lmsg_get_data_from_other_scene_back_failed(const PackInfo & pack_info);

	int lmsg_day_change(const PackInfo & pack_info);

	int lmsg_week_change(const PackInfo & pack_info);

	int lmsg_player_charge(const PackInfo & pack_info);

	int cmsg_get_server_time(const PackInfo & pack_info);

	int cmsg_special_action(const PackInfo & pack_info);

	int cmsg_special_action_person(const PackInfo & pack_info);

	int ltos_short_init_finish(const PackInfo & pack_info);

	int cmsg_select_inherit_role(const PackInfo & pack_info);

	int cmsg_inherit_role(const PackInfo & pack_info);

	int ltos_delivery_player_data(const PackInfo & pack_info);

	int cmsg_set_follow_role(const PackInfo & pack_info);

	int ltos_inner_request_logoff(const PackInfo & pack_info);

	int gtos_query_qq_vip_info(const PackInfo & pack_info);

public:
	int nmsg_player_is_online(const PackInfo & pack_info);

	int nmsg_player_is_offline(const PackInfo & pack_info);

	int nmsg_short_flush_finish(const PackInfo & pack_info);

	int nmsg_player_enter_map(const PackInfo & pack_info);

	int nmsg_player_online_dispatch_msg(const PackInfo & pack_info);

	int nmsg_player_level_up(const PackInfo & pack_info);

	int nmsg_use_item_notify(const PackInfo & pack_info);

public:
	int rmsg_role_add(const PackInfo & pack_info);

	int rmsg_role_rejoin(const PackInfo & pack_info);

	int rmsg_update_reward(const PackInfo & pack_info);

	int pmsg_reduce_gold(const PackInfo & pack_info);

	int pmsg_player_jump_map(const PackInfo & pack_info);

	int pmsg_reduce_energy(const PackInfo & pack_info);

	int pmsg_query_energy(const PackInfo & pack_info);

	int pmsg_add_energy(const PackInfo & pack_info);

	int pmsg_update_prestige(const PackInfo & pack_info);

	int pmsg_update_reverence(const PackInfo & pack_info);

	int pmsg_reduce_reverence(const PackInfo & pack_info);

	int pmsg_change_char_ability(const PackInfo & pack_info);

	int pmsg_query_system_unlock(const PackInfo & pack_info);

	int pmsg_update_vipexp(const PackInfo & pack_info);
public:
	int gmcmd_levelup(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_addexp(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_addmoney(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_addcoin(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_addseni(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_jumpmap(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_modifyattr(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_addenergy(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_addpres(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_addvipexp(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_addrever(const vector<string> & gm_param, uint64 target_guid);

	int gmcmd_setyellowlevel(const vector<string> & gm_param, uint64 target_guid);

public:
	void initPlayer(uint64 player_guid);

	void initRole(typed::entity::Role * role, RoleResInfo * role_res_info);

	void playerDataLoadFinish(uint64 player_guid, bool is_success);

	void playerFlushDataFinish(uint64 player_guid, bool is_success, RequestList * request_list);

	//void playerUpdateDataFinish(uint64 player_guid, bool is_success);
public:
	void update();

	void updatePool();
protected:
	virtual int initing();

	void getMapIdVec(vector<std::pair<int, int> > & map_vec);

	bool reach_map_condition(int map_id, typed::entity::Player * player, MapResInfo * * map_res_info);

	void send_create_self_msg(typed::entity::Player * player);

	void send_create_role_msg(typed::entity::Player * player);

	void send_create_role_msg(typed::entity::Player * player, typed::entity::Role * role);

	void fillRoleAttr(::google::protobuf::RepeatedField< ::google::protobuf::uint32 > * attributes, typed::entity::Role * role, typed::entity::Player * player);

	//void fillRoleAttrRank(::google::protobuf::RepeatedField< ::google::protobuf::uint32 > * attributes, typed::entity::Role * role, typed::entity::Player * player);

	//void send_create_form_msg(typed::entity::Player * player);

	void send_create_item_msg(typed::entity::Player * player);

	void send_create_item_msg(typed::entity::Player * player, const ::google::protobuf::RepeatedField< ::google::protobuf::uint64 > & bag);

	void dispatchPlayerData(typed::entity::Player * player);

	void send_open_time(uint64 player_guid);

protected:
	void notifyPlayerIsFirstOnline(uint64 player);

	void notifyPlayerIsFirstOnlineToLine(typed::entity::Player * player);

	void notifyPlayerIsOnline(typed::entity::Player * player);

	void notifyPlayerIsOffline(uint64 player);

	void notifyPlayerIsOnlineAfterSync(typed::entity::Player * player);

	void notifyPlayerJumpMap(uint64 player, uint32 previous_map_id, uint32 target_map_id);

	void initPlayerAttribute(typed::entity::Player * player);

	void firstInitPlayer(typed::entity::Player * player);

	void firstInitPlayerLevel(typed::entity::Player * player);

	void notifyPlayerUpdateMoney(typed::entity::Player * player);

	void notifyPlayerUpdateEnergy(typed::entity::Player * player);

	void notifyDayChange();

	void notifyWeekChange();

	RoleResInfo * getRoleResInfo(int role_id);
private:

private:
	void updatePlayerExp(uint64 player_guid, int increase_value);

	void updateRoleExp(uint64 role_guid, int increase_value, int addiction_status, int index, uint32 source_from);

	void updateSeniority(typed::entity::Player * player, int increase_value, uint32 source_from);

	void updateMoney(typed::entity::Player * player, typed::entity::Coin * coin, int increase_value, uint32 source_from);

	void updateCoin(typed::entity::Player * player, typed::entity::Coin * coin, int increase_value, uint32 source_from, bool is_charge);

	void updatePrestige(typed::entity::Player * player, int increase_value, uint32 source_from);

	void calcUpdateLevel(typed::entity::Player * player, uint32 & exp, int & curr_level);

	void levelUpgrade(typed::entity::Role * role, int from_level, int update_level);

	void levelReduce(typed::entity::Role * role, int from_level, int update_level);

	void notifyRoleLevelUp(typed::entity::Player * player, typed::entity::Role * role, int pre_level);

	void notifyRoleUpdateExp(typed::entity::Player * player, typed::entity::Role * role);

	int playerJumpMap(typed::entity::Player * player, uint32 target_map_id);

	TransferPlayerInfo * collectTransferPlayerInfo(typed::entity::Player * player, uint32 target_map_id);

	void transferPlayerToLineScene(TransferPlayerInfo * transfer_info);

	void playerFlushDataFinishAfterLostGateConnection(uint64 player_guid, bool is_success, RequestList * request_list);

	void checkFlushStatus();

	void processNeedFlushPlayer();

	void checkPlayerCleanDay(typed::entity::Player * player);

	void checkPlayerCleanWeek(typed::entity::Player * player);

	void updateVipExp(typed::entity::Player * player, int increase_exp, int source_from);

	void notifyPlayerVipLevelUp(typed::entity::Player * player, uint32 from_vip_level);

	void notifyPlayerVipLevelUpToLineScene(typed::entity::Player * player, uint32 from_vip_level);

	void attachRoleCharAbility(typed::entity::Role * role, uint32 char_ability);

	bool is_valid_charge_request(typed::entity::Vip * vip, uint32 request_index, uint32 charge_value);

	void recodeChargeInfo(typed::entity::Vip * vip, uint32 request_index, uint32 charge_value);

	void playerDayChange(typed::entity::Player * player, int online_status);

	void notifyClientDayChange(typed::entity::Player * player);

	void playerWeekChange(typed::entity::Player * player);

	void playerOnlineAgain(typed::entity::Player * player);

	void calcOfflineExp(typed::entity::Player * player);

	bool isGuildBattleMap(MapResInfo * map_res_info);

	bool canEnterGuildBattleMap(typed::entity::Player * player, MapResInfo * map_res_info);

	//int getAttrRank(typed::entity::Role * role, int attr_index, int attr_rank_index, int player_level);

	void checkEnergyReward();

	void provideEnergy(uint64 player_guid, uint32 energy_value, uint64 provide_time);

	void notifyClientProvideEnergy(uint64 player_guid, uint32 energy_value);

	void checkPlayerProvideEnergy(typed::entity::Player * player);

	void initProvideEnergyInfo();

	void notifySystemUnlock(uint64 player_guid, uint32 system_id);

	bool reduceInheritMoney(typed::entity::Player * player, uint32 from_role_level, uint32 inherit_type, uint32 & cost_money, uint32 & cost_coin);

	GoldParamResInfo * getGoldParamResInfo(uint32 gold_id);

	void notifyRoleAttrUpdate(typed::entity::Role * role);

	void checkDoubleexp(typed::entity::Player * player);

	void updateDoubleexpInfo(typed::entity::Player * player);

	DoubleexpRes * getDoubleexpRes(uint32 id);

	bool useMutilexp(uint32 source_from);

	void fillInheritSubToken(::google::protobuf::RepeatedPtrField< ::typed::protocol::smsg_select_inherit_role_SubToken > * dst_st, ::google::protobuf::RepeatedPtrField< ::typed::protocol::nmsg_inherit_select_SubToken > * src_st);

	bool loadPreChargeInfo();

	void chargePreChargeInfo(typed::entity::Player * player);

	void sendHireZsly(typed::entity::Player * player, int hired);

	void deliveryCoinToLS(typed::entity::Player * player);

	string md5_encode(const std::string& pw);

	int sendMail(uint64 player_guid, const string& sender, const string& title, const string& tontent, uint32 item_id, uint32 source_from);

	int req_chat_notify(const vector<uint64> vec_guid, const vector<string>& vec_name, uint32 talk_id);

	Mail_formatInfo* getMailFormatInfo(uint32 id);

	int init_robot_info(typed::entity::Player* player);

	//bool resetInheritCoinInfo();
private:
	typedef map<uint64, PlayerAttachInfo>	PlayerOnlineStateMap_t;

//	typedef map<uint64, typed::protocol::cmsg_player_jump_map> PlayerJumpMapInfoMap_t;

	long m_update_timer_id;

	long m_update_pool_timer_id;

	MapResInfoMap_t * m_map_info_map;

	RoleResInfoMap_t * m_role_res_info_map;

	WarInitResInfo * m_war_init_res_info;

	LevelExpResInfoMap_t * m_level_exp_res_info;

	SystemResInfo * m_system_res_info;

	InitializationCfg * m_intialization_res_info;

	PlayerInfo m_player_info;

	PlayerAssistant	m_player_assistant;

	PlayerOnlineStateMap_t	m_player_state;

	bool					m_lost_gate_connection;

	PlayerGuidSet_t			m_need_flush_player_after_lost_gate_connection;

	Uint64Set_t				m_flush_back_player_after_lost_gate_connection;

	FlushEntityFinish		m_flush_entity_finish;

	typedef	define_unordered_map<uint64, bool>	NeedFlushPlayerMap_t;

	// player guid, is update>
	NeedFlushPlayerMap_t	m_need_flush_player_map;

	bool					m_send_notify_gate_lost_connection;

	uint32					m_update_call_number;

	uint64					m_current_day;

	uint64					m_current_week;

	Uint64Set_t				m_player_online_before_finish_flushing;

	VipexpResInfoMap_t *	m_vipexp_res_info_map;

	typedef define_unordered_map<uint32, uint32> LevelUnlockFun_t;

	LevelUnlockFun_t m_level_unlock_function;

	OfflinehangupInfo * m_offline_hangup_res;

	uint32 m_offline_hangup_exp_unlock_level;

	uint64 m_last_provide_energy_time;

	uint64 m_last_provide_energy_value;

	SystemUnlockRes * m_system_unlock_res;

	InheritInfoRes * m_inherite_info_res;

	GoldParamResInfoMap_t * m_gold_param_info_res_map;

	VipInfo * m_vip_role_follow;

	GoldParamResInfo * m_inherite_gold_res;

	DoubleexpResMap_t * m_doubleexp_res_map;

	Document * m_pre_charge_doc;

	define_unordered_map<string, Element *> m_pre_charge_account_ele_map;
};
#endif