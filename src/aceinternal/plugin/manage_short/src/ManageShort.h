
#ifndef MANAGE_SHORT_HPP
#define MANAGE_SHORT_HPP

#include "typedef.h"
#include "Plugin.h"
#include "Scene.h"
#include "opcode.h"
#include "ManageShort_def.h"
#include "entity_ins.h"
#include "protocol_ins.h"
#include "ManagePlayerMoney.h"
#include "protocol/msg_exchange.pb.h"
#include "protocol/msg_cross.pb.h"
#include "entity/container.pb.h"
#include "protocol/msg_cplugin.pb.h"
#include "protocol/msg_query.pb.h"

extern "C" MSHORT_EXOPRT Plugin * make_plugin();

class MSHORT_EXOPRT ManageShort : public Plugin
{
public:
	BEGING_INPUT_MSG_MAP()

		INPUT_HANDLE_MSG(LMSG_PLAYER_FIRST_ONLINE, &ManageShort::lmsg_player_first_online, new typed::protocol::lmsg_player_first_online());

		INPUT_HANDLE_MSG(LMSG_PLAYER_UPGRADE_LEVEL, &ManageShort::lmsg_player_upgrade_level, new typed::protocol::lmsg_player_upgrade_level());

		INPUT_HANDLE_MSG(LMSG_PLAYER_IS_ONLINE, &ManageShort::lmsg_player_is_online, NULL);

		INPUT_HANDLE_MSG(LMSG_PLAYER_IS_OFFLINE, &ManageShort::lmsg_player_is_offline, NULL);

		INPUT_HANDLE_MSG(LMSG_PLAYER_IS_JUMPMAP, &ManageShort::lmsg_player_is_jumpmap, NULL);

		INPUT_HANDLE_MSG(LMSG_PLAYER_JUMPMAP_READY, &ManageShort::lmsg_player_jumpmap_ready, NULL);

		INPUT_HANDLE_MSG(LMSG_REDUCE_GOLD_RETURN, &ManageShort::lmsg_reduce_gold_return, NULL);

		INPUT_HANDLE_MSG(LMSG_UPDATE_REWARD_RETURN, &ManageShort::lmsg_update_reward_return, NULL);

		INPUT_HANDLE_MSG(LMSG_VIP_LEVLEUP, &ManageShort::lmsg_vip_levleup, NULL);

		INPUT_HANDLE_MSG(MMSG_MEMBER_CHARGE, &ManageShort::mmsg_member_charge, new typed::protocol::mmsg_member_charge());

		INPUT_HANDLE_MSG(MMSG_MEMBER_GIVE_GIFT, &ManageShort::mmsg_member_give_gift, new typed::protocol::mmsg_member_give_gift());

		INPUT_HANDLE_MSG(LMSG_PLAYER_CHARGE_CONFIRM, &ManageShort::lmsg_player_charge_confirm, new typed::protocol::lmsg_player_charge_confirm());
		
		INPUT_HANDLE_MSG(LMSG_WEEK_CHANGE_FOR_PLAYER, &ManageShort::lmsg_week_change_for_player, NULL);
		
		INPUT_HANDLE_MSG(LMSG_DAY_CHANGE_FOR_PLAYER, &ManageShort::lmsg_day_change_for_player, NULL);

		INPUT_HANDLE_MSG(LMSG_UPDATE_PLAYER_FIGHT_POWER, &ManageShort::lmsg_update_player_fight_power, NULL);

		INPUT_HANDLE_MSG(STOL_UPDATE_REWARD, &ManageShort::stol_update_reward, NULL);
		
		INPUT_HANDLE_MSG(CMSG_LOOKUP_PLAYER, &ManageShort::cmsg_lookup_player, NULL);

		INPUT_HANDLE_MSG(LMSG_PLAYER_ONLINE_AGAIN, &ManageShort::lmsg_player_online_again, NULL);

		INPUT_HANDLE_MSG(LMSG_PLAYER_ONLINE_DISPATCH_MSG, &ManageShort::lmsg_player_online_dispatch_msg, NULL);

		// exchange
		INPUT_HANDLE_MSG(EMSG_EXCHANGE_REMOTE_QUERY, &ManageShort::emsg_exchange_remote_query, new typed::protocol::emsg_exchange_remote_query());

		INPUT_HANDLE_MSG(EMSG_EXCHANGE_REMOTE_USED, &ManageShort::emsg_exchange_remote_used, new typed::protocol::emsg_exchange_remote_used());

		INPUT_HANDLE_MSG(LMSG_UNLOCK_TOWN, &ManageShort::lmsg_unlock_town, new typed::protocol::lmsg_unlock_town());

		INPUT_HANDLE_MSG(STOL_SYSTEM_UNLOCK_NOTIFY, &ManageShort::stol_system_unlock_notify, new typed::protocol::stol_system_unlock_notify());

		INPUT_HANDLE_MSG(SMSG_CROSS_SERVER_REGISTER, &ManageShort::smsg_cross_server_register, new typed::protocol::smsg_cross_server_register());

		INPUT_HANDLE_MSG(SMSG_CROSS_SERVER_DISCONNECT, &ManageShort::smsg_cross_server_disconnect, new typed::protocol::smsg_cross_server_disconnect());

		INPUT_HANDLE_MSG(STOL_REQUEST_PLAYER_DATA, &ManageShort::stol_request_player_data, NULL);

		INPUT_HANDLE_MSG(ATOL_QUALIFYBATTLE_CHAMPION, &ManageShort::atol_qualifybattle_champion, new typed::protocol::atol_qualifybattle_champion);

		INPUT_HANDLE_MSG(STOL_DELIVERY_PLAYER_DATA, &ManageShort::stol_delivery_player_data, NULL);

		INPUT_HANDLE_MSG(SMSG_DELIVERY_CROSS_PLAYER_DATA_START, &ManageShort::smsg_delivery_cross_player_data_start, new typed::protocol::smsg_cross_deliver_player_data_start());

		INPUT_HANDLE_MSG(SMSG_DELIVERY_CROSS_PLAYER_DATA_END, &ManageShort::smsg_delivery_cross_player_data_end, new typed::protocol::smsg_cross_deliver_player_data_end());

		INPUT_HANDLE_MSG(SMSG_DELIVERY_CROSS_PLAYER_DATA, &ManageShort::smsg_delivery_cross_player_data, new typed::protocol::smsg_cross_deliver_player_data());

		INPUT_HANDLE_MSG(QMSG_RESET_CLIENT_CONFIG, &ManageShort::qmsg_reset_client_config, new typed::protocol::qmsg_reset_client_config());

	END_INPUT_MSG_MAP()

	BEGIN_REQUEST_MSG_MAP()
		REQUEST_HANDLE_MSG(PLMSG_LINE_REDUCE_GOLD, &ManageShort::plmsg_line_reduce_gold);

		REQUEST_HANDLE_MSG(PLMSG_LINE_UPDATE_REWARD, &ManageShort::plmsg_line_update_reward);

		REQUEST_HANDLE_MSG(PLMSG_PICK_UP, &ManageShort::plmsg_pick_up);

		REQUEST_HANDLE_MSG(PLMSG_QUERY_VIP_NUMBER, &ManageShort::plmsg_query_vip_number);

		REQUEST_HANDLE_MSG(PLMSG_REQUEST_ROBOT_BY_TYPE, &ManageShort::plmsg_request_robot_by_type);
	END_REQUEST_MSG_MAP()

	BEGING_NOTIFY_MSG_MAP()
		NOTIFY_HANDLE_MSG(NMSG_SCENE_INITED, &ManageShort::nmsg_scene_inited);

		NOTIFY_HANDLE_MSG(NMSG_FLUSH_ALL_ENTITY, &ManageShort::nmsg_flush_all_entity)

		NOTIFY_HANDLE_MSG(NLMSG_PLAYER_IS_ONLINE, &ManageShort::nlmsg_player_is_online)
		NOTIFY_HANDLE_MSG(NLMSG_PLAYER_ONLINE_DISPATCH_MSG, &ManageShort::nlmsg_player_online_dispatch_msg)
		NOTIFY_HANDLE_MSG(NLMSG_PLAYER_IS_OFFLINE, &ManageShort::nlmsg_player_is_offline)
		NOTIFY_HANDLE_MSG(NLMSG_PLAYER_UPGRADE_LEVEL, &ManageShort::nlmsg_player_upgrade_level)
		NOTIFY_HANDLE_MSG(NLMSG_PLAYER_JUMPMAP_READY, &ManageShort::nlmsg_player_jumpmap_ready)
		NOTIFY_HANDLE_MSG(NMSG_SHORT_INIT_FINISH, &ManageShort::nmsg_short_init_finish)
	END_NOTIFY_MSG_MAP()

public:
	ManageShort();

	~ManageShort();
public:

	int lmsg_player_first_online(const PackInfo & pack_info);

	int lmsg_player_upgrade_level(const PackInfo & pack_info);

	int lmsg_player_is_online(const PackInfo & pack_info);

	int lmsg_player_is_offline(const PackInfo & pack_info);

	int lmsg_player_is_jumpmap(const PackInfo & pack_info);

	int lmsg_player_jumpmap_ready(const PackInfo & pack_info);

	int lmsg_reduce_gold_return(const PackInfo & pack_info);

	int lmsg_update_reward_return(const PackInfo & pack_info);

	int lmsg_vip_levleup(const PackInfo & pack_info);

	int mmsg_member_charge(const PackInfo & pack_info);

	int mmsg_member_give_gift(const PackInfo & pack_info);

	int lmsg_player_charge_confirm(const PackInfo & pack_info);

	int lmsg_week_change_for_player(const PackInfo & pack_info);

	int lmsg_day_change_for_player(const PackInfo & pack_info);

	int lmsg_update_player_fight_power(const PackInfo & pack_info);

	int stol_update_reward(const PackInfo & pack_info);

	int cmsg_lookup_player(const PackInfo & pack_info);

	int lmsg_player_online_again(const PackInfo & pack_info);

	int lmsg_player_online_dispatch_msg(const PackInfo & pack_info);

	int emsg_exchange_remote_query(const PackInfo & pack_info);

	int emsg_exchange_remote_used(const PackInfo & pack_info);

	int lmsg_unlock_town(const PackInfo & pack_info);

	int stol_system_unlock_notify(const PackInfo & pack_info);

	int smsg_cross_server_register(const PackInfo & pack_info);

	int smsg_cross_server_disconnect(const PackInfo & pack_info);

	int stol_request_player_data(const PackInfo & pack_info);

	int atol_qualifybattle_champion(const PackInfo & pack_info);

	int stol_delivery_player_data(const PackInfo & pack_info);

	int smsg_delivery_cross_player_data_start(const PackInfo & pack_info);

	int smsg_delivery_cross_player_data_end(const PackInfo & pack_info);

	int smsg_delivery_cross_player_data(const PackInfo & pack_info);

	int qmsg_reset_client_config(const PackInfo & pack_info);

public:
	int plmsg_line_reduce_gold(const PackInfo & pack_info);

	int plmsg_line_update_reward(const PackInfo & pack_info);

	int plmsg_pick_up(const PackInfo & pack_info);

	int plmsg_query_vip_number(const PackInfo & pack_info);

	int plmsg_request_robot_by_type(const PackInfo & pack_info);
public:
	int nmsg_scene_inited(const PackInfo & pack_info);

	int nmsg_flush_all_entity(const PackInfo & pack_info);

	int nlmsg_player_is_online(const PackInfo & pack_info);
	int nlmsg_player_online_dispatch_msg(const PackInfo & pack_info);
	int nlmsg_player_is_offline(const PackInfo & pack_info);
	int nlmsg_player_upgrade_level(const PackInfo & pack_info);
	int nlmsg_player_jumpmap_ready(const PackInfo & pack_info);
	int nmsg_short_init_finish(const PackInfo & pack_info);


public:
	void loadPlayerShortFinish(RequestList * request_list);

	void loadContainerContainerFinsh(RequestList * request_list);

	void loadContainerFinsh(RequestList * request_list);

	void loadContainerEntityFinsh(RequestList * request_list, uint64 container_guid);

	void loadShortFinish(RequestList * request_list);

	void loadShortOtherGuidFinish(RequestList * request_list, uint64 player_guid);

	void update();

protected:
	virtual int initing();

	void initPlayerShortInfo();

	void initContainer();

	void loadPlayerShortInfo();

	void loadContainerContainer();

	void loadContainerEntity();

	void loadContainerEntity(typed::entity::Container * con_entity);

	void loadContainer();

	void loadShortInfo();

	void loadShortOtherGuid(typed::entity::Short * short_entity);

	bool assignAvailableShortPlayers();

	void collectLoadingShortGuid(uint64 short_guid);

	void removeLoadingShortGuid(uint64 short_guid);

	void flushAllShortEntity();

	void flushAllPlayerShortEntity();

	void flushShortFinish(RequestList * request_list);

	void flushShortPlayersFinish(RequestList * request_list);

	void checkFlushStatus();

	ManagePlayerMoney * getManagePlayerMoneyInfo(uint64 player_guid);

	void addManagePlayerMoney(uint64 player, ManagePlayerMoney * manage_player_money);

	void checkManagePlayerMoneyInfo(uint64 player);

	void increaseLunchDay();

	void notifyLineDayChange();

	void notifyOtherSceneDayChange();

	void checkWeekChange();

	void notifyLineWeekChange();

	void notifyOtherSceneWeekChange();

	void playerCharge(PlayerBriefInfo * player_bi);

	void addNewPlayerShortInfo(uint64 guid, typed::protocol::lmsg_player_first_online * lmsg_pfo);

	void getPlayerLookupDataBack(uint64 request_guid, uint64 lookup_player, uint32 request_id, bool is_success);

	void lookupCrossPlayerInfo(uint64 observer, uint64 lookup_player);

	void sendLookupPlayer(uint64 request_guid, typed::entity::Player * player);

	void sendLookupRole(uint64 request_guid, typed::entity::Player * player);

	void sendLookupEquipment(uint64 request_guid, typed::entity::Player * player);

	void sendLookupRunes(uint64 request_guid, typed::entity::Player * player);

	void sendLookupGuidName(uint64 request_guid, typed::entity::Player * player);

	void sendLookupDodge(uint64 request_guid, typed::entity::Player * player);

	void checkShortPlayer();

	bool isCollectedShort(uint64 player_guid);

	void shortLoadFinishNotify();

	bool reduceOfflinePlayerCoin(uint64 player_guid, typed::protocol::plmsg_line_reduce_gold * reduce_gold, uint32 & return_code);

	void collectVipNumber(uint32 from_vip_level, uint32 to_vip_level);

	void checkUpdatedContainer();

	void loadCoinFromDb(uint64 player_guid, uint32 map_id);

	void loadPlayerCoinFinish(RequestList * request_list, uint32 map_id);

	void insert_robot(uint64 player_guid, uint32 robot_type);

	void insert_openid_to_guid(const string& open_id, uint64 player_guid);

	uint64 get_inviter_guid_by_open_id(const string& open_id);

private:
	uint64 m_short_owner_guid;

	typed::entity::ShortPlayers * m_available_short_players;

	int			m_flush_short_total;

	int			m_curr_flush_short_num;

	int			m_curr_flush_shortplayer_num;

	// 加载的guid集合，包括short、friend、mail等
	Uint64Set_t m_loading_short_guid;

	bool		m_is_load_failed;

	int			m_update_timer_id;

	uint64		m_current_day;

	uint64		m_current_week;

	typedef define_unordered_map<uint64, ManagePlayerMoney *> ManagePlayerMoneyMap_t;

	ManagePlayerMoneyMap_t m_manage_player_money_map;

	Uint64Set_t m_all_short_player_set;

	bool m_is_shutdown;

	SystemResInfo * m_system_res_info;

	Uint64Set_t m_active_amah_player_guid;

	Uint64Set_t m_loading_container_set;
	string m_client_config;

	define_unordered_map<uint32, define_unordered_set<uint64> > m_all_robot;
	define_unordered_map<string, uint64> m_open_id_to_guid;
};

#endif