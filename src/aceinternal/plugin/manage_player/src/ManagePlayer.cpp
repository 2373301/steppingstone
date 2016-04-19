
#include <ace/Date_Time.h>
#include <ace/Time_Value.h>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include "ManagePlayer.h"
#include "coredef.h"
#include "Pool.h"
#include "entity_ins.h"
#include "protocol_ins.h"
#include "Packet.h"
#include "Logger.h"
#include "PlayerFunction.h"
#include "protocol/msg_notify.pb.h"
#include "SystemUnlockRes.h"
#include "OfflinehangupRes.h"
#include "VipRes.h"
#include "entity/doubleexp.pb.h"
#include "md5.h"
#include "WebVipRes.h"
#include "MailFormatRes.h"

#define UPDATE_CHECK_TIME	1000

#define UPDATE_POOL_TIME	200
//#define UPDATE_POOL_TIME	10000

#define ADDICTION_STATUS_TIRED		1
#define ADDICTION_STATUS_WEAKNESS	2

#define SOURCE_FROM_LINE_SCENE	190000

#define SOURCE_NOT_RECORD		100000

#define ZSLW_ROLE_ID			1057

#define IS_SOURCE_FROM_LS(src_from) (SOURCE_FROM_LINE_SCENE == src_from)

#define IS_SOURCE_NOT_RECORD(src_from)	(SOURCE_NOT_RECORD == src_from)

#define IS_ADDICTION_STATUS_TIRED(addi_status)		(ADDICTION_STATUS_TIRED == addi_status)

#define IS_ADDICTION_STATUS_WEAKNESS(addi_status)	(ADDICTION_STATUS_WEAKNESS == addi_status)

#define GET_ADDICTION_VALUE(value, addi_status)	\
	if (IS_ADDICTION_STATUS_TIRED(addi_status))	\
	{	\
		value = value * 0.5;	\
	}	\
	else if (IS_ADDICTION_STATUS_WEAKNESS(addi_status))	\
	{	\
		value = 0;	\
	}

#define RECORD_VIP_FROM_PLATFORM 6000

#define TALK_PLATFORM_VIP_ADD 901
#define TALK_PLATFORM_YEAR_CARD 902

Plugin * make_plugin()
{
	return new ManagePlayer();
}

string pre_charge_file_name("./pre_charge.xml");

uint64 getCurrTimeInMinute()
{
	ACE_Time_Value time_value = ACE_OS::gettimeofday();

	uint64 total_minutes = 0;
	time_value.to_usec(total_minutes);

	total_minutes /= 1000000;
	total_minutes /= 60;

	return total_minutes;
}

uint64 getCurrentDay()
{
	ACE_Time_Value time_value = ACE_OS::gettimeofday();
	ACE_Date_Time curr_dt(time_value);

	uint64 curr_mic = 0;
	time_value.to_usec(curr_mic);

	curr_mic /= 1000000;

	return curr_mic - curr_dt.hour() * 3600 - curr_dt.minute() * 60 - curr_dt.second();
}

uint64 getCurrentWeek()
{
	ACE_Time_Value time_value = ACE_OS::gettimeofday();
	ACE_Date_Time curr_dt(time_value);
	int week_day = curr_dt.weekday();
	if (0 == week_day)
	{
		week_day = 7;
	}

	uint64 curr_mic = 0;
	time_value.to_usec(curr_mic);

	curr_mic /= 1000000;

	curr_mic -= (week_day - 1) * 24 * 3600 + curr_dt.hour() * 3600 + curr_dt.minute() * 60 + curr_dt.second();

	return curr_mic;
}

ManagePlayer::ManagePlayer()
: m_update_timer_id(0)
, m_update_pool_timer_id(0)
, m_map_info_map(NULL)
, m_role_res_info_map(NULL)
, m_war_init_res_info(NULL)
, m_level_exp_res_info(NULL)
, m_system_res_info(NULL)
, m_intialization_res_info(NULL)
, m_player_assistant(this)
, m_lost_gate_connection(false)
, m_send_notify_gate_lost_connection(false)
, m_update_call_number(0)
, m_current_day(0)
, m_current_week(0)
, m_offline_hangup_res(NULL)
, m_offline_hangup_exp_unlock_level(0)
, m_last_provide_energy_time(0)
, m_last_provide_energy_value(0)
, m_system_unlock_res(NULL)
//, m_inherite_info_res(NULL)
, m_gold_param_info_res_map(NULL)
, m_vip_role_follow(NULL)
, m_inherite_gold_res(NULL)
, m_pre_charge_doc(NULL)
{
	m_plugin_type = PT_MANAGE_PLAYER;
}

ManagePlayer::~ManagePlayer()
{}

int ManagePlayer::init(const PluginCfg & plugin_cfg)
{
	if (NULL == plugin_cfg.scene)
	{
		return -1;
	}

	m_plugin_cfg = plugin_cfg;

	//PLAYER_LOG_ERROR("error, txt is <%s>, num is <%d>", "player is no find", 9989);
	//PLAYER_LOG_ERROR("second error");

	return initing();
}

//int ManagePlayer::input(const PackInfo & pack_info)
//{
//	return 0;
//}
//
//int ManagePlayer::notify(const PackInfo & pack_info)
//{
//	return 0;
//}
//
//int ManagePlayer::request(const PackInfo & pack_info)
//{
//	return 0;
//}

int ManagePlayer::cmsg_enter_world(const PackInfo & pack_info)
{
	uint64 curr_time = getCurrentTimeInSec();
	m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 101, 101, pack_info.guid, curr_time, m_plugin_cfg.template_id);

	++m_player_info.enter_world_no;

	Uint64Set_t::iterator it_sub = m_player_online_before_finish_flushing.find(pack_info.guid);
	if (it_sub != m_player_online_before_finish_flushing.end())
	{
		PLAYER_LOG_INFO("player online but player online while flushing data, player <%llu>", pack_info.guid);
		return 0;
	}

	PlayerOnlineStateMap_t::iterator it = m_player_state.find(pack_info.guid);
	if (it != m_player_state.end())
	{
		if (POS_LOADING_ENTER_MAP == it->second.player_state)
		{
			PLAYER_LOG_INFO("player is entering world but the player is already in loading status, player is <%llu>", pack_info.guid);
			return 0;
		}
		else if (POS_ONLINE == it->second.player_state)
		{
			// online again, dispatch player data
			GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);
			//notifyPlayerIsOnline(player);
			playerOnlineAgain(player);
			MANAGE_GRID_READD(m_plugin_cfg.manage_grid, player);
			PLAYER_LOG_INFO("player is online again while player is already online, player is <%llu>", pack_info.guid);
			return 0;
		}
		else if (POS_FLUSHING_LEAVE_MAP == it->second.player_state)
		{
			m_player_online_before_finish_flushing.insert(pack_info.guid);
			PLAYER_LOG_INFO("player is entering world while data flashing, player is <%llu>", pack_info.guid);
			return 0;
			//
			//m_plugin_cfg.pool->playerIsOnline(pack_info.guid);

			//m_player_assistant.clearEntityFlushState(pack_info.guid);
			//GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
			//PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);
			////notifyPlayerIsOnline(player);
			//dispatchPlayerData(player);
			//MANAGE_GRID_ADD(m_plugin_cfg.manage_grid, player);
			//it->second.player_state = POS_ONLINE;
			//return 0;
		}
		else if (POS_FLUSHING_LEAVE_MAP_WITH_JUMP == it->second.player_state)
		{
			// not allow to onine
			PLAYER_LOG_WARNING("player is online while jump map, not allowed");
			return 0;
		}
	}

	PLAYER_LOG_DEBUG("player is enter world, start to load player info, guid is <%llu>", pack_info.guid);

	// load player data
	++m_player_info.load_player_request_no;

	m_player_assistant.request_load_player(pack_info.guid);

	m_player_state[pack_info.guid] = PlayerAttachInfo(POS_LOADING_ENTER_MAP);

	return 0;
}

int ManagePlayer::cmsg_leave_world(const PackInfo & pack_info)
{
	++m_player_info.leave_world_no;

	PLAYER_LOG_INFO("player is leave world, start to flush player info, guid is <%llu>", pack_info.guid);

	Uint64Set_t::iterator it_sub = m_player_online_before_finish_flushing.find(pack_info.guid);
	if (it_sub != m_player_online_before_finish_flushing.end())
	{
		m_player_online_before_finish_flushing.erase(it_sub);
		return 0;
	}

	PlayerOnlineStateMap_t::iterator it = m_player_state.find(pack_info.guid);
	if (it != m_player_state.end())
	{
		if (POS_LOADING_ENTER_MAP == it->second.player_state)
		{
			it->second.player_state = POS_OFFLINE;
			return 0;
		}
		else if (POS_FLUSHING_LEAVE_MAP_WITH_JUMP == it->second.player_state)
		{
			it->second.player_state = POS_FLUSHING_LEAVE_MAP;
			if (it->second.transfer_info != NULL)
			{
				delete it->second.transfer_info;
				it->second.transfer_info = NULL;
			}
			return 0;
		}
		else if (POS_FLUSHING_LEAVE_MAP == it->second.player_state)
		{
			return 0;
		}
		else if (POS_OFFLINE == it->second.player_state)
		{
			return 0;
		}
		else
		{
			// change the player state
			it->second.player_state = POS_FLUSHING_LEAVE_MAP;
		}
	}
	else
	{
		// error
		PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
		if (NULL != player_bi)
		{
			if (player_bi->scene_id != m_plugin_cfg.template_id)
			{
				PLAYER_LOG_INFO("player is leave world, but the other map get the msg, transfer <%d> leave map msg to target <%d>, player <%llu>", m_plugin_cfg.template_id, player_bi->scene_id, pack_info.guid);
				// transfer leave map msg to the target map
				PackInfo * pi = new PackInfo(pack_info.opcode, pack_info.guid, NULL);
				m_plugin_cfg.line_scene->sendSceneMsg(player_bi->scene_id, pi);
			}
			else
			{
				PLAYER_LOG_ERROR("player is leave world, but do not find the player state in map, player <%llu>", pack_info.guid);
				// not finish the jump map,clear the jum map info from line scene
				
			}
		}
		else
		{
			PLAYER_LOG_ERROR("player is leave world, but do not find the player brief info, player <%llu>", pack_info.guid);
		}
		return SD_INTEGRITY_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	uint64 curr_time = getCurrentTimeInSec();

	int online_time = curr_time - player->last_login_time();

	m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 102, 102, pack_info.guid, curr_time, online_time, player->vip_level(), m_plugin_cfg.template_id);

	//notifyPlayerIsOffline(pack_info.guid);

	//m_plugin_cfg.line_scene->playerLogoff(player->guid());

	m_plugin_cfg.scene->playerOffline(player->guid());

	m_plugin_cfg.pool->playerIsOffline(player->guid());

	MANAGE_GRID_REMOVE(m_plugin_cfg.manage_grid, player);

	++m_player_info.flush_player_request_no;

	//m_player_assistant.request_flush_player(pack_info.guid, false);
	m_need_flush_player_map[pack_info.guid] = false;

	return 0;
}

int ManagePlayer::cmsg_jump_map(const PackInfo & pack_info)
{
	PLAYER_LOG_DEBUG("enter jump map");

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	typed::protocol::cmsg_player_jump_map * jump_map = (typed::protocol::cmsg_player_jump_map *) pack_info.msg;

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(jump_map->target_map_id() != m_plugin_cfg.template_id, CE_NCHECK_CODE);

	return playerJumpMap(player, jump_map->target_map_id());

}

int ManagePlayer::gmsg_player_jump_map(const PackInfo & pack_info)
{
	TransferPlayerInfo * transfer_player_info = m_plugin_cfg.line_scene->obtainPlayer(pack_info.guid, m_plugin_cfg.template_id);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(transfer_player_info, SE_EDATA_CODE);

	for (GuidMsgMap_t::iterator it = transfer_player_info->msg_map.begin(); it != transfer_player_info->msg_map.end(); ++it)
	{
		POOL_OP_ADD(m_plugin_cfg.pool, it->first, it->second, false, transfer_player_info->player_guid);
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	GET_ATTACH_FROM_POOL(m_plugin_cfg.pool, player->attach(), attache);
	if (NULL == attache)
	{
		PLAYER_LOG_ERROR("");
	}

	m_player_state[player->guid()] = PlayerAttachInfo(POS_ONLINE);

	m_plugin_cfg.scene->playerOnline(player->guid());

	delete transfer_player_info;

	return 0;
}

int ManagePlayer::cmsg_jump_map_ready(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	GET_ATTACH_FROM_POOL(m_plugin_cfg.pool, player->attach(), attach);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(attach, SE_NGUID_CODE);

	if (m_plugin_cfg.pool->isPlayerOnlineInThisMap(player->guid()))
	{
		PLAYER_LOG_ERROR("recv jump map ready msg, but the player is already on this map <%llu>", player->guid());
		return CE_NCHECK_CODE;
	}

	m_plugin_cfg.pool->playerIsOnline(player->guid());

	MANAGE_GRID_ADD(m_plugin_cfg.manage_grid, player);

	m_plugin_cfg.scene->collectPlayerLevel(player->guid(), player->level());
	m_plugin_cfg.scene->playerOnline(player->guid());

	checkPlayerCleanDay(player);

	checkPlayerCleanWeek(player);

	{
		PackInfo * pi = new PackInfo(LMSG_PLAYER_JUMPMAP_READY, player->guid(), NULL);
		m_plugin_cfg.line_scene->scenePackInput(pi);
	}

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_PLAYER_ENTER_MAP, pack_info.guid, NULL);

	return 0;
}

int ManagePlayer::cmsg_request_role_attr(const PackInfo & pack_info)
{
	typed::protocol::cmsg_request_role_attr * request_role_attr = (typed::protocol::cmsg_request_role_attr *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(request_role_attr, CE_EPARAM_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	//GET_ROLE_FROM_POOL(m_plugin_cfg.pool, request_role_attr->role_guid(), role);
	//PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, SE_NGUID_CODE);

	//PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN((role->player() == player->guid()), CE_EPARAM_CODE);
	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, request_role_attr->role_guid(), role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, SE_NGUID_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(player->guid() == role->player(), CE_NCHECK_CODE);

	typed::protocol::smsg_request_role_attr smsg_role_attr;
	smsg_role_attr.set_role_guid(role->guid());
	fillRoleAttr(smsg_role_attr.mutable_role_attr(), role, player);
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_REQUEST_ROLE_ATTR, player->guid(), smsg_role_attr.SerializeAsString());

	return 0;
}

/* 客户端角色属性
1	等级
2	hp总值
3	外功攻击
4	外功防御
5	内功攻击
6	内功防御
7	奇术攻击
8	奇术防御
9	命中率
10	闪避率
11	暴击率
12	格挡率
13	连击概率
14	合击概率
15	总先攻值
16	装备幻化先攻值
17	装备强化先攻值
18	心法等级先攻值
19	三围培养先攻值
20	轻功等级先攻值
21	脉轮等级先攻值
22	武馆训练先攻值
23	成员等级先攻值
24  韧性
25  破击
*/
int ManagePlayer::cmsg_request_role_attr_rank(const PackInfo & pack_info)
{
	typed::protocol::cmsg_request_role_attr_rank * request_role_attr = (typed::protocol::cmsg_request_role_attr_rank *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(request_role_attr, CE_EPARAM_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	//GET_ROLE_FROM_POOL(m_plugin_cfg.pool, request_role_attr->role_guid(), role);
	//PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, SE_NGUID_CODE);

	//PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN((role->player() == player->guid()), CE_EPARAM_CODE);
	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, request_role_attr->role_guid(), role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, SE_NGUID_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(player->guid() == role->player(), CE_NCHECK_CODE);

	typed::protocol::smsg_request_role_attr_rank smsg_role_attr;
	smsg_role_attr.set_role_guid(role->guid());
	fillRoleAttrRank(smsg_role_attr.mutable_role_attr(), role, player, m_system_res_info);
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_REQUEST_ROLE_ATTR_RANK, player->guid(), smsg_role_attr.SerializeAsString());

	return 0;
}

int ManagePlayer::cmsg_loop_back(const PackInfo & pack_info)
{
	MAKE_NEW_PACKET(ps, CMSG_LOOPBACK, pack_info.guid, pack_info.msg->SerializeAsString());

	SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);

	return 0;
}

int ManagePlayer::smsg_scene_inited(const PackInfo & pack_info)
{
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_SCENE_INITED, 0, NULL);
	return 0;
}

int ManagePlayer::smsg_lost_gate_connection(const PackInfo & pack_info)
{
	PLAYER_LOG_INFO("receive lost gate connection message, start to flush all players, line is <%d> map is <%d>, player num is <%d>", m_plugin_cfg.line_no, m_plugin_cfg.template_id, m_plugin_cfg.pool->getOnlinePlayer().size());

	m_lost_gate_connection = true;

	if (IS_LINE_SCENE())
	{
		//Uint64Set_t transfer_player_set = m_plugin_cfg.line_scene->getTransferPlayerSet();
		//for (Uint64Set_t::iterator it = transfer_player_set.begin(); it != transfer_player_set.end(); ++it)
		//{
		//	uint64 player_guid = *it;
		//	TransferPlayerInfo * transfer_player_info = m_plugin_cfg.line_scene->obtainPlayer(player_guid, m_plugin_cfg.template_id);
		//	if (NULL != transfer_player_info)
		//	{
		//		for (GuidMsgMap_t::iterator sub_it = transfer_player_info->msg_map.begin(); sub_it != transfer_player_info->msg_map.end(); ++sub_it)
		//		{
		//			POOL_OP_ADD(m_plugin_cfg.pool, sub_it->first, sub_it->second, false, transfer_player_info->player_guid);
		//		}
		//		m_need_flush_player_after_lost_gate_connection.insert(player_guid);
		//	}
		//	else
		//	{
		//		PLAYER_LOG_ERROR("failed to get player transfer info while lost gate connection, player <%llu>", *it);
		//	}
		//}
	}
	else
	{
		m_need_flush_player_after_lost_gate_connection = m_plugin_cfg.pool->getOnlinePlayer();
	}


	for (PlayerGuidSet_t::iterator it = m_need_flush_player_after_lost_gate_connection.begin(); it != m_need_flush_player_after_lost_gate_connection.end(); ++it)
	{
		if (!IS_LINE_SCENE())
		{
			GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, *it, player);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(player, continue);

			MANAGE_GRID_REMOVE(m_plugin_cfg.manage_grid, player);
		}

		m_need_flush_player_map[*it] = false;
	}

	for (NeedFlushPlayerMap_t::iterator it = m_need_flush_player_map.begin(); it != m_need_flush_player_map.end(); ++it)
	{
		m_need_flush_player_after_lost_gate_connection.insert(it->first);
	}
	//m_need_flush_player_after_lost_gate_connection = m_need_flush_player_set;

	if (m_need_flush_player_map.size() == 0)
	{
		m_flush_entity_finish.flush_player_finish = true;
	}

	if ((m_need_flush_player_map.size() == 0) && (!IS_LINE_SCENE()))
	{
		DEF_LOG_INFO("the player size is zero after lost gate connection, line is <%d>, map is <%d>", m_plugin_cfg.line_no, m_plugin_cfg.template_id);
		//m_plugin_cfg.scene->allPlayersFlushedAfterLostGateConnection();
		m_shutdown_success = true;
	}

	//if (m_need_flush_player_after_lost_gate_connection.size() == 0)
	//{
	//	// no players online, 
	//	//m_plugin_cfg.scene->allPlayersFlushedAfterLostGateConnection();
	//	m_flush_entity_finish.flush_player_finish = true;
	//	checkFlushStatus();
	//	return 0;
	//}

	//for (PlayerGuidSet_t::iterator it = m_need_flush_player_after_lost_gate_connection.begin(); it != m_need_flush_player_after_lost_gate_connection.end(); ++it)
	//{
	//	if (!IS_LINE_SCENE())
	//	{
	//		GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, *it, player);
	//		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(player, continue);

	//		MANAGE_GRID_REMOVE(m_plugin_cfg.manage_grid, player);
	//	}
	//	m_player_assistant.request_flush_player(*it, false);
	//}

	return 0;
}

int ManagePlayer::lmsg_request_player_fight_data(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	if (NULL == player)
	{
		// not find, 
		//PackInfo * pi = new PackInfo(pack_info.opcode, pack_info.guid, NULL);
		//pi->msg = cloneMessage(pack_info.msg);
		m_plugin_cfg.line_scene->transmitPlayerMsg((PackInfo *)&pack_info);
	}
	else
	{
		CollectMsgInfo * collect_msg_info = m_plugin_cfg.line_scene->collectPlayerFightDataMsgInfo(m_plugin_cfg, player);

		m_plugin_cfg.line_scene->collectMessage(player->guid(), collect_msg_info);
	}

	return 0;
}

int ManagePlayer::lmsg_get_data_from_other_scene_back(const PackInfo & pack_info)
{
	m_plugin_cfg.scene_request->playerFightDataBackFromScene(pack_info.guid, true);
	return 0;
}

int ManagePlayer::lmsg_get_data_from_other_scene_back_failed(const PackInfo & pack_info)
{
	m_plugin_cfg.scene_request->playerFightDataBackFromScene(pack_info.guid, false);
	return 0;
}

int ManagePlayer::lmsg_day_change(const PackInfo & pack_info)
{
	typed::protocol::lmsg_day_change * day_change = (typed::protocol::lmsg_day_change *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(day_change, SE_EPLUGIN_CODE);

	m_current_day = day_change->new_day();

	notifyDayChange();

	return 0;
}

int ManagePlayer::lmsg_week_change(const PackInfo & pack_info)
{
	typed::protocol::lmsg_week_change * week_change = (typed::protocol::lmsg_week_change *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(week_change, SE_EPLUGIN_CODE);

	m_current_week = week_change->new_week();

	notifyWeekChange();

	return 0;
}

int ManagePlayer::lmsg_player_charge(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	GET_COIN_FROM_POOL(m_plugin_cfg.pool, player->coin_guid(), coin);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, SE_NGUID_CODE);

	typed::protocol::lmsg_player_charge * player_charge = (typed::protocol::lmsg_player_charge *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player_charge, SE_EPLUGIN_CODE);

	GET_VIP_FROM_POOL(m_plugin_cfg.pool, player->vip(), vip);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(vip, SE_NGUID_CODE);

//	PLAYER_LOG_INFO("player charge, player is <%llu>, charge value is <%d>, request index is <%d>", pack_info.guid, player_charge->charge_value(), player_charge->request_index());
	bool is_valid_charge = is_valid_charge_request(vip, player_charge->request_index(), player_charge->charge_value());

	if (is_valid_charge)
	{
		if (vip->first_charge_gift() == 1)
		{
			vip->set_first_charge_gift(2);
			POOL_OP_UPDATE(m_plugin_cfg.pool, vip->guid());
			typed::protocol::smsg_update_first_charge_gift suf_charge_gift;
			suf_charge_gift.set_status(2);
			MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_UPDATE_FIRST_CHARGE_GIFT, pack_info.guid, suf_charge_gift.SerializeAsString());
		}

		updateVipExp(player, player_charge->charge_value(), 264);

		updateCoin(player, coin, player_charge->charge_value(), 264, true);

		recodeChargeInfo(vip, player_charge->request_index(), player_charge->charge_value());

		notifyPlayerUpdateMoney(player);

		typed::protocol::nmsg_player_charge * np_charge = new typed::protocol::nmsg_player_charge();
		np_charge->set_charge_value(player_charge->charge_value());
		np_charge->set_total_charge_value(vip->charge_coin());
		uint32 charge_data = getTimeDifferentDay(player_charge->charge_time(), m_plugin_cfg.line_scene->getLunchInfo().first_lunch_time) + 1;
		np_charge->set_charge_date(charge_data);
		MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_PLAYER_CHARGE, player->guid(), np_charge);

		typed::protocol::lmsg_player_notify * lp_notify = new typed::protocol::lmsg_player_notify();
		lp_notify->set_id(403);

		lp_notify->add_param_id(player_charge->charge_value());
		lp_notify->add_param_name("");

		lp_notify->add_param_id(player->vip_level());
		lp_notify->add_param_name("");
		PackInfo * pi = new PackInfo(LMSG_PLAYER_NOTIFY, player->guid(), lp_notify);
		m_plugin_cfg.line_scene->scenePackInput(pi);

	}


	typed::protocol::lmsg_player_charge_confirm * charge_confirm = new typed::protocol::lmsg_player_charge_confirm();
	charge_confirm->set_request_index(player_charge->request_index());
	charge_confirm->set_charge_value(player_charge->charge_value());
	if (is_valid_charge)
	{
		charge_confirm->set_result(1);
	}
	else
	{
		charge_confirm->set_result(2);
	}

	PackInfo * pi = new PackInfo(LMSG_PLAYER_CHARGE_CONFIRM, pack_info.guid, charge_confirm);
	m_plugin_cfg.line_scene->scenePackInput(pi);

	return 0;
}

int ManagePlayer::cmsg_get_server_time(const PackInfo & pack_info)
{
	typed::protocol::smsg_get_server_time sgs_time;

	uint64 curr_time = 0;
	ACE_OS::gettimeofday().to_usec(curr_time);
	curr_time = curr_time / (1000 * 1000);
	sgs_time.set_server_time((uint32)curr_time);

	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_GET_SERVER_TIME, pack_info.guid, sgs_time.SerializeAsString());
	return 0;
}

int ManagePlayer::cmsg_special_action(const PackInfo & pack_info)
{
	typed::protocol::cmsg_special_action * cs_action = (typed::protocol::cmsg_special_action *)pack_info.msg;

	typed::protocol::smsg_special_action sp_action;
	sp_action.set_player_guid(pack_info.guid);
	sp_action.set_action_type(cs_action->action_type());
	sp_action.set_param(cs_action->param());

	MAKE_NEW_PACKET(ps, SMSG_SPECIAL_ACTION, pack_info.guid, sp_action.SerializeAsString());

	m_plugin_cfg.manage_grid->broadcastPacket(ps);

	return 0;
}

int ManagePlayer::cmsg_special_action_person(const PackInfo & pack_info)
{
	typed::protocol::cmsg_special_action_person * csa_persion = (typed::protocol::cmsg_special_action_person *)pack_info.msg;

	typed::protocol::smsg_special_action_person ssa_persion;
	ssa_persion.set_action_type(csa_persion->action_type());
	ssa_persion.set_param(csa_persion->param());
	ssa_persion.set_player_guid(pack_info.guid);
	for (int i = 0; i < csa_persion->player_guid_size(); ++i)
	{
		MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_SPECIAL_ACTION_PERSON, csa_persion->player_guid(i), ssa_persion.SerializeAsString());
	}

	return 0;
}

int ManagePlayer::ltos_short_init_finish(const PackInfo & pack_info)
{
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_SHORT_INIT_FINISH, 0, NULL);
	return 0;
}

int ManagePlayer::cmsg_select_inherit_role(const PackInfo & pack_info)
{
	typed::protocol::cmsg_select_inherit_role * csi_role = (typed::protocol::cmsg_select_inherit_role *)pack_info.msg;

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(csi_role->from_role_guid() != csi_role->to_role_guid(), CE_NCHECK_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, csi_role->from_role_guid(), from_role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(from_role, SE_NGUID_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(player->guid() == from_role->player(), CE_NCHECK_CODE);

	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, csi_role->to_role_guid(), to_role);

	if (NULL != to_role)
	{
		PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(player->guid() == to_role->player(), CE_NCHECK_CODE);
	}

	typed::protocol::nmsg_inherit_select * ni_select = new typed::protocol::nmsg_inherit_select();
	ni_select->set_from_role_guid(from_role->guid());
	ni_select->set_to_role_guid(csi_role->to_role_guid());

	PackInfo pi(NMSG_INHERIT_SELECT, player->guid(), ni_select);
	m_plugin_cfg.message->notifyMsgToPlugins(pi);

	// todo
	typed::protocol::smsg_select_inherit_role ssi_role;
	ssi_role.set_from_role_guid(csi_role->from_role_guid());
	ssi_role.set_to_role_guid(csi_role->to_role_guid());

	if (NULL != to_role)
	{
		// from role info
		ssi_role.mutable_from_role_info()->mutable_edu_attr()->set_str_attr(ni_select->mutable_to_role_info()->mutable_edu_attr()->str_attr());
		ssi_role.mutable_from_role_info()->mutable_edu_attr()->set_agi_attr(ni_select->mutable_to_role_info()->mutable_edu_attr()->agi_attr());
		ssi_role.mutable_from_role_info()->mutable_edu_attr()->set_int_attr(ni_select->mutable_to_role_info()->mutable_edu_attr()->int_attr());

		ssi_role.mutable_from_role_info()->mutable_pellet_attr()->set_str_attr(ni_select->mutable_to_role_info()->mutable_pellet_attr()->str_attr());
		ssi_role.mutable_from_role_info()->mutable_pellet_attr()->set_agi_attr(ni_select->mutable_to_role_info()->mutable_pellet_attr()->agi_attr());
		ssi_role.mutable_from_role_info()->mutable_pellet_attr()->set_int_attr(ni_select->mutable_to_role_info()->mutable_pellet_attr()->int_attr());

		ssi_role.mutable_from_role_info()->set_training_level(ni_select->mutable_to_role_info()->training_level());

		fillInheritSubToken(ssi_role.mutable_from_role_info()->mutable_yin_shl(), ni_select->mutable_to_role_info()->mutable_yin_shl());
		fillInheritSubToken(ssi_role.mutable_from_role_info()->mutable_yang_shl(), ni_select->mutable_to_role_info()->mutable_yang_shl());

		ssi_role.mutable_from_role_info()->mutable_talentid()->CopyFrom(ni_select->mutable_from_role_info()->talentid());


		// to role info
		ssi_role.mutable_to_role_info()->mutable_edu_attr()->set_str_attr(ni_select->mutable_from_role_info()->mutable_edu_attr()->str_attr());
		ssi_role.mutable_to_role_info()->mutable_edu_attr()->set_agi_attr(ni_select->mutable_from_role_info()->mutable_edu_attr()->agi_attr());
		ssi_role.mutable_to_role_info()->mutable_edu_attr()->set_int_attr(ni_select->mutable_from_role_info()->mutable_edu_attr()->int_attr());

		ssi_role.mutable_to_role_info()->mutable_pellet_attr()->set_str_attr(ni_select->mutable_from_role_info()->mutable_pellet_attr()->str_attr());
		ssi_role.mutable_to_role_info()->mutable_pellet_attr()->set_agi_attr(ni_select->mutable_from_role_info()->mutable_pellet_attr()->agi_attr());
		ssi_role.mutable_to_role_info()->mutable_pellet_attr()->set_int_attr(ni_select->mutable_from_role_info()->mutable_pellet_attr()->int_attr());

		ssi_role.mutable_to_role_info()->set_training_level(ni_select->mutable_from_role_info()->training_level());

		fillInheritSubToken(ssi_role.mutable_to_role_info()->mutable_yin_shl(), ni_select->mutable_from_role_info()->mutable_yin_shl());
		fillInheritSubToken(ssi_role.mutable_to_role_info()->mutable_yang_shl(), ni_select->mutable_from_role_info()->mutable_yang_shl());

		ssi_role.mutable_to_role_info()->mutable_talentid()->CopyFrom(ni_select->mutable_to_role_info()->talentid());
	}
	else
	{
		ssi_role.mutable_from_role_info()->mutable_edu_attr()->set_str_attr(ni_select->mutable_from_role_info()->mutable_edu_attr()->str_attr());
		ssi_role.mutable_from_role_info()->mutable_edu_attr()->set_agi_attr(ni_select->mutable_from_role_info()->mutable_edu_attr()->agi_attr());
		ssi_role.mutable_from_role_info()->mutable_edu_attr()->set_int_attr(ni_select->mutable_from_role_info()->mutable_edu_attr()->int_attr());

		ssi_role.mutable_from_role_info()->mutable_pellet_attr()->set_str_attr(ni_select->mutable_from_role_info()->mutable_pellet_attr()->str_attr());
		ssi_role.mutable_from_role_info()->mutable_pellet_attr()->set_agi_attr(ni_select->mutable_from_role_info()->mutable_pellet_attr()->agi_attr());
		ssi_role.mutable_from_role_info()->mutable_pellet_attr()->set_int_attr(ni_select->mutable_from_role_info()->mutable_pellet_attr()->int_attr());

		ssi_role.mutable_from_role_info()->set_training_level(ni_select->mutable_from_role_info()->training_level());

		fillInheritSubToken(ssi_role.mutable_from_role_info()->mutable_yin_shl(), ni_select->mutable_from_role_info()->mutable_yin_shl());
		fillInheritSubToken(ssi_role.mutable_from_role_info()->mutable_yang_shl(), ni_select->mutable_from_role_info()->mutable_yang_shl());
		ssi_role.mutable_from_role_info()->mutable_talentid()->CopyFrom(ni_select->mutable_from_role_info()->talentid());
	}

	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_SELECT_INHERIT_ROLE, player->guid(), ssi_role.SerializeAsString());

	return 0;
}

int ManagePlayer::cmsg_inherit_role(const PackInfo & pack_info)
{
	typed::protocol::cmsg_inherit_role * ci_role = (typed::protocol::cmsg_inherit_role *)pack_info.msg;

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(ci_role->from_role_guid() != ci_role->to_role_guid(), CE_NCHECK_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, ci_role->from_role_guid(), from_role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(from_role, SE_NGUID_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(player->guid() == from_role->player(), CE_NCHECK_CODE);

	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, ci_role->to_role_guid(), to_role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(to_role, SE_NGUID_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(player->guid() == to_role->player(), CE_NCHECK_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(player->roles(0) != from_role->guid(), CE_NCHECK_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(player->roles(0) != to_role->guid(), CE_NCHECK_CODE);

	typed::protocol::pmsg_inherit_check_equipment * pic_equ = new typed::protocol::pmsg_inherit_check_equipment();
	pic_equ->set_from_role_guid(from_role->guid());
	pic_equ->set_to_role_guid(to_role->guid());

	{
		PackInfo pi(PMSG_INHERIT_CHECK_EQUIPMENT, pack_info.guid, pic_equ);
		m_plugin_cfg.message->requestMsgToPlugins(pi);
		PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(pic_equ->result() == 1, CE_NCHECK_CODE);
	}

	typed::protocol::smsg_inherit_role si_role;
	si_role.set_from_role_guid(ci_role->from_role_guid());
	si_role.set_to_role_guid(ci_role->to_role_guid());

	bool reduce_money_success = false;
	uint32 cost_money = 0;
	uint32 cost_coin = 0;
	reduce_money_success = reduceInheritMoney(player, from_role->level(), 0, cost_money, cost_coin);
	if (!reduce_money_success)
	{
		si_role.set_result(2);

		MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_INHERIT_ROLE, player->guid(), si_role.SerializeAsString());
		return 0;
	}

	si_role.set_result(1);

	typed::protocol::nmsg_inherit_excute * ni_excute = new typed::protocol::nmsg_inherit_excute();
	ni_excute->set_from_role_guid(from_role->guid());
	ni_excute->set_to_role_guid(to_role->guid());

	PackInfo pi(NMSG_INHERIT_EXCUTE, player->guid(), ni_excute);

	m_plugin_cfg.message->notifyMsgToPlugins(pi);

	uint32 from_role_level = from_role->level();
	uint32 to_role_level = to_role->level();

	from_role->set_level(to_role_level);
	to_role->set_level(from_role_level);

	if (from_role_level > to_role_level)
	{
		levelReduce(from_role, from_role_level, to_role_level);
		levelUpgrade(to_role, to_role_level, from_role_level);
	}
	else if (from_role_level < to_role_level)
	{
		levelUpgrade(from_role, from_role_level, to_role_level);
		levelReduce(to_role, to_role_level, from_role_level);
	}

	uint32 from_role_exp = from_role->exp();
	from_role->set_exp(to_role->exp());
	to_role->set_exp(from_role_exp);

	notifyRoleLevelUp(player, to_role, to_role_level);

	notifyRoleLevelUp(player, from_role, from_role_level);

	uint32 from_role_fight_power = m_plugin_cfg.scene->calcRoleFightPower(from_role);
	uint32 to_role_fight_power = m_plugin_cfg.scene->calcRoleFightPower(to_role);

	si_role.set_from_role_fight_power(from_role_fight_power);
	si_role.set_to_role_fight_power(to_role_fight_power);
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_INHERIT_ROLE, player->guid(), si_role.SerializeAsString());

	notifyRoleAttrUpdate(from_role);
	notifyRoleAttrUpdate(to_role);

	POOL_OP_UPDATE(m_plugin_cfg.pool, to_role->guid());

	POOL_OP_UPDATE(m_plugin_cfg.pool, from_role->guid());

	from_role->set_inherit_from(from_role->inherit_from() + 1);
	to_role->set_inherit_to(to_role->inherit_to() + 1);
	

	m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 2902, 2902, player->guid(), player->name(), player->level(), cost_money, cost_coin, from_role->level(), to_role_level, to_role->level(), from_role->role_id(), to_role->role_id());

	return 0;
}

int ManagePlayer::ltos_delivery_player_data(const PackInfo & pack_info)
{
	typed::entity::Coin * coin = (typed::entity::Coin *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, SE_EPLUGIN_CODE);

	PackInfo * pi = (PackInfo *)&pack_info;
	pi->msg = NULL;

	m_player_assistant.load_player(coin);

	return 0;
}

int ManagePlayer::cmsg_set_follow_role(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(player->vip_level() >= m_vip_role_follow->unlock_lv, CE_NCHECK_CODE);
	
	typed::protocol::cmsg_set_follow_role * csf_role = (typed::protocol::cmsg_set_follow_role *)pack_info.msg;
	if (csf_role->role_guid() != 0)
	{
		GET_ROLE_FROM_POOL(m_plugin_cfg.pool, csf_role->role_guid(), role);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, SE_NGUID_CODE);

		PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(player->guid() == role->player(), CE_NCHECK_CODE);

		player->set_follow_role_id(role->role_id());
	}
	else
	{
		// clear
		player->set_follow_role_id(0);
	}

	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	typed::protocol::smsg_set_follow_role ssf_role;
	ssf_role.set_role_guid(csf_role->role_guid());
	ssf_role.set_result(1);
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_SET_FOLLOW_ROLE, player->guid(), ssf_role.SerializeAsString());

	// todo
	m_plugin_cfg.manage_grid->updateFollowRoleID(player);
	//typed::protocol::smsg_update_follow_role suf_role;
	//suf_role.set_player_guid(player->guid());
	//suf_role.set_follow_role_id(csf_role->role_guid());
	//MAKE_NEW_PACKET(broad_ps, SMSG_UPDATE_FOLLOW_ROLE, player->guid(), suf_role.SerializeAsString());

	return 0;
}

int ManagePlayer::ltos_inner_request_logoff(const PackInfo & pack_info)
{
	typed::protocol::stol_request_player_data * srp_data = (typed::protocol::stol_request_player_data *)pack_info.msg;
	if (srp_data->map_id() == m_plugin_cfg.template_id)
	{
		return 0;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	if (NULL == player)
	{
		PlayerOnlineStateMap_t::iterator it = m_player_state.find(pack_info.guid);
		if (it != m_player_state.end())
		{
			m_player_state.erase(it);
			m_player_assistant.clearPlayerStatus(pack_info.guid);
		}
	}
	else
	{
		PackInfo pi(CMSG_PLAYER_LEAVE_MAP, pack_info.guid, NULL);
		cmsg_leave_world(pi);
	}

	return 0;
}

int ManagePlayer::nmsg_player_is_online(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	if (player->attach() != 0)
	{
		typed::entity::Attach * attach = new typed::entity::Attach();
		attach->set_guid(player->attach());
		attach->set_player(player->guid());

		POOL_OP_ADD(m_plugin_cfg.pool, attach->guid(), attach, false, attach->player());
	}

	if (player->coin_guid() == 0)
	{
		typed::entity::Coin * coin = new typed::entity::Coin();
		uint64 coin_guid = SWITCH_GUID_TYPE(ET_COIN, pack_info.guid);
		coin->set_guid(coin_guid);
		coin->set_player(pack_info.guid);
		coin->set_coin(player->coin());
		coin->set_money(player->money());

		player->set_coin_guid(coin_guid);

		POOL_OP_ADD(m_plugin_cfg.pool, coin->guid(), coin, true, coin->player());
		POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());
	}

	checkDoubleexp(player);

	updateDoubleexpInfo(player);

	return 0;
}

int ManagePlayer::nmsg_player_is_offline(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	uint64 curr_time = 0;

	ACE_OS::gettimeofday().to_usec(curr_time);
	curr_time /= 1000000;

	player->set_last_logoff_time(curr_time);

	ACE_Date_Time ace_dt;
	char date_str[100] = {0};
	sprintf(date_str, "%04d-%02d-%02dT%02d:%02d:%02d", ace_dt.year(), ace_dt.month(), ace_dt.day(), ace_dt.hour(), ace_dt.minute(), ace_dt.second());
	player->set_last_logoff_time_str(date_str);

	return 0;
}

int ManagePlayer::nmsg_short_flush_finish(const PackInfo & pack_info)
{
	m_flush_entity_finish.flush_short_finish = true;
	checkFlushStatus();
	return 0;
}

int ManagePlayer::nmsg_player_enter_map(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	checkPlayerProvideEnergy(player);

	return 0;
}

int ManagePlayer::nmsg_player_online_dispatch_msg(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	checkPlayerProvideEnergy(player);

	send_open_time(pack_info.guid);
	return 0;
}
void ManagePlayer::send_open_time(uint64 player_guid)
{
	typed::protocol::smsg_server_open_time smsg;
	smsg.set_open_time(m_plugin_cfg.line_scene->getLunchInfo().first_lunch_time);
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_SERVER_OPEN_TIME, player_guid, smsg.SerializeAsString());
}

int ManagePlayer::nmsg_player_level_up(const PackInfo & pack_info)
{
	typed::protocol::pmsg_player_level_upgrade * pl = (typed::protocol::pmsg_player_level_upgrade *)pack_info.msg;
	for (int i = pl->pre_level() + 1; i <= pl->curr_level(); ++i)
	{
		UnlockFunctionMap_t::iterator it = m_system_unlock_res->level_unlock_map.find(i);
		if (it != m_system_unlock_res->level_unlock_map.end())
		{
			vector<uint32> & sys_vec = it->second;
			for (int i = 0; i < sys_vec.size(); ++i)
			{
				notifySystemUnlock(pack_info.guid, sys_vec[i]);
			}
		}
	}
	return 0;
}

int ManagePlayer::nmsg_use_item_notify(const PackInfo & pack_info)
{
	typed::protocol::nmsg_use_item_notify * nui_notify = (typed::protocol::nmsg_use_item_notify *)pack_info.msg;

	if (27 == nui_notify->item_type())
	{
		PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(nui_notify->result() == 0, SE_EPLUGIN_CODE);
		
		GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

		GET_DOUBLE_FROM_POOL(m_plugin_cfg.pool, player->doubleexp(), doubleexp);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(doubleexp, SE_NGUID_CODE);

		DoubleexpRes * new_res = getDoubleexpRes(nui_notify->item_pub1());
		PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(new_res, SE_NRES_CODE);

		if (doubleexp->id() != 0)
		{
			// can use
			DoubleexpRes * pre_res = getDoubleexpRes(doubleexp->id());
			PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pre_res, SE_NRES_CODE);

			if (new_res->group < pre_res->group)
			{
				nui_notify->set_result(2);
				return 0;
			}

			if (new_res->level <= pre_res->level)
			{
				nui_notify->set_result(2);
				return 0;
			}
		}

		// can use
		doubleexp->set_id(new_res->id);
		doubleexp->set_left_num(new_res->times);
		doubleexp->set_mutil_value(new_res->mutil_value);

		POOL_OP_UPDATE(m_plugin_cfg.pool, doubleexp->guid());

		nui_notify->set_result(1);

		updateDoubleexpInfo(player);
	}

	return 0;
}

int ManagePlayer::rmsg_role_add(const PackInfo & pack_info)
{
	typed::protocol::pmsg_add_role * add_role = (typed::protocol::pmsg_add_role *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(add_role, -1);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	//GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, -1);

	RoleResInfo * role_res_info = getRoleResInfo(add_role->role_id());
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role_res_info, -1);

	uint64 role_guid = 0;
	MAKE_NEW_GUID(m_plugin_cfg.scene, ET_ROLE, role_guid);

	typed::entity::Role * role = new typed::entity::Role();
	role->set_player(pack_info.guid);
	role->set_guid(role_guid);

	POOL_OP_ADD(m_plugin_cfg.pool, role->guid(), role, true, player->guid());

	role->set_name(role_res_info->name);
	role->set_sex(role_res_info->sex);
	role->set_job(role_res_info->job);
	role->set_level(role_res_info->level);
	role->set_normal_skill(role_res_info->normal_skill);
	role->set_extremely_skill(role_res_info->extremely_skill);
	role->set_hire_id(add_role->hire_id());
	role->set_role_id(add_role->role_id());
	role->set_char_ability(role_res_info->char_ability);
	role->set_faction(role_res_info->faction);
	role->set_quality(role_res_info->color);

	for (int i = 0; i < 8; ++i)
	{
		role->add_equipments(0);
	}

	initRole(role, role_res_info);

	POOL_OP_UPDATE(m_plugin_cfg.pool, role->guid());
	role->set_postion(0);
	player->add_roles(role->guid());

	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	add_role->set_role_guid(role->guid());

	typed::protocol::nmsg_create_role * n_c_role = new typed::protocol::nmsg_create_role();
	n_c_role->set_player(role->player());
	n_c_role->set_create_role(role->guid());
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_CREATE_ROLE, player->guid(), n_c_role);

	send_create_role_msg(player, role);

	if (role->role_id() == ZSLW_ROLE_ID)
	{
		sendHireZsly(player, 1);
	}

	return 0;
}

int ManagePlayer::rmsg_role_rejoin(const PackInfo & pack_info)
{
	typed::protocol::pmsg_role_rejoin * role_rejoin = (typed::protocol::pmsg_role_rejoin *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role_rejoin, -1);

	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, role_rejoin->role_guid(), role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, -1);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, role->player(), player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, -1);

	//GET_ROLE_RES_INFO(m_role_res_info_map, role_res_info, role->role_id());
	//PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role_res_info, -1);

	//uint32 curr_hp = GET_ROLE_ATTR(role, RAT_CURR_HP);
	//initRole(role, role_res_info);
	//role->set_attributes(RAT_CURR_HP, curr_hp);

	player->add_roles(role->guid());

	send_create_role_msg(player, role);

	typed::protocol::nmsg_role_rejoin * nr_rejoin = new typed::protocol::nmsg_role_rejoin();
	nr_rejoin->set_role_guid(role->guid());
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_ROLE_REJOIN, player->guid(), nr_rejoin);

	//send_create_item_msg(player, role->equipments());		

	return 0;
}

int ManagePlayer::rmsg_update_reward(const PackInfo & pack_info)
{
	typed::protocol::pmsg_update_reward * update_reward = (typed::protocol::pmsg_update_reward *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(update_reward, -1);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	GET_COIN_FROM_POOL(m_plugin_cfg.pool, player->coin_guid(), coin);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, SE_NGUID_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(update_reward->money() >= 0, SE_EPLUGIN_CODE);
	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(update_reward->coin() >= 0, SE_EPLUGIN_CODE);
	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(update_reward->seniority() >= 0, SE_EPLUGIN_CODE);
	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(update_reward->prestige() >= 0, SE_EPLUGIN_CODE);

	GET_DOUBLE_FROM_POOL(m_plugin_cfg.pool, player->doubleexp(), doubleexp);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(doubleexp, SE_NGUID_CODE);

	uint32 mutil_value = 1;
	if ((doubleexp->left_num() > 0)
		&& useMutilexp(update_reward->source_from()))
	{
		mutil_value = doubleexp->mutil_value();

		doubleexp->set_left_num(doubleexp->left_num() - 1);
		if (doubleexp->left_num() == 0)
		{
			doubleexp->set_id(0);
			doubleexp->set_left_num(0);
			doubleexp->set_mutil_value(1);
		}

		POOL_OP_UPDATE(m_plugin_cfg.pool, doubleexp->guid());

		updateDoubleexpInfo(player);
	}

	uint32 source_from = update_reward->source_from();

	if (update_reward->role_guid_size() == update_reward->role_exp_size())
	{
		for (int i = 0; i < update_reward->role_guid_size(); ++i)
		{
			//if (0 == i)
			//{
			//	updatePlayerExp(update_reward->player_guid(), update_reward->role_exp(i));
			//}
			uint32 role_exp = update_reward->role_exp(i) * mutil_value;

			updateRoleExp(update_reward->role_guid(i), role_exp, player->addiction_status(), i, update_reward->source_from());
		}
	}
	else
	{
		PLAYER_LOG_ERROR("update_reward role guid size does not equal role exp size");
	}

	if (update_reward->seniority() != 0)
	{
		updateSeniority(player, update_reward->seniority(), source_from);
	}

	if (update_reward->money() != 0)
	{
		updateMoney(player, coin, update_reward->money(), source_from);
	}

	if (update_reward->coin() != 0)
	{
		updateCoin(player, coin, update_reward->coin(), source_from, false);
	}

	if (update_reward->prestige() != 0)
	{
		updatePrestige(player, update_reward->prestige(), source_from);

		typed::protocol::smsg_update_prestige su_prestige;
		su_prestige.set_prestige_value(player->prestige());
		MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_UPDATE_PRESTIGE, player->guid(), su_prestige.SerializeAsString());
	}

	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	notifyPlayerUpdateMoney(player);
	return 0;
}

int ManagePlayer::pmsg_reduce_gold(const PackInfo & pack_info)
{
	typed::protocol::pmsg_reduce_gold * reduce_gold = (typed::protocol::pmsg_reduce_gold *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(reduce_gold, -1);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, -1);

	GET_COIN_FROM_POOL(m_plugin_cfg.pool, player->coin_guid(), coin);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, SE_NGUID_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(reduce_gold->money() >= 0, SE_EPLUGIN_CODE);
	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(reduce_gold->coin() >= 0, SE_EPLUGIN_CODE);
	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(reduce_gold->seniority() >= 0, SE_EPLUGIN_CODE);

	if ((reduce_gold->money() > player->money())
		|| (reduce_gold->coin() > player->coin())
		|| (reduce_gold->seniority() > player->seniority()))
	{
		reduce_gold->set_result(2);
		return -1;
	}

	typed::protocol::smsg_money_update money_update;

	if (reduce_gold->money() != 0)
	{
		updateMoney(player, coin, 0 - reduce_gold->money(), reduce_gold->source_from());
	}
	money_update.set_curr_money(player->money());

	if (reduce_gold->coin() != 0)
	{
		updateCoin(player, coin, 0 - reduce_gold->coin(), reduce_gold->function_id(), false);
	}
	money_update.set_curr_coin(player->coin());

	if (reduce_gold->seniority() != 0)
	{
		updateSeniority(player, 0 - reduce_gold->seniority(), reduce_gold->source_from());
	}
	money_update.set_curr_seniority(player->seniority());

	reduce_gold->set_result(true);

	MAKE_NEW_PACKET(ps, SMSG_MONEY_UPDATE, player->guid(), money_update.SerializeAsString());
	SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);

	return 0;
}

int ManagePlayer::pmsg_player_jump_map(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	typed::protocol::pmsg_player_jump_map * p_msg_jump = (typed::protocol::pmsg_player_jump_map *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(p_msg_jump, SE_EPLUGIN_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(p_msg_jump->target_map() != player->map_id(), SE_GMCMD_PARAM_CODE);

	return playerJumpMap(player, p_msg_jump->target_map());
}

int ManagePlayer::pmsg_reduce_energy(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	typed::protocol::pmsg_reduce_energy * reduce_energy = (typed::protocol::pmsg_reduce_energy *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(reduce_energy, SE_EPLUGIN_CODE);

	if (reduce_energy->reduce_energy() <= 0)
	{
		reduce_energy->set_result(1);
		return 0;
	}

	reduce_energy->set_result(2);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(reduce_energy->reduce_energy() > 0, SE_EPLUGIN_CODE);

	int energy = reduce_energy->reduce_energy();
	if (player->left_energy_reward() > 0)
	{
		if (player->left_energy_reward() >= reduce_energy->reduce_energy())
		{
			player->set_left_energy_reward(player->left_energy_reward() - reduce_energy->reduce_energy());
			reduce_energy->set_result(1);
			energy = 0;
		}
		else
		{
			energy = reduce_energy->reduce_energy() - player->left_energy_reward();
			player->set_left_energy_reward(0);
		}
		notifyClientProvideEnergy(player->guid(), player->left_energy_reward());
	}

	if (0 == energy)
	{
		return 0;
	}

	energy = player->energy() - energy;
	if (energy < 0)
	{
		PLAYER_LOG_ERROR("reduce energy error, reduce value <%d> is more than player energy <%d>", reduce_energy->reduce_energy(), player->energy());
		return SE_EPLUGIN_CODE;
	}

	int old_energy = player->energy();

	reduce_energy->set_result(1);

	player->set_energy(energy);

	notifyPlayerUpdateEnergy(player);

	uint32 source_from = reduce_energy->source_from();

	m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 252, reduce_energy->source_from(), player->guid(), player->name(), player->level(), 
		reduce_energy->reduce_energy(), old_energy, player->energy());

	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	return 0;
	//typed::protocol::smsg_update_energy 
}

int ManagePlayer::pmsg_query_energy(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	typed::protocol::pmsg_query_energy * query_energy = (typed::protocol::pmsg_query_energy *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(query_energy, SE_EPLUGIN_CODE);

	query_energy->set_query_energy(player->energy() + player->left_energy_reward());

	return 0;
}

int ManagePlayer::pmsg_add_energy(const PackInfo & pack_info)
{
	typed::protocol::pmsg_add_energy * p_add_energy = (typed::protocol::pmsg_add_energy *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(p_add_energy, SE_EPLUGIN_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	int old_energy = player->energy();

	if (p_add_energy->can_exceed_limit() == 1)
	{
		player->set_energy(player->energy() + p_add_energy->energy());
	}
	else
	{
		if (player->energy() >= m_system_res_info->tili)
		{
			return 0;
		}
		else
		{
			int energy = ::min(int(player->energy() + p_add_energy->energy()), int(m_system_res_info->tili));
			player->set_energy(energy);
		}
	}

	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	notifyPlayerUpdateEnergy(player);

	uint32 source_from = p_add_energy->source_from();

	if (!IS_SOURCE_FROM_LS(p_add_energy->source_from()))
	{
		m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 251, p_add_energy->source_from(), player->guid(), player->name(), player->level(), 
			player->energy() - old_energy, old_energy, player->energy());
	}

	typed::protocol::nmsg_energy_increase * ne_increase = new typed::protocol::nmsg_energy_increase();
	ne_increase->set_curr_energy(player->energy());

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_ENERGY_INCREASE, player->guid(), ne_increase);

	return 0;
}

int ManagePlayer::pmsg_update_prestige(const PackInfo & pack_info)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	typed::protocol::pmsg_update_prestige * update_prestige = (typed::protocol::pmsg_update_prestige *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(update_prestige, SE_EPLUGIN_CODE);

	updatePrestige(player, update_prestige->value(), update_prestige->source_from());
	//int increase_value = update_prestige->value();
	//GET_ADDICTION_VALUE(increase_value, player->addiction_status());
	//if (0 == increase_value)
	//{
	//	return 0;
	//}

	//int v = player->prestige() + increase_value;

	//PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(v >= 0, SE_EPLUGIN_CODE);

	//player->set_prestige(v);

	//typed::protocol::nmsg_prestige_increase * np_increase = new typed::protocol::nmsg_prestige_increase();
	//np_increase->set_curr_prestige(v);

	//MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_PRESTIGE_INCREASE, player->guid(), np_increase);

	typed::protocol::smsg_update_prestige su_prestige;
	su_prestige.set_prestige_value(player->prestige());
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_UPDATE_PRESTIGE, player->guid(), su_prestige.SerializeAsString());

	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	return 0;
}

int ManagePlayer::pmsg_update_reverence(const PackInfo & pack_info)
{
	typed::protocol::pmsg_update_reverence * pu_reverence = (typed::protocol::pmsg_update_reverence *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pu_reverence, SE_EPLUGIN_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	int increase_value = pu_reverence->value();
	GET_ADDICTION_VALUE(increase_value, player->addiction_status());
	if (0 == increase_value)
	{
		return 0;
	}
	
	int v = player->reverence() + increase_value;

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(v >= 0, SE_EPLUGIN_CODE);

	player->set_reverence(v);

	typed::protocol::smsg_update_reverence su_reverence;
	su_reverence.set_reverence_value(player->reverence());

	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_UPDATE_REVERENCE, pack_info.guid, su_reverence.SerializeAsString());

	return 0;
}

int ManagePlayer::pmsg_reduce_reverence(const PackInfo & pack_info)
{
	typed::protocol::pmsg_reduce_reverence * pr_reverence = (typed::protocol::pmsg_reduce_reverence *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pr_reverence, SE_EPLUGIN_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	uint32 reduce_value = pr_reverence->value();
	if (reduce_value <= player->reverence())
	{
		player->set_reverence(player->reverence() - reduce_value);

		typed::protocol::smsg_update_reverence su_reverence;
		su_reverence.set_reverence_value(player->reverence());

		MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_UPDATE_REVERENCE, pack_info.guid, su_reverence.SerializeAsString());

		pr_reverence->set_result(1);
	}
	else
	{
		pr_reverence->set_result(2);
	}

	return 0;
}

int ManagePlayer::pmsg_change_char_ability(const PackInfo & pack_info)
{
	typed::protocol::pmsg_change_char_ability * char_ability = (typed::protocol::pmsg_change_char_ability *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(char_ability, SE_EPLUGIN_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, char_ability->role_guid(), role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, SE_NGUID_CODE);

	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(role->player() == player->guid(), SD_INTEGRITY_CODE);

	role->set_char_ability(char_ability->skill_id());



	return 0;
}

int ManagePlayer::pmsg_query_system_unlock(const PackInfo & pack_info)
{
	typed::protocol::pmsg_query_system_unlock * pqs_unlock = (typed::protocol::pmsg_query_system_unlock *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pqs_unlock, SE_EPLUGIN_CODE);

	if (0 == pack_info.guid)
	{
		LevelUnlockFun_t::iterator it = m_level_unlock_function.find(pqs_unlock->system_id());
		if (it != m_level_unlock_function.end())
		{
			// level unlock

			pqs_unlock->set_unlock_level(it->second);
		}
		else
		{
			pqs_unlock->set_unlock_level(0);
		}
		return 0;
	}

	int player_level = 0;
	if (IS_LINE_SCENE())
	{
		PlayerBriefInfo * pb_info = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pb_info, SE_EPLUGIN_CODE);
		player_level = pb_info->short_info->level();

		LevelUnlockFun_t::iterator it = m_level_unlock_function.find(pqs_unlock->system_id());
		if (it != m_level_unlock_function.end())
		{
			// level unlock
			if (player_level >= it->second)
			{
				// unlock
				pqs_unlock->set_unlock(1);
			}
			else
			{
				pqs_unlock->set_unlock(0);
			}

			pqs_unlock->set_unlock_level(it->second);
		}
		else
		{
			pqs_unlock->set_unlock_level(0);
		}

		return 0;
	}
	else
	{
		GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

		player_level = player->level();

		LevelUnlockFun_t::iterator it = m_level_unlock_function.find(pqs_unlock->system_id());
		if (it != m_level_unlock_function.end())
		{
			// level unlock
			if (player_level >= it->second)
			{
				// unlock
				pqs_unlock->set_unlock(1);
			}
			else
			{
				pqs_unlock->set_unlock(0);
			}

			pqs_unlock->set_unlock_level(it->second);
			return 0;
		}
		else
		{
			// task, stage unlock
			GET_STAGE_FROM_POOL(m_plugin_cfg.pool, player->stage(), stage);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(stage, SE_NGUID_CODE);

			for (int i = 0; i < stage->stage_unlock_function_size(); ++i)
			{
				if (stage->stage_unlock_function(i) == pqs_unlock->system_id())
				{
					// ok, unlock
					pqs_unlock->set_unlock(1);
					return 0;
				}
			}

			for (int i = 0; i < stage->task_unlock_function_size(); ++i)
			{
				if (stage->task_unlock_function(i) == pqs_unlock->system_id())
				{
					// ok, unlock
					pqs_unlock->set_unlock(1);
					return 0;
				}
			}
		}

		// ok, no unlock
		pqs_unlock->set_unlock(0);
		return 0;
	}

}

int ManagePlayer::pmsg_update_vipexp(const PackInfo & pack_info)
{
	typed::protocol::pmsg_update_vipexp * pu_vipexp = (typed::protocol::pmsg_update_vipexp *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pu_vipexp, SE_EPLUGIN_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	updateVipExp(player, pu_vipexp->increase_vip_exp(), pu_vipexp->source_from());

	return 0;
}

int ManagePlayer::gmcmd_levelup(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() < 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	int up_level = atoi(gm_param[0].c_str());
	if (up_level <= 0)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	uint64 role_guid = player->roles(0);
	if (gm_param.size() >= 2)
	{
		int role_index = atoi(gm_param[1].c_str());
		if ((role_index > 0) && (role_index <= player->roles_size()))
		{
			role_guid = player->roles(role_index - 1);
		}
	}

	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, role_guid, role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, SE_NGUID_CODE);

	int level_cap = std::min((int32)player->level_limit(), m_system_res_info->level_cap);

	if (role->level() >= level_cap)
	{
		return 0;
	}

	if (role->level() + up_level > level_cap)
	{
		up_level = level_cap - role->level();
	}

	int pre_level = role->level();
	role->set_level(role->level() + up_level);
	levelUpgrade(role, role->level(), role->level() + up_level);
	POOL_OP_UPDATE(m_plugin_cfg.pool, role->guid());

	m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 211, 212, player->guid(), player->name(), player->level(), 
		role->guid(), pre_level, role->level(), role->exp(), role->exp(), role->exp());

	notifyRoleLevelUp(player, role, pre_level);

	return 0;
}

int ManagePlayer::gmcmd_addexp(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	int add_exp = atoi(gm_param[0].c_str());
	if (add_exp <= 0)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(0), role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, SE_NGUID_CODE);

	updateRoleExp(player->roles(0), add_exp, player->addiction_status(), 0, 202);

	POOL_OP_UPDATE(m_plugin_cfg.pool, role->guid());

	//notifyRoleLevelUp(player, role);

	return 0;
}

int ManagePlayer::gmcmd_addmoney(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	GET_COIN_FROM_POOL(m_plugin_cfg.pool, player->coin_guid(), coin);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, SE_NGUID_CODE);

	int add_money = atoi(gm_param[0].c_str());
	int money_amount = player->money() + add_money;
	if (money_amount < 0)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	updateMoney(player, coin, add_money, 223);

	notifyPlayerUpdateMoney(player);

	return 0;
}

int ManagePlayer::gmcmd_addcoin(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	GET_COIN_FROM_POOL(m_plugin_cfg.pool, player->coin_guid(), coin);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, SE_NGUID_CODE);

	int add_coin = atoi(gm_param[0].c_str());
	int coin_amount = player->coin() + add_coin;
	if (coin_amount < 0)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	updateCoin(player, coin, add_coin, 263, false);

	notifyPlayerUpdateMoney(player);

	return 0;
}

int ManagePlayer::gmcmd_addseni(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	int add_seni = atoi(gm_param[0].c_str());
	int seni_amount = player->seniority() + add_seni;
	if (seni_amount < 0)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	updateSeniority(player, add_seni, 233);

	notifyPlayerUpdateMoney(player);

	return 0;
}

int ManagePlayer::gmcmd_jumpmap(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	int map_id = atoi(gm_param[0].c_str());
	if (map_id < 0)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	typed::protocol::pmsg_player_jump_map * player_jump_map = new typed::protocol::pmsg_player_jump_map();
	player_jump_map->set_target_map(map_id);
	PackInfo pi(PMSG_PLAYER_JUMP_MAP, player->guid(), player_jump_map);
	if (!REQUEST_MSG_TO_PLUGINS(m_plugin_cfg.message, pi))
	{
		return SE_GMCMD_PARAM_CODE;
	}

	return 0;
}

int ManagePlayer::gmcmd_modifyattr(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 2)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool,target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	uint64 role_guid = player->roles(0);
	if (gm_param.size() >= 3)
	{
		int role_index = atoi(gm_param[2].c_str());
		if ((role_index > 0) && (role_index <= player->roles_size()))
		{
			role_guid = player->roles(role_index - 1);
		}
	}

	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, role_guid, role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, SE_NGUID_CODE);

	int attr_index = atoi(gm_param[0].c_str());
	int attr_value = atoi(gm_param[1].c_str());

	//if ((attr_index <= RAT_BEGIN) || (attr_index >= RAT_ENDING))
	//{
	//	return SE_GMCMD_PARAM_CODE;
	//}

	m_plugin_cfg.scene->modifyRoleAttrBySchemeId(role, attr_index, attr_value, true);

	return 0;
}

int ManagePlayer::gmcmd_addenergy(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool,target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	int energy = atoi(gm_param[0].c_str());

	if (energy < 0)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	typed::protocol::pmsg_add_energy * p_add_energy = new typed::protocol::pmsg_add_energy();
	p_add_energy->set_energy(energy);
	p_add_energy->set_can_exceed_limit(1);
	p_add_energy->set_source_from(253);

	PackInfo pi(PMSG_ADD_ENERGY, player->guid(), p_add_energy);
	REQUEST_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);

	//player->set_energy(energy);

	//notifyPlayerUpdateEnergy(player);

	return 0;
}

int ManagePlayer::gmcmd_addpres(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool,target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	int prestige = atoi(gm_param[0].c_str());
	typed::protocol::pmsg_update_prestige * update_pres = new typed::protocol::pmsg_update_prestige();
	update_pres->set_value(prestige);
	update_pres->set_source_from(242);
	PackInfo pi(PMSG_UPDATE_PRESTIGE, target_guid, update_pres);
	REQUEST_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);

	return 0;
}

int ManagePlayer::gmcmd_addrever(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool,target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	int reverence = atoi(gm_param[0].c_str());

	typed::protocol::pmsg_update_reverence * pu_reverence = new typed::protocol::pmsg_update_reverence();
	pu_reverence->set_value(reverence);
	PackInfo pi(PMSG_UPDATE_REVERENCE, target_guid, pu_reverence);
	REQUEST_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);

	return 0;
}

int ManagePlayer::gmcmd_addvipexp(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	int add_exp = atoi(gm_param[0].c_str());
	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(add_exp > 0, SE_GMCMD_PARAM_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	//GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(0), role);

	updateVipExp(player, add_exp, 272);

	return 0;
}

int ManagePlayer::gmcmd_setyellowlevel(const vector<string> & gm_param, uint64 target_guid)
{
	if (gm_param.size() != 1)
	{
		return SE_GMCMD_PARAM_CODE;
	}

	int yellow_level = atoi(gm_param[0].c_str());
	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(yellow_level > 0, SE_GMCMD_PARAM_CODE);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, target_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	if (yellow_level == 0)
	{
		player->set_is_yellow_vip(0);
		player->set_yellow_vip_level(0);
	}
	else
	{
		player->set_is_yellow_vip(1);
		player->set_yellow_vip_level(yellow_level);
	}
	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	m_plugin_cfg.manage_grid->updateYellowVipLevel(player);

	return 0;
}

void ManagePlayer::initPlayer(uint64 player_guid)
{
	if (m_player_assistant.initFirstEnterPlayer(player_guid))
	{
		playerDataLoadFinish(player_guid, true);
	}
	else
	{
		// error, should never happen
	}
}

void ManagePlayer::initRole(typed::entity::Role * role, RoleResInfo * role_res_info)
{
	// update attributes
	role->mutable_attributes()->Clear();
	for (int attr_i = RAT_BEGIN; attr_i < RAT_ENDING; ++attr_i)
	{
		role->add_attributes(0);
	}

	//m_plugin_cfg.scene->modifyRoleAttr(role, RAT_HP, role_res_info->hp + role_res_info->hp_growth * (role->level() - 1), false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_HP_BASE, role_res_info->hp + role_res_info->hp_growth * (role->level() - 1), false);

	//增加攻击防御等级成长
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_STR_ATT_BASE, role_res_info->phy_att_growth * (role->level() - 1), false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_STR_DEF_BASE, role_res_info->phy_def_growth * (role->level() - 1), false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_AGI_ATT_BASE, role_res_info->agi_att_growth * (role->level() - 1), false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_AGI_DEF_BASE, role_res_info->agi_def_growth * (role->level() - 1), false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_INT_ATT_BASE, role_res_info->int_att_growth * (role->level() - 1), false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_INT_DEF_BASE, role_res_info->int_def_growth * (role->level() - 1), false);

	//m_plugin_cfg.scene->modifyRoleAttr(role, RAT_CURR_HP, role->attributes(RAT_HP), false);

	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_STR, role_res_info->str, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_AGI, role_res_info->agi, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_INT, role_res_info->int_value, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_STR_ATT_BASE, role_res_info->phy_att, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_AGI_ATT_BASE, role_res_info->agi_att, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_INT_ATT_BASE, role_res_info->int_att, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_STR_DEF_BASE, role_res_info->phy_def, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_AGI_DEF_BASE, role_res_info->agi_def, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_INT_DEF_BASE, role_res_info->int_def, false);

	//伙伴自带百分比增加属性，目前只有血量和三攻三防
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_HP_PERCENT, role_res_info->hp_percent, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_STR_ATT_PERCENT, role_res_info->phy_att_percent, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_STR_DEF_PERCENT, role_res_info->phy_def_percent, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_AGI_ATT_PERCENT, role_res_info->agi_att_percent, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_AGI_DEF_PERCENT, role_res_info->agi_def_percent, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_INT_ATT_PERCENT, role_res_info->int_att_percent, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_INT_DEF_PERCENT, role_res_info->int_def_percent, false);

	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_CRITICAL, role_res_info->critical, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_CRITICAL_MULTIPLE, role_res_info->critical_damage, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_BLOCK, role_res_info->block, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_BLOCK_DEFENCE_MULTIPLE, role_res_info->block_defence, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_BLOCK_DAMAGE_MULTIPLE, role_res_info->block_damage, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_DODGE, role_res_info->dodge, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_HIT, role_res_info->hit, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_STRIKE_BACK, role_res_info->strike_back, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_ENRAGE, role_res_info->enarge, false);
	//MODIFY_ROLE_ATTR(m_plugin_cfg.scene->set_attributes(role->set_attributes(RAT_FORCE_VALUE->set_attributes(role_res_info->);
	//MODIFY_ROLE_ATTR(m_plugin_cfg.scene->set_attributes(role->set_attributes(RAT_ENRAGE->set_attributes(role_res_info->);
	//MODIFY_ROLE_ATTR(m_plugin_cfg.scene->set_attributes(role->set_attributes(RAT_STRIKE_BACK->set_attributes(role_res_info->);

	//
	int str_value = role->attributes(RAT_STR);

	//int str_att_base = role->attributes(RAT_STR_ATT_BASE) + str_value * m_war_init_res_info->str_att / TEN_THOUSAND_PERCENT_BASE;
	//int str_att_percent = role->attributes(RAT_STR_ATT_PERCENT);
	//m_plugin_cfg.scene->modifyRoleAttr(role, RAT_STR_ATT_BASE, m_war_init_res_info->str_att, false);
	//role->set_attributes(RAT_STR_ATTACK,str_att_base * (1 + str_att_percent / TEN_THOUSAND_PERCENT_BASE));

	//int str_def_base = role->attributes(RAT_STR_DEF_BASE) + str_value * m_war_init_res_info->str_def / TEN_THOUSAND_PERCENT_BASE;
	//int str_def_percent = role->attributes(RAT_STR_DEF_PERCENT);
	//m_plugin_cfg.scene->modifyRoleAttr(role, RAT_STR_DEF_BASE, m_war_init_res_info->str_def, false);
	//role->set_attributes(RAT_STR_DEFENSE, str_def_base * (1 + str_def_percent / TEN_THOUSAND_PERCENT_BASE));


	//int agi_value = role->attributes(RAT_AGI);
	//int agi_att_base = role->attributes(RAT_AGI_ATT_BASE) + agi_value * m_war_init_res_info->agi_att / TEN_THOUSAND_PERCENT_BASE;
	//int agi_att_percent = role->attributes(RAT_AGI_ATT_PERCENT);
	//m_plugin_cfg.scene->modifyRoleAttr(role, RAT_AGI_ATT_BASE, m_war_init_res_info->agi_att, false);
	//role->set_attributes(RAT_AGI_ATTACK, agi_att_base * (1 + agi_att_percent / TEN_THOUSAND_PERCENT_BASE));

	//int agi_def_base = role->attributes(RAT_AGI_DEF_BASE) + agi_value * m_war_init_res_info->agi_def / TEN_THOUSAND_PERCENT_BASE;
	//int agi_def_percent = role->attributes(RAT_AGI_DEF_PERCENT);
	//m_plugin_cfg.scene->modifyRoleAttr(role, RAT_AGI_DEF_BASE, m_war_init_res_info->agi_def, false);
	//role->set_attributes(RAT_AGI_DEFENSE, agi_def_base * (1 + agi_def_percent / TEN_THOUSAND_PERCENT_BASE));


	//int int_value = role->attributes(RAT_INT);
	//int int_att_base = role->attributes(RAT_INT_ATT_BASE) + int_value * m_war_init_res_info->int_att / TEN_THOUSAND_PERCENT_BASE;
	//int int_att_percent = role->attributes(RAT_INT_ATT_PERCENT);
	//m_plugin_cfg.scene->modifyRoleAttr(role, RAT_INT_ATT_BASE, m_war_init_res_info->int_att, false);
	//role->set_attributes(RAT_INT_ATTACK, int_att_base * (1 + int_att_percent / TEN_THOUSAND_PERCENT_BASE));

	//int int_def_base = role->attributes(RAT_INT_DEF_BASE) + int_value * m_war_init_res_info->int_def / TEN_THOUSAND_PERCENT_BASE;
	//int int_def_percent = role->attributes(RAT_INT_DEF_PERCENT);
	//m_plugin_cfg.scene->modifyRoleAttr(role, RAT_INT_DEF_BASE, m_war_init_res_info->int_def, false);
	//role->set_attributes(RAT_INT_DEFENSE, int_def_base * (1 + int_def_percent / TEN_THOUSAND_PERCENT_BASE));

	//int fight_power = m_plugin_cfg.scene->calcRoleFightPower(role);
	//role->set_attributes(RAT_FIGHT_POWER, fight_power);

	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_MULTI_HIT, role_res_info->combo, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_UNION_HIT, role_res_info->joint, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_MULTI_HIT_ADDI, role_res_info->combo_up, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_UNION_HIT_ADDI, role_res_info->joint_up, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_MULTI_HIT_REDUCE, role_res_info->combo_down, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_UNION_HIT_REDUCE, role_res_info->joint_down, false);
	m_plugin_cfg.scene->modifyRoleAttr(role, RAT_INITIATIVE_LEVEL, role->level() * m_system_res_info->level_first_strike, false);


	// init char ability
	uint32 char_ability = role->char_ability();
	role->set_char_ability(0);
	attachRoleCharAbility(role, char_ability);

	// law init
	
	// sect init

	// equipment init

}

void ManagePlayer::playerDataLoadFinish(uint64 player_guid, bool is_success)
{
	is_success ? ++m_player_info.load_player_success_no : ++m_player_info.load_player_failed_no;

	// player offline
	PlayerOnlineStateMap_t::iterator it_ps = m_player_state.find(player_guid);
	if ((it_ps != m_player_state.end()) && (POS_OFFLINE == it_ps->second.player_state))
	{
		//m_player_assistant.request_flush_player(player_guid, false);
		m_need_flush_player_map[player_guid] = false;
		return ;
	}

	PLAYER_LOG_INFO("player data is loading finish, player guid is <%llu>, load result is <%d>", player_guid, is_success);

	if (is_success)
	{
		GET_ENTITY_FROM_POOL(m_plugin_cfg.pool, typed::entity::Player, player_guid, player);
		if (NULL != player)
		{
			if (player->coin_guid() != 0)
			{
				GET_COIN_FROM_POOL(m_plugin_cfg.pool, player->coin_guid(), coin);
				PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, );
				if (player->coin() > coin->coin())
				{
					// cost
					player->set_total_coin_cost(player->total_coin_cost() + (player->coin() - coin->coin()));
				}
				if (player->money() > coin->money())
				{
					player->set_total_money_cost(player->total_money_cost() + (player->money() - coin->money()));
				}
				player->set_coin(coin->coin());
				player->set_money(coin->money());
			}

			m_plugin_cfg.scene->collectPlayerLevel(player->guid(), player->level());
			// online, clear the flower rank, ls will update it
			player->set_flower_rank(0);

			//m_plugin_cfg.line_scene->addPlayerShortInfo(player->guid(), new typed::entity::Short(), 10001, true);
			m_plugin_cfg.pool->playerIsOnline(player->guid());

			it_ps->second.player_state = POS_ONLINE;
			bool is_first_online = false;
			if (player->announce() == 0)
			{
				is_first_online = true;
			}

			if (is_first_online)
			{
				firstInitPlayer(player);
			}

			player->set_map_id(m_plugin_cfg.template_id);

			initPlayerAttribute(player);

			PLAYER_LOG_DEBUG("GOLDCARD_TEST:%llu", player->goldcard());

			if (is_first_online)
			{
				firstInitPlayerLevel(player);
				notifyPlayerIsFirstOnlineToLine(player);
				notifyPlayerIsFirstOnline(player->guid());
				POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());
			}

			//PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(player_guid);
			//if (NULL != player_bi)
			//{
			//	player_bi->scene_id = m_plugin_cfg.template_id;
			//}

			notifyPlayerIsOnline(player);

			checkPlayerCleanDay(player);

			checkPlayerCleanWeek(player);

			//if (!is_first_online)
			//{
			//	m_plugin_cfg.line_scene->playerLogin(player->guid(), player->map_id(), player->level());
			//}

			m_plugin_cfg.scene->playerOnline(player->guid());

			dispatchPlayerData(player);

			calcOfflineExp(player);

			if (is_first_online)
			{
				chargePreChargeInfo(player);
			}

			//
			MAKE_AND_SEND_PLAYER_MESSAGE(STOG_QUERY_QQ_VIP_INFO, player->guid(), "");

			MANAGE_GRID_ADD(m_plugin_cfg.manage_grid, player);
		}
		else
		{
			PLAYER_LOG_ERROR("player loading finish, but can not get the player from pool, guid is <%llu>", player_guid);
		}
	}
	else
	{
		// failed to login in game server, should never happen
		PLAYER_LOG_ERROR("failed to load player <%llu>", player_guid);
		NeedFlushPlayerMap_t::iterator nfp_it = m_need_flush_player_map.find(player_guid);
		if (nfp_it != m_need_flush_player_map.end())
		{
			m_need_flush_player_map.erase(nfp_it);
		}
		m_player_state.erase(it_ps);
	}
}

void ManagePlayer::playerFlushDataFinish(uint64 player_guid, bool is_success, RequestList * request_list)
{
	PLAYER_LOG_INFO("player data is flushing finish, player guid is <%llu>, flush result is <%d>", player_guid, is_success);

	if (is_success)
	{
		++m_player_info.flush_player_success_no;
	}
	else
	{
		++m_player_info.flush_player_failed_no;
	}

	if (m_lost_gate_connection)
	{
		playerFlushDataFinishAfterLostGateConnection(player_guid, is_success, request_list);
		return ;
	}

	PlayerOnlineStateMap_t::iterator it = m_player_state.find(player_guid);
	if (it != m_player_state.end())
	{
		m_plugin_cfg.pool->clearFlushState(player_guid);

		GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, player_guid, player);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(player, );

		if (POS_FLUSHING_LEAVE_MAP_WITH_JUMP == it->second.player_state)
		{
			//if (NULL != player)
			//{
			//	m_plugin_cfg.pool->remove(player->attach(), false, false);
			//}

			// remove data from pool
			for (RequestList::RequestVec_t::const_iterator req_it = request_list->getRequestList().begin(); req_it !=request_list->getRequestList().end(); ++req_it)
			{
				Request * req = *req_it;
				//POOL_OP_REMOVE(m_plugin_cfg.pool, req->guid, false);
				m_plugin_cfg.pool->remove(req->guid, false, false);
			}

			// flush finish, start to transfer player data to the line scene
			typed::entity::Player * player = (typed::entity::Player *)it->second.transfer_info->player;
			PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

			// notify client to jump map
			typed::protocol::smsg_player_jump_map s_jump_map;
			
			MapResInfoMap_t::iterator map_it = m_map_info_map->find(it->second.transfer_info->target_map_id);
			if (map_it != m_map_info_map->end())
			{
				MapResInfo * map_info = map_it->second;
				s_jump_map.set_src_x(map_info->born_x);
				s_jump_map.set_src_y(map_info->born_y);

			}

			s_jump_map.set_target_map_id(it->second.transfer_info->target_map_id);
			s_jump_map.set_line_no(m_plugin_cfg.line_no);
			//s_jump_map.set_map_type(1);
			MAKE_NEW_PACKET(ps, SMSG_PLAYER_JUMP_MAP, player->guid(), s_jump_map.SerializeAsString());
			SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);

			transferPlayerToLineScene(it->second.transfer_info);
			it->second.transfer_info = NULL;

			m_player_state.erase(it);

		}
		else //if (POS_ONLINE != it->second.player_state)
		{
			if (NULL != player)
			{
				deliveryCoinToLS(player);

				POOL_OP_REMOVE(m_plugin_cfg.pool, player->coin_guid(), false);
				POOL_OP_REMOVE(m_plugin_cfg.pool, player->attach(), false);
			}

			for (RequestList::RequestVec_t::const_iterator req_it = request_list->getRequestList().begin(); req_it !=request_list->getRequestList().end(); ++req_it)
			{
				Request * req = *req_it;
				POOL_OP_REMOVE(m_plugin_cfg.pool, req->guid, false);
			}

			Uint64Set_t::iterator it_sub = m_player_online_before_finish_flushing.find(player_guid);

			if (it_sub != m_player_online_before_finish_flushing.end())
			{
				++m_player_info.load_player_request_no;

				m_player_assistant.request_load_player(player_guid);

				it->second = PlayerAttachInfo(POS_LOADING_ENTER_MAP);

				m_player_online_before_finish_flushing.erase(it_sub);
			}
			else
			{
				m_player_state.erase(it);
			}

			typed::protocol::cmsg_player_leave_map leave_map;
			//leave_world.set_leave_type(typed::protocol::cmsg_player_leave_world_leave_t_finish_game);
			leave_map.set_leave_type(1);
			MAKE_NEW_PACKET(ps, SMSG_PLAYER_LEAVE_MAP, player_guid, leave_map.SerializeAsString());
			SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);
		}
		//else
		//{
		//	// player online again
		//}
	}
	else
	{
		// error
	}
	
	//m_player_flushing_set.erase(player_guid);
	if (is_success)
	{
	}
	else
	{
		// fatal error
	}
}

void ManagePlayer::update()
{
	m_update_call_number = ++m_update_call_number % 60;
	if (0 == m_update_call_number)
	{
		PLAYER_LOG_INFO("line <%d>, map <%d>, enter world: <%u>, leave world: <%u>, request load: <%u>, load success: <%u>, load failed: <%u>, request flush: <%u>, flush success: <%u>, flush failed: <%u>", m_plugin_cfg.line_no, m_plugin_cfg.template_id, m_player_info.enter_world_no, m_player_info.leave_world_no, 
			m_player_info.load_player_request_no, m_player_info.load_player_success_no, m_player_info.load_player_failed_no, m_player_info.flush_player_request_no, m_player_info.flush_player_success_no, m_player_info.flush_player_failed_no);
	}

	checkEnergyReward();

	//if (IS_LINE_SCENE())
	//{
	//	uint64 curr_day = getCurrentDay();
	//	if (curr_day > m_current_day)
	//	{
	//		m_current_day = curr_day;
	//		notifyDayChange();
	//	}
	//}
}

void ManagePlayer::updatePool()
{
	processNeedFlushPlayer();

	m_plugin_cfg.pool->update();
}

int ManagePlayer::initing()
{
	m_map_info_map = (MapResInfoMap_t *)&m_plugin_cfg.scene->getManageResource()->getMapResInfo();

	m_role_res_info_map = (RoleResInfoMap_t *)&m_plugin_cfg.scene->getManageResource()->getRoleResInfo();

	m_war_init_res_info = (WarInitResInfo *)&m_plugin_cfg.scene->getManageResource()->getWarInitResInfo();

	m_level_exp_res_info = (LevelExpResInfoMap_t *)&m_plugin_cfg.scene->getManageResource()->getLevelExpResInfo();
	
	m_system_res_info = (SystemResInfo *)&m_plugin_cfg.scene->getManageResource()->getSystemResInfo();

	m_intialization_res_info = (InitializationCfg *)&m_plugin_cfg.scene->getManageResource()->getInitializationCfg();

	m_update_timer_id = m_plugin_cfg.scene->ITimer_scheme(UPDATE_CHECK_TIME, boost::bind(&ManagePlayer::update, this));

	m_update_pool_timer_id = m_plugin_cfg.scene->ITimer_scheme(UPDATE_POOL_TIME, boost::bind(&ManagePlayer::updatePool, this));

	m_offline_hangup_res = (OfflinehangupInfo *)&m_plugin_cfg.scene->getManageResource()->getOfflinehangupInfo();

	m_vipexp_res_info_map = (VipexpResInfoMap_t *)&m_plugin_cfg.scene->getManageResource()->getVipexpResInfoMap();
	m_player_assistant.init(m_plugin_cfg);

	m_doubleexp_res_map = (DoubleexpResMap_t *)&m_plugin_cfg.scene->getManageResource()->getDoubleexpResMap();

	m_current_day = getCurrentDay();

	m_current_week = getCurrentWeek();

	m_system_unlock_res = (SystemUnlockRes *)&m_plugin_cfg.scene->getManageResource()->getSystemUnlockInfoMap();
	//m_inherite_info_res = (InheritInfoRes *)&m_plugin_cfg.scene->getManageResource()->getInheritInfoRes();
	m_gold_param_info_res_map = (GoldParamResInfoMap_t *)&m_plugin_cfg.scene->getManageResource()->getGoldParamResInfoMap();

	{
		GoldParamResInfoMap_t::iterator it = m_gold_param_info_res_map->find(192);
		PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(it != m_gold_param_info_res_map->end(), SE_NRES_CODE);
		m_inherite_gold_res = it->second;
	}

	VipInfoMap_t * vip_info_map = (VipInfoMap_t *)&m_plugin_cfg.scene->getManageResource()->getVipInfoMap();
	VipInfoMap_t::iterator it = vip_info_map->find(134);
	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(vip_info_map->end() != it, SE_NRES_CODE);

	m_vip_role_follow = it->second;

	//PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(resetInheritCoinInfo(), SE_NRES_CODE);

	for (UnlockFunctionMap_t::iterator it = m_system_unlock_res->level_unlock_map.begin(); it != m_system_unlock_res->level_unlock_map.end(); ++it)
	{
		for (vector<uint32>::iterator sub_it = it->second.begin(); sub_it != it->second.end(); ++sub_it)
		{
			m_level_unlock_function[*sub_it] = it->first;
		}
	}

	vector<std::pair<int, int> > map_vec;
	getMapIdVec(map_vec);

	// offline exp unlock level
	LevelUnlockFun_t::iterator vip_it = m_level_unlock_function.find(34);
	if (vip_it != m_level_unlock_function.end())
	{
		m_offline_hangup_exp_unlock_level = vip_it->second;
	}
	else
	{
		PLAYER_LOG_ERROR("failed to find offline exp unlock level, function id is 34\n");
		return -1;
	}

	initProvideEnergyInfo();

	if (!loadPreChargeInfo())
	{
		PLAYER_LOG_ERROR("failed to load pre charge info");
		return -1;
	}

//	m_plugin_cfg.manage_grid->init(map_vec);
	return 0;
}

void ManagePlayer::getMapIdVec(vector<std::pair<int, int> > & map_vec)
{
	MapResInfoMap_t::iterator it = m_map_info_map->find(m_plugin_cfg.template_id);
	if (it != m_map_info_map->end())
	{
		map_vec.push_back(std::make_pair(it->first, it->second->type));
	}
}

bool ManagePlayer::reach_map_condition(int map_id, typed::entity::Player * player, MapResInfo * * map_res_info)
{
	GET_STAGE_FROM_POOL(m_plugin_cfg.pool, player->stage(), stage);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(stage, SE_NGUID_CODE);

	bool is_unlock = false;
	for (int i = 0; i < stage->unlock_town_id_size(); ++i)
	{
		if (stage->unlock_town_id(i) == map_id)
		{
			is_unlock = true;
			break;
		}
	}

//	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(is_unlock, false);

	*map_res_info = NULL;

	bool result = false;
	MapResInfoMap_t::iterator it = m_map_info_map->find(map_id);
	if (it != m_map_info_map->end())
	{
		*map_res_info = it->second;

		MapResInfo * map_info = it->second;
		if (player->level() >= map_info->enter_level)
		{
			// todo check task need
			if ((8 == map_info->type) || (100 == map_info->type))
			{
				PLAYER_LOG_DEBUG("player enter the map of type 8, player is <%llu>, map id is <%d>", player->guid(), map_id);
				result = true;
			}
			else if (is_unlock)
			{
				result = true;
			}
		}
		else
		{
			PLAYER_LOG_ERROR("failed to check level , target map enter level is <%d>", map_info->enter_level);
		}
	}
	else
	{
		PLAYER_LOG_ERROR("failed to get map by map id : <%d>", map_id);
	}

	if (!result)
	{
		PLAYER_LOG_ERROR("failed to jump map, map id is <%d>, player is <%llu>, is_unlock is <%d>", map_id, player->guid(), is_unlock ? 1 : 0);
	}

	return result;
}

void ManagePlayer::send_create_self_msg(typed::entity::Player * player)
{
	if (NULL == player)
	{
		// error, should never happen
		return ;
	}

	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(0), role);
	if (NULL == role)
	{
		return ;
	}

	typed::protocol::smsg_create_self self_create;

	self_create.set_guid(player->guid());
	self_create.set_name(player->name());
	self_create.set_level(player->level());
	self_create.set_exp(player->exp());
	self_create.set_profession(player->profession());
	self_create.set_sex(player->sex());
	self_create.set_money(player->money());
	self_create.set_coin(player->coin());
	self_create.set_src_x(player->src_x());
	self_create.set_src_y(player->src_y());
	self_create.set_dst_x(player->dst_x());
	self_create.set_dst_y(player->dst_y());
	self_create.set_seniority(player->seniority());
	self_create.set_map_id(player->map_id());
	self_create.set_direction(player->direction());
	self_create.set_role_id(role->role_id());
	self_create.set_prestige(player->prestige());
	self_create.set_rank(player->rank());
	self_create.set_guild(player->guild());
	self_create.set_guild_name(player->guild_name());
	self_create.set_energy(player->energy());
	self_create.set_is_addiction(player->is_addiction());
	self_create.set_vip_level(player->vip_level());
	self_create.set_vip_exp(player->vip_exp());
	self_create.set_reverence(player->reverence());

	self_create.set_member_slot(player->member_slot());
	self_create.set_guild_icon(player->guild_icon());
	self_create.set_follow_role_id(player->follow_role_id());
	for (int i = 0; i < player->current_title_size(); ++i)
	{
		self_create.add_current_title(player->current_title(i));
	}

	for (int i = 0; i < player->roles_size(); ++i)
	{
		self_create.add_roles(player->roles(i));
	}

	for (int i = 0; i < player->bag_size(); ++i)
	{
		self_create.add_bag(player->bag(i));
	}

	for (int i = 0; i < player->storehouse_size(); ++i)
	{
		self_create.add_storehouse(player->storehouse(i));
	}
	self_create.set_special_right(player->special_right());
	self_create.set_horse_id(player->horse_id());
	self_create.set_wife_follow(player->wife_follow());
	self_create.set_yellow_vip_level(player->yellow_vip_level());
	self_create.set_is_yellow_vip(player->is_yellow_vip());
	self_create.set_is_yellow_year_vip(player->is_yelow_year_vip());
	self_create.set_is_yellow_high_vip(player->is_yellow_high_vip());

	MAKE_NEW_PACKET(packet, SMSG_CREATE_SELF, player->guid(), self_create.SerializeAsString());
	SEND_PLAYER_MESSAGE(m_plugin_cfg.message, packet);
}

void ManagePlayer::send_create_role_msg(typed::entity::Player * player)
{
	if (NULL == player)
	{
		// error, should never happen
		return ;
	}

	int hired_zslw = 2;

	for (int i = 0; i < player->roles_size(); ++i)
	{
		GET_ENTITY_FROM_POOL(m_plugin_cfg.pool, typed::entity::Role, player->roles(i), role);
		if (NULL == role)
		{
			// error, should never happen
			continue;
		}

		send_create_role_msg(player, role);
		if (role->role_id() == ZSLW_ROLE_ID)
		{
			hired_zslw = 1;
		}
	}

	if (2 == hired_zslw)
	{
		for (int i = 0; i < player->leave_team_roles_size(); ++i)
		{
			GET_ENTITY_FROM_POOL(m_plugin_cfg.pool, typed::entity::Role, player->leave_team_roles(i), role);
			if (NULL == role)
			{
				// error, should never happen
				continue;
			}

			if (role->role_id() == ZSLW_ROLE_ID)
			{
				hired_zslw = 1;
				break;
			}
		}
	}

	//for (int i = 0; i < player->leave_team_roles_size(); ++i)
	//{
	//	GET_ENTITY_FROM_POOL(m_plugin_cfg.pool, typed::entity::Role, player->leave_team_roles(i), role);
	//	if (NULL == role)
	//	{
	//		// error, should never happen
	//		continue;
	//	}

	//	send_create_role_msg(player, role);
	//}

	sendHireZsly(player, hired_zslw);
}

/* 客户端角色属性
1当前hp
2hp总值
3外功
4内功
5奇术
6外功攻击
7内功攻击
8奇术攻击
9外功防御
10内功防御
11奇术防御
12命中率
13闪避率
14格挡率
15暴击率
16暴击伤害倍数
17格挡伤害减免
18格挡伤害
19反击
20战斗力
21总先攻值
22装备幻化先攻值
23装备强化先攻值
24心法等级先攻值
25三围培养先攻值
26轻功等级先攻值
27脉轮等级先攻值
28武馆训练先攻值
29成员等级先攻值
*/

void ManagePlayer::send_create_role_msg(typed::entity::Player * player, typed::entity::Role * role)
{
	typed::protocol::smsg_create_role create_role;

	create_role.set_guid(role->guid());
	create_role.set_player(role->player());
	create_role.set_role_id(role->role_id());
	create_role.set_name(role->name());
	create_role.set_sex(role->sex());
	create_role.set_job(role->job());
	create_role.set_faction(role->faction());
	create_role.set_level(role->level());
	create_role.set_exp(role->exp());
	create_role.set_postion(role->postion());
	create_role.set_normal_skill(role->normal_skill());
	create_role.set_char_ability(role->char_ability());
	create_role.set_extremely_skill(role->extremely_skill());
	create_role.set_hire_id(role->hire_id());
	create_role.set_inherit_to(role->inherit_to());
	create_role.set_inherit_from(role->inherit_from());

	fillRoleAttr(create_role.mutable_attributes(), role, player);
	fillRoleAttrRank(create_role.mutable_attrs_rank(), role, player, m_system_res_info);
	//create_role.add_attributes(role->attributes(RAT_CURR_HP));
	//create_role.add_attributes(role->attributes(RAT_HP));
	//create_role.add_attributes(role->attributes(RAT_STR));
	//create_role.add_attributes(role->attributes(RAT_AGI));
	//create_role.add_attributes(role->attributes(RAT_INT));


	//create_role.mutable_attributes()->CopyFrom(role->attributes());

	//for (int i = 0; i < role->equipments_size(); ++i)
	//{
	//	create_role.add_equipment(role->equipments(i));
	//}

	//for (int i = 0; i < role->attributes_size(); ++i)
	//{
	//	create_role.add_attributes(role->attributes(i));
	//}

	MAKE_NEW_PACKET(packet, SMSG_CREATE_ROLE, player->guid(), create_role.SerializeAsString());
	SEND_PLAYER_MESSAGE(m_plugin_cfg.message, packet);
}

void ManagePlayer::fillRoleAttr(::google::protobuf::RepeatedField< ::google::protobuf::uint32 > * attributes, typed::entity::Role * role, typed::entity::Player * player)
{
	attributes->Add(role->attributes(RAT_CURR_HP));
	attributes->Add(role->attributes(RAT_HP));
	attributes->Add(role->attributes(RAT_STR));
	attributes->Add(role->attributes(RAT_AGI));
	attributes->Add(role->attributes(RAT_INT));
	attributes->Add(role->attributes(RAT_STR_ATTACK));
	attributes->Add(role->attributes(RAT_AGI_ATTACK));
	attributes->Add(role->attributes(RAT_INT_ATTACK));
	attributes->Add(role->attributes(RAT_STR_DEFENSE));
	attributes->Add(role->attributes(RAT_AGI_DEFENSE));
	attributes->Add(role->attributes(RAT_INT_DEFENSE));
	attributes->Add(role->attributes(RAT_CRITICAL));
	attributes->Add(role->attributes(RAT_CRITICAL_MULTIPLE));
	attributes->Add(role->attributes(RAT_BLOCK));
	attributes->Add(role->attributes(RAT_BLOCK_DEFENCE_MULTIPLE));
	attributes->Add(role->attributes(RAT_BLOCK_DAMAGE_MULTIPLE));
	attributes->Add(role->attributes(RAT_DODGE));
	attributes->Add(role->attributes(RAT_HIT));
	attributes->Add(role->attributes(RAT_STRIKE_BACK));
	attributes->Add(role->attributes(RAT_FIGHT_POWER));
	// for 先攻值
	attributes->Add(role->attributes(RAT_INITIATIVE_ALL));
	attributes->Add(role->attributes(RAT_INITIATIVE_COLOR));
	attributes->Add(role->attributes(RAT_INITIATIVE_STHEN));
	attributes->Add(role->attributes(RAT_INITIATIVE_LAW));
	attributes->Add(role->attributes(RAT_INITIATIVE_EDUCATE));
	attributes->Add(role->attributes(RAT_INITIATIVE_DODGE));
	attributes->Add(role->attributes(RAT_INITIATIVE_CHAKRE));
	attributes->Add(role->attributes(RAT_INITIATIVE_TRAINING));
	attributes->Add(role->attributes(RAT_INITIATIVE_LEVEL));
}

//void ManagePlayer::fillRoleAttrRank(::google::protobuf::RepeatedField< ::google::protobuf::uint32 > * attributes, typed::entity::Role * role, typed::entity::Player * player)
//{
//	attributes->Add(role->level());
//	attributes->Add(role->attributes(RAT_HP));
//	attributes->Add(role->attributes(RAT_STR_ATTACK));
//	attributes->Add(role->attributes(RAT_STR_DEFENSE));
//	attributes->Add(role->attributes(RAT_AGI_ATTACK));
//	attributes->Add(role->attributes(RAT_AGI_DEFENSE));
//	attributes->Add(role->attributes(RAT_INT_ATTACK));
//	attributes->Add(role->attributes(RAT_INT_DEFENSE));
//	attributes->Add(getAttrRank(role, RAT_HIT, RAT_HIT_RANK, player->level()));
//	attributes->Add(getAttrRank(role, RAT_DODGE, RAT_DODGE_RANK, player->level()));
//	attributes->Add(getAttrRank(role, RAT_CRITICAL, RAT_CRITICAL_RANK, player->level()));
//	attributes->Add(getAttrRank(role, RAT_BLOCK, RAT_BLOCK_RANK, player->level()));
//	attributes->Add(getAttrRank(role, RAT_MULTI_HIT, RAT_MULTI_HIT_RANK, player->level()));
//	attributes->Add(getAttrRank(role, RAT_UNION_HIT, RAT_UNION_HIT_RANK, player->level()));
//
//	attributes->Add(role->attributes(RAT_INITIATIVE_ALL));
//	attributes->Add(role->attributes(RAT_INITIATIVE_COLOR));
//	attributes->Add(role->attributes(RAT_INITIATIVE_STHEN));
//	attributes->Add(role->attributes(RAT_INITIATIVE_LAW));
//	attributes->Add(role->attributes(RAT_INITIATIVE_EDUCATE));
//	attributes->Add(role->attributes(RAT_INITIATIVE_DODGE));
//	attributes->Add(role->attributes(RAT_INITIATIVE_CHAKRE));
//	attributes->Add(role->attributes(RAT_INITIATIVE_TRAINING));
//	attributes->Add(role->attributes(RAT_INITIATIVE_LEVEL));
//
//}

//void ManagePlayer::send_create_form_msg(typed::entity::Player * player)
//{
//	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );
//
//	GET_FORM_FROM_POOL(m_plugin_cfg.pool, player->form(), form);
//	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(form, );
//
//	typed::protocol::smsg_create_player_form create_form;
//	create_form.set_guid(form->guid());
//	create_form.set_player(form->player());
//	create_form.set_curr_group(form->curr_form());
//	for (int i = 0; i < form->role_form_location_size(); ++i)
//	{
//		const typed::entity::Form_FormInfo & form_info = form->role_form_location(i);
//		typed::protocol::FormInfo * fi = create_form.add_role_form_location();
//		fi->set_id(form_info.id());
//		fi->set_group(form_info.group());
//		fi->set_level(form_info.level());
//		//fi->mutable_role_location()->CopyFrom(form_info.role_location());
//		for (int j = 0; j < form_info.role_location_size(); ++j)
//		{
//			if (form_info.role_location(j) == 1)
//			{
//				fi->add_role_location(0);
//			}
//			else
//			{
//				fi->add_role_location(form_info.role_location(j));
//			}
//		}
//	}
//
//	MAKE_NEW_PACKET(packet, SMSG_CREATE_PLAYER_FORM, player->guid(), create_form.SerializeAsString());
//	SEND_PLAYER_MESSAGE(m_plugin_cfg.message, packet);
//}

void ManagePlayer::send_create_item_msg(typed::entity::Player * player)
{
	send_create_item_msg(player, player->bag());
	send_create_item_msg(player, player->storehouse());
	for (int i = 0; i < player->roles_size(); ++i)
	{
		GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(i), role);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);

		send_create_item_msg(player, role->equipments());
	}

	for (int i = 0; i < player->leave_team_roles_size(); ++i)
	{
		GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->leave_team_roles(i), role);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);

		send_create_item_msg(player, role->equipments());
	}
}

void ManagePlayer::send_create_item_msg(typed::entity::Player * player, const ::google::protobuf::RepeatedField< ::google::protobuf::uint64 > & bag)
{
	typed::protocol::smsg_create_item create_item;
	for (::google::protobuf::RepeatedField< ::google::protobuf::uint64 >::const_iterator it = bag.begin(); it != bag.end(); ++it)
	{
		uint64 item_guid = *it;
		if (item_guid > 1)
		{
			GET_ITEM_FROM_POOL(m_plugin_cfg.pool, *it, item);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);

			fillClientItemContent(item, &create_item);
			MAKE_NEW_PACKET(ps, SMSG_CREATE_ITEM, player->guid(), create_item.SerializeAsString());
			SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);
		}
	}
}

void ManagePlayer::dispatchPlayerData(typed::entity::Player * player)
{
	send_create_self_msg(player);
	send_create_role_msg(player);
	//send_create_form_msg(player);
	send_create_item_msg(player);
	notifyPlayerIsOnlineAfterSync(player);
	PackInfo * pi = new PackInfo(LMSG_PLAYER_ONLINE_DISPATCH_MSG, player->guid(), NULL);
	m_plugin_cfg.line_scene->scenePackInput(pi);
}

void ManagePlayer::notifyPlayerIsFirstOnline(uint64 player)
{
	PackInfo pi(NMSG_PLAYER_IS_FIRST_ONLINE, player, NULL);
	NOTIFY_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);
}

void ManagePlayer::notifyPlayerIsFirstOnlineToLine(typed::entity::Player * player)
{
	typed::protocol::lmsg_player_first_online * lmsg_pfo = new typed::protocol::lmsg_player_first_online();
	
	lmsg_pfo->set_name(player->name());
	lmsg_pfo->set_level(player->level());
	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(0), role);
	CHECK_NULL_POINT_WITH_LOG_CMD(role, );
	if (NULL != role)
		lmsg_pfo->set_role_id(role->role_id());

	lmsg_pfo->set_sex(player->sex());
	lmsg_pfo->set_job(player->profession());
	lmsg_pfo->set_map_id(player->map_id());
	lmsg_pfo->set_role_id(player->role_id());
	lmsg_pfo->set_robot_id(player->robot_id());
	lmsg_pfo->set_open_id(player->open_id_qq());
	lmsg_pfo->set_open_id_referrer(player->open_id_referrer());
	PackInfo * pack_info = new PackInfo(LMSG_PLAYER_FIRST_ONLINE, player->guid(), lmsg_pfo, 1);
	m_plugin_cfg.line_scene->scenePackInput(pack_info);
}

void ManagePlayer::notifyPlayerIsOnline(typed::entity::Player * player)
{
	PackInfo pi(NMSG_PLAYER_IS_ONLINE, player->guid(), NULL);
	NOTIFY_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);

	//m_plugin_cfg.line_scene->playerLogin(player->guid(), player->map_id(), player->level());
	{
		typed::protocol::lmsg_player_first_online * lmsg_pfo = new typed::protocol::lmsg_player_first_online();

		lmsg_pfo->set_name(player->name());
		lmsg_pfo->set_level(player->level());
		GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(0), role);
		CHECK_NULL_POINT_WITH_LOG_CMD(role, );
		if (NULL != role)
			lmsg_pfo->set_role_id(role->role_id());

		lmsg_pfo->set_sex(player->sex());
		lmsg_pfo->set_job(player->profession());
		lmsg_pfo->set_map_id(player->map_id());

		PackInfo * pi = new PackInfo(LMSG_PLAYER_IS_ONLINE, player->guid(), lmsg_pfo, 1);
		m_plugin_cfg.line_scene->scenePackInput(pi);
	}
}

void ManagePlayer::notifyPlayerIsOffline(uint64 player)
{
	m_plugin_cfg.line_scene->playerLogoff(player);

	{
		//GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, player, player_entity);
		//PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player_entity, );

		PackInfo * pi = new PackInfo(LMSG_PLAYER_IS_OFFLINE, player, NULL);
		m_plugin_cfg.line_scene->scenePackInput(pi);
	}

	PackInfo pi(NMSG_PLAYER_IS_OFFLINE, player, NULL);
	NOTIFY_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);
}

void ManagePlayer::notifyPlayerIsOnlineAfterSync(typed::entity::Player * player)
{
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_PLAYER_ONLINE_DISPATCH_MSG, player->guid(), NULL);
}

void ManagePlayer::notifyPlayerJumpMap(uint64 player, uint32 previous_map_id, uint32 target_map_id)
{
	{
		PackInfo * pi = new PackInfo(LMSG_PLAYER_IS_JUMPMAP, player, NULL);
		m_plugin_cfg.line_scene->scenePackInput(pi);
	}

	PackInfo pi(NMSG_PLAYER_LEAVE_MAP, player, NULL);
	NOTIFY_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);
}

void ManagePlayer::initPlayerAttribute(typed::entity::Player * player)
{
	if (NULL == player)
	{
		return ;
	}

	player->clear_attributes();
	for (int i = PAT_BEGIN; i < PAT_ENDING; ++i)
	{
		player->add_attributes(0);
	}

	// 初始值
	for (int i = 0; i < player->roles_size(); ++i)
	{
		GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(i), role);
		if (NULL == role)
		{
			continue;
		}

		RoleResInfo * role_res_info = getRoleResInfo(role->role_id());
		if (NULL == role_res_info)
		{
			continue;
		}

		// temp code
		if (role->quality() == 0)
		{
			role->set_quality(role_res_info->color);
			POOL_OP_UPDATE(m_plugin_cfg.pool, role->guid());
		}

		uint32 curr_hp = GET_ROLE_ATTR(role, RAT_CURR_HP);
		initRole(role, role_res_info);
		// todo
		//role->set_attributes(RAT_CURR_HP, curr_hp);
	}

	for (int i = 0; i < player->leave_team_roles_size(); ++i)
	{
		GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->leave_team_roles(i), role);
		if (NULL == role)
		{
			continue;
		}

		RoleResInfo * role_res_info = getRoleResInfo(role->role_id());
		if (NULL == role_res_info)
		{
			continue;
		}

		uint32 curr_hp = GET_ROLE_ATTR(role, RAT_CURR_HP);
		initRole(role, role_res_info);
		// todo
		//role->set_attributes(RAT_CURR_HP, curr_hp);
	}
}

void ManagePlayer::firstInitPlayer(typed::entity::Player * player)
{
	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(0), role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, );

	ACE_Date_Time curr_time(ACE_OS::gettimeofday());
	player->set_last_clean_day_relational_time(getCurrentDay());

	player->set_energy(m_intialization_res_info->energy);
	player->set_money(m_intialization_res_info->money);
	player->set_coin(m_intialization_res_info->coin);
	player->set_seniority(m_intialization_res_info->frame);
	player->set_platform(m_plugin_cfg.server_cfg.platform);

	uint64 guid = 0;
	bool update_player_to_db = false;


	if (player->attach() == 0)
	{
		if (m_plugin_cfg.scene->get_guid(ET_ATTACH, guid))
		{
			player->set_attach(guid);
		}
	}

	if (player->announce() == 0)
	{
		if (m_plugin_cfg.scene->get_guid(ET_ANNOUNCE, guid))
		{
			typed::entity::Announce * announce = new typed::entity::Announce();
			announce->set_guid(guid);
			announce->set_player(player->guid());

			player->set_announce(guid);

			POOL_OP_ADD(m_plugin_cfg.pool, announce->guid(), announce, true, player->guid());
			update_player_to_db = true;
		}
	}
	init_robot_info(player);


}

void ManagePlayer::firstInitPlayerLevel(typed::entity::Player * player)
{
	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(0), role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, );

	// init player level
	if (m_intialization_res_info->level > 1)
	{
		levelUpgrade(role, role->level(), m_intialization_res_info->level);
		role->set_level(m_intialization_res_info->level);
		player->set_level(role->level());
	}

	if (m_intialization_res_info->exp > 0)
	{
		role->set_exp(m_intialization_res_info->exp);
	}

	// init player exp


	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	RoleResInfo * role_res = getRoleResInfo(role->role_id());
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role_res, );
	role->set_quality(role_res->color);
	POOL_OP_UPDATE(m_plugin_cfg.pool, role->guid());
}

void ManagePlayer::notifyPlayerUpdateMoney(typed::entity::Player * player)
{
	typed::protocol::smsg_money_update money_update;
	money_update.set_curr_seniority(player->seniority());
	money_update.set_curr_money(player->money());
	money_update.set_curr_coin(player->coin());
	MAKE_NEW_PACKET(ps, SMSG_MONEY_UPDATE, player->guid(), money_update.SerializeAsString());
	SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);
}

void ManagePlayer::notifyPlayerUpdateEnergy(typed::entity::Player * player)
{
	// todo
	typed::protocol::smsg_update_energy update_energy;
	update_energy.set_curr_energy(player->energy());

	MAKE_NEW_PACKET(ps, SMSG_UPDATE_ENERGY, player->guid(), update_energy.SerializeAsString());
	SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);
}

void ManagePlayer::notifyDayChange()
{
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_DAY_CHANGE_SCENE, 0, NULL);

	if (!IS_LINE_SCENE())
	{
		const PlayerGuidSet_t & online_player = m_plugin_cfg.pool->getOnlinePlayer();
		for (PlayerGuidSet_t::const_iterator it = online_player.begin(); it != online_player.end(); ++it)
		{
			GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, *it, player);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(player, continue);

			playerDayChange(player, 1);

			notifyClientDayChange(player);
		}
	}
}

void ManagePlayer::notifyWeekChange()
{
	if (!IS_LINE_SCENE())
	{
		MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_WEEK_CHANGE_SCENE, 0, NULL);

		const PlayerGuidSet_t & online_player = m_plugin_cfg.pool->getOnlinePlayer();
		for (PlayerGuidSet_t::const_iterator it = online_player.begin(); it != online_player.end(); ++it)
		{
			GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, *it, player);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(player, continue);

			typed::protocol::nmsg_week_change * nw_change = new typed::protocol::nmsg_week_change();
			nw_change->set_online_status(1);
			MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_WEEK_CHANGE, player->guid(), nw_change);
			player->set_last_clean_week_relational_time(m_current_week);
			POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

			typed::protocol::lmsg_week_change_for_player * lwc_for_player = new typed::protocol::lmsg_week_change_for_player();
			lwc_for_player->set_online_status(1);

			PackInfo * ls_pi = new PackInfo(LMSG_WEEK_CHANGE_FOR_PLAYER, *it, lwc_for_player);
			m_plugin_cfg.line_scene->scenePackInput(ls_pi);

		}
	}
}

RoleResInfo * ManagePlayer::getRoleResInfo(int role_id)
{
	RoleResInfoMap_t::iterator it = m_role_res_info_map->find(role_id);
	if (m_role_res_info_map->end() != it)
	{
		return it->second;
	}

	return NULL;
}

void ManagePlayer::updatePlayerExp(uint64 player_guid, int increase_value)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, player_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

	uint32 exp = player->exp() + increase_value;
	int update_level = player->level();

	calcUpdateLevel(player, exp, update_level);

	if (update_level > player->level())
	{
		// update level
		typed::protocol::smsg_player_upgrade_level upgrade_level;
		upgrade_level.set_upgrade_level(update_level);
		upgrade_level.set_upgrade_exp(exp);
//		GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(0), role);
//		upgrade_level.set_upgrade_hp(GET_ROLE_ATTR(role, RAT_HP));
		MAKE_NEW_PACKET(ps, SMSG_PLAYER_UPGRADE_LEVEL, player->guid(), upgrade_level.SerializeAsString());
		SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);
	}

	player->set_exp(exp);
	player->set_level(update_level);
}

void ManagePlayer::updateRoleExp(uint64 role_guid, int increase_value, int addiction_status, int index, uint32 source_from)
{
	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, role_guid, role);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role, );

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, role->player(), player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

	GET_ADDICTION_VALUE(increase_value, addiction_status);
	if (0 == increase_value)
	{
		return ;
	}

	int level_cap = std::min((int32)player->level_limit(), m_system_res_info->level_cap);

	if (role->level() >= level_cap)
	{
		// already top level
		return ;
	}

	uint32 old_role_exp = role->exp();

	uint32 exp = role->exp() + increase_value;
	int update_level = role->level();

	calcUpdateLevel(player, exp, update_level);

	uint32 role_curr_level = role->level();

	role->set_exp(exp);
	role->set_level(update_level);

	if (update_level > role_curr_level)
	{
		levelUpgrade(role, role_curr_level, update_level);
		notifyRoleLevelUp(player, role, role_curr_level);
		// level
		if (!IS_SOURCE_FROM_LS(source_from))
		{
			m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 211, source_from, player->guid(), player->name(), player->level(), 
				role->guid(), role_curr_level, role->level(), increase_value, old_role_exp, role->exp());
		}
	}
	else
	{
		notifyRoleUpdateExp(player, role);
	}

	if (player->roles(0) == role->guid())
	{
		typed::protocol::stol_player_get_exp * spg_exp = new typed::protocol::stol_player_get_exp();
		spg_exp->set_player_level(role->level());
		spg_exp->set_player_exp(role->exp());
		PackInfo * pi = new PackInfo(STOL_PLAYER_GET_EXP, player->guid(), spg_exp);
		m_plugin_cfg.line_scene->scenePackInput(pi);
	}

	// exp
	if (!IS_SOURCE_FROM_LS(source_from))
	{
		if (!IS_SOURCE_NOT_RECORD(source_from))
		{
			m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 201, source_from, player->guid(), player->name(), player->level(),
				role->guid(), increase_value, old_role_exp, role->exp());
		}
	}

	POOL_OP_UPDATE(m_plugin_cfg.pool, role->guid());
}

void ManagePlayer::updateSeniority(typed::entity::Player * player, int increase_value, uint32 source_from)
{
	if (increase_value > 0)
	{
		GET_ADDICTION_VALUE(increase_value, player->addiction_status());
		if (0 == increase_value)
		{
			return ;
		}
	}

	int64 seniority_v = player->seniority() + increase_value;

	if (seniority_v >= 0)
	{
		player->set_seniority((uint32)seniority_v);
	}

	if (increase_value > 0)
	{
		if (!IS_SOURCE_FROM_LS(source_from))
		{
			m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 231, source_from, player->guid(), player->name(), player->level(), 
				increase_value, player->seniority() - increase_value, player->seniority());
		}
	}
	else
	{
		if (!IS_SOURCE_FROM_LS(source_from))
		{
			m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 232, source_from, player->guid(), player->name(), player->level(), 
				0 - increase_value, player->seniority() - increase_value, player->seniority());
		}

		player->set_total_seni_cost(player->total_seni_cost() - increase_value);
	}

	typed::protocol::nmsg_seniority_increase * ns_increase = new typed::protocol::nmsg_seniority_increase();
	ns_increase->set_curr_seniority(player->seniority());

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_SENIORITY_INCREASE, player->guid(), ns_increase);
}

void ManagePlayer::updateMoney(typed::entity::Player * player, typed::entity::Coin * coin, int increase_value, uint32 source_from)
{
	if (increase_value > 0)
	{
		GET_ADDICTION_VALUE(increase_value, player->addiction_status());
		if (0 == increase_value)
		{
			return ;
		}
	}

	int64 money_v = player->money() + increase_value;
	if (money_v >= 0)
	{
		player->set_money(uint32(money_v));
	}

	if (increase_value > 0)
	{
		if (!IS_SOURCE_FROM_LS(source_from))
		{
			m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 221, source_from, player->guid(), player->name(), player->level(), 
				increase_value, player->money() - increase_value, player->money());
		}
	}
	else
	{
		m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 222, source_from, player->guid(), player->name(), player->level(), 
			0 - increase_value, player->money() - increase_value, player->money());

		player->set_total_money_cost(player->total_money_cost() - increase_value);
	}

	coin->set_money(player->money());
	POOL_OP_UPDATE(m_plugin_cfg.pool, coin->guid());

	typed::protocol::nmsg_money_increase * nm_increase = new typed::protocol::nmsg_money_increase();
	nm_increase->set_curr_money(player->money());

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_MONEY_INCREASE, player->guid(), nm_increase);
}

void ManagePlayer::updateCoin(typed::entity::Player * player, typed::entity::Coin * coin, int increase_value, uint32 source_from, bool is_charge)
{
	if ((!is_charge) && (increase_value > 0))
	{
		GET_ADDICTION_VALUE(increase_value, player->addiction_status());
		if (0 == increase_value)
		{
			return ;
		}
	}

	if (increase_value < 0)
	{
		// cost
		int cost_coin = 0 - increase_value;
		int total_coin = cost_coin;
		GET_VIP_FROM_POOL(m_plugin_cfg.pool, player->vip(), vip);
		if (NULL != vip)
		{
			vip->set_cost_coin(vip->cost_coin() + cost_coin);
			total_coin = vip->cost_coin();
		}

		typed::protocol::nmsg_coin_cost * nc_cost = new typed::protocol::nmsg_coin_cost();
		nc_cost->set_cost_value(cost_coin);
		nc_cost->set_total_cost_value(total_coin);
		MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_COIN_COST, player->guid(), nc_cost);
	}

	int64 coin_v = player->coin() + increase_value;
	if (coin_v >= 0)
	{
		player->set_coin(uint32(coin_v));
	}

	if (increase_value > 0)
	{
		if (!IS_SOURCE_FROM_LS(source_from))
		{
			m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 261, source_from, player->guid(), player->name(), player->level(), 
				increase_value, player->coin() - increase_value, player->coin());
		}
	}
	else
	{
		m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 262, source_from + 9000, player->guid(), player->name(), player->level(), 
			0 - increase_value, player->coin() - increase_value, player->coin());

		player->set_total_coin_cost(player->total_coin_cost() - increase_value);
	}

	coin->set_coin(player->coin());
	POOL_OP_UPDATE(m_plugin_cfg.pool, coin->guid());

	typed::protocol::nmsg_coin_increase * nc_increase = new typed::protocol::nmsg_coin_increase();
	nc_increase->set_curr_coin(player->coin());

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_COIN_INCREASE, player->guid(), nc_increase);
}

void ManagePlayer::updatePrestige(typed::entity::Player * player, int increase_value, uint32 source_from)
{
	if (increase_value > 0)
	{
		GET_ADDICTION_VALUE(increase_value, player->addiction_status());
		if (0 == increase_value)
		{
			return ;
		}
	}

	int64 prestige_v = player->prestige() + increase_value;
	if (prestige_v >= 0)
	{
		player->set_prestige(prestige_v);
	}

	if (!IS_SOURCE_FROM_LS(source_from))
	{
		m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 241, source_from, player->guid(), player->name(), player->level(), 
			increase_value, player->prestige() - increase_value, player->prestige());
	}

	typed::protocol::nmsg_prestige_increase * np_increase = new typed::protocol::nmsg_prestige_increase();
	np_increase->set_curr_prestige(prestige_v);

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_PRESTIGE_INCREASE, player->guid(), np_increase);
}

void ManagePlayer::calcUpdateLevel(typed::entity::Player * player, uint32 & exp, int & curr_level)
{
	GET_LEVEL_EXP_RES_INFO(m_level_exp_res_info, exp_info, curr_level);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(exp_info, );

	int level_cap = std::min((int32)player->level_limit(), m_system_res_info->level_cap);

	if (curr_level >= level_cap)
	{
		exp = 0;
		return ;
	}

	if (exp >= exp_info->exp)
	{
		curr_level += 1;
		exp -= exp_info->exp;
		calcUpdateLevel(player, exp, curr_level);
	}
}

void ManagePlayer::levelUpgrade(typed::entity::Role * role, int from_level, int update_level)
{
	RoleResInfo * role_res = getRoleResInfo(role->role_id());
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role_res, );

	int hp_upgrade = (update_level - from_level) * role_res->hp_growth;

	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_HP_BASE, hp_upgrade, true);

	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_STR_ATT_BASE, (update_level - from_level) * role_res->phy_att_growth, true);
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_STR_DEF_BASE, (update_level - from_level) * role_res->phy_def_growth, true);
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_AGI_ATT_BASE, (update_level - from_level) * role_res->agi_att_growth, true);
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_AGI_DEF_BASE, (update_level - from_level) * role_res->agi_def_growth, true);
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_INT_ATT_BASE, (update_level - from_level) * role_res->int_att_growth, true);
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_INT_DEF_BASE, (update_level - from_level) * role_res->int_def_growth, true);

	int initiative_value = (update_level - from_level) * m_system_res_info->level_first_strike;
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_INITIATIVE_LEVEL, initiative_value, true);
}

void ManagePlayer::levelReduce(typed::entity::Role * role, int from_level, int reduce_level)
{
	RoleResInfo * role_res = getRoleResInfo(role->role_id());
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(role_res, );

	int hp_upgrade = (from_level - reduce_level) * role_res->hp_growth;

	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_HP_BASE, 0 - hp_upgrade, true);

	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_STR_ATT_BASE, -(from_level - reduce_level) * role_res->phy_att_growth, true);
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_STR_DEF_BASE, -(from_level - reduce_level) * role_res->phy_def_growth, true);
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_AGI_ATT_BASE, -(from_level - reduce_level) * role_res->agi_att_growth, true);
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_AGI_DEF_BASE, -(from_level - reduce_level) * role_res->agi_def_growth, true);
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_INT_ATT_BASE, -(from_level - reduce_level) * role_res->int_att_growth, true);
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_INT_DEF_BASE, -(from_level - reduce_level) * role_res->int_def_growth, true);

	int initiative_value = (from_level - reduce_level) * m_system_res_info->level_first_strike;
	MODIFY_ROLE_ATTR(m_plugin_cfg.scene, role, RAT_INITIATIVE_LEVEL, 0 - initiative_value, true);
	
}

void ManagePlayer::notifyRoleLevelUp(typed::entity::Player * player, typed::entity::Role * role, int pre_level)
{
	if (role->guid() == player->roles(0))
	{
		player->set_level(role->level());

		m_plugin_cfg.scene->collectPlayerLevel(player->guid(), player->level());

		POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

		typed::protocol::lmsg_player_upgrade_level * lmsg_ul = new typed::protocol::lmsg_player_upgrade_level();
		lmsg_ul->set_curr_level(role->level());
		lmsg_ul->set_pre_level(pre_level);
		PackInfo * pi = new PackInfo(LMSG_PLAYER_UPGRADE_LEVEL, player->guid(), lmsg_ul);
		m_plugin_cfg.line_scene->scenePackInput(pi);

		m_plugin_cfg.manage_grid->levelup(player);

		typed::protocol::pmsg_player_level_upgrade * level_up = new typed::protocol::pmsg_player_level_upgrade();
		level_up->set_player(player->guid());
		level_up->set_pre_level(pre_level);
		level_up->set_curr_level(role->level());
		MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_PLAYER_LEVEL_UP, player->guid(), level_up);
	}

	// update level
	typed::protocol::smsg_role_upgrade_level upgrade_level;
	upgrade_level.set_role_guid(role->guid());
	upgrade_level.set_upgrade_level(role->level());
	upgrade_level.set_upgrade_exp(role->exp());
	upgrade_level.set_upgrade_hp(GET_ROLE_ATTR(role, RAT_HP));
	MAKE_NEW_PACKET(ps, SMSG_ROLE_UPGRADE_LEVEL, role->player(), upgrade_level.SerializeAsString());
	SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);
}

void ManagePlayer::notifyRoleUpdateExp(typed::entity::Player * player, typed::entity::Role * role)
{
	typed::protocol::smsg_role_upgrade_level upgrade_level;
	upgrade_level.set_role_guid(role->guid());
	upgrade_level.set_upgrade_level(role->level());
	upgrade_level.set_upgrade_exp(role->exp());
	upgrade_level.set_upgrade_hp(GET_ROLE_ATTR(role, RAT_HP));
	MAKE_NEW_PACKET(ps, SMSG_ROLE_UPGRADE_LEVEL, role->player(), upgrade_level.SerializeAsString());
	SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);
}

int ManagePlayer::playerJumpMap(typed::entity::Player * player, uint32 target_map_id)
{
	//if (target_map_id == player->map_id())
	//{
	//	// error, todo
	//	PLAYER_LOG_ERROR("player is jump to the same map : <%d>\n", player->map_id());
	//	return 0;
	//}

	PLAYER_LOG_DEBUG("enter jump map, target map is <%d>, curr map is <%d>", target_map_id, player->map_id());

	MapResInfoMap_t::iterator it = m_map_info_map->find(player->map_id());
	if (m_map_info_map->end() == it)
	{
		PLAYER_LOG_ERROR("faile to get player map info by id <%d>", player->map_id());
		return 0;
	}

	MapResInfo * curr_map_res_info = it->second;

	MapResInfo * map_res_info;
	if (reach_map_condition(target_map_id, player, &map_res_info))
	{
		// collect transfer player info 
		if ((1 == curr_map_res_info->type || 8 == curr_map_res_info->type || 100 == curr_map_res_info->type) && (1 == map_res_info->type || 8 == map_res_info->type || 100 == map_res_info->type) 
			&& (target_map_id != player->map_id()))
		{
			if ((!isGuildBattleMap(map_res_info))
				|| (isGuildBattleMap(map_res_info) && canEnterGuildBattleMap(player, map_res_info)))
			{
				MANAGE_GRID_REMOVE(m_plugin_cfg.manage_grid, player);

				notifyPlayerJumpMap(player->guid(), player->map_id(), target_map_id);

				m_plugin_cfg.pool->playerIsOffline(player->guid());
				m_plugin_cfg.scene->playerOffline(player->guid());

				PlayerOnlineStateMap_t::iterator it = m_player_state.find(player->guid());
				if (it != m_player_state.end())
				{
					TransferPlayerInfo * transfer_info = collectTransferPlayerInfo(player, target_map_id);
					if (NULL != transfer_info)
					{
						it->second = POS_FLUSHING_LEAVE_MAP_WITH_JUMP;

						it->second.transfer_info = transfer_info;

						player->set_map_id(map_res_info->map_id);
						player->set_src_x(map_res_info->born_x);
						player->set_src_y(map_res_info->born_y);
						player->set_dst_x(map_res_info->born_x);
						player->set_dst_y(map_res_info->born_y);

						//m_player_assistant.request_flush_player(player->guid(), true);
						m_need_flush_player_map[player->guid()] = true;

						m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 1502, 1502, player->guid(), player->name(), player->level(), target_map_id, m_plugin_cfg.template_id);
					}
					else
					{
						PLAYER_LOG_ERROR("failed to collect player transfer info");
					}
				}
				else
				{
					PLAYER_LOG_ERROR("failed to get player state while player jump map");
				}
			}
		}
		else
		{
			//typed::protocol::smsg_player_jump_map s_jump_map;
			//s_jump_map.set_target_map_id(map_res_info->map_id);
			//s_jump_map.set_src_x(map_res_info->born_x);
			//s_jump_map.set_src_y(map_res_info->born_y);
			//MAKE_NEW_PACKET(ps, SMSG_PLAYER_JUMP_MAP, player->guid(), s_jump_map.SerializeAsString());
			//SEND_PLAYER_MESSAGE(m_plugin_cfg.message, ps);
		}

		// no add, the other scene should call add
		//MANAGE_GRID_ADD(m_plugin_cfg.manage_grid, player);

	}
	else
	{
		// todo error message
		PLAYER_LOG_ERROR("failed to check map condition, target map is <%d>, curr map is <%d>", target_map_id, player->map_id());
	}
	return 0;
}

TransferPlayerInfo * ManagePlayer::collectTransferPlayerInfo(typed::entity::Player * player, uint32 target_map_id)
{
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, NULL);

	GET_COIN_FROM_POOL(m_plugin_cfg.pool, player->coin_guid(), coin);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, NULL);

	TransferPlayerInfo * trans_player_info = new TransferPlayerInfo();
	trans_player_info->player_guid = player->guid();
	trans_player_info->player = player;
	trans_player_info->target_map_id = target_map_id;

	//trans_player_info->putMsg(player->guid(), player);
	PlayerAssistant::EntityGuidMap_t entity_guid_map;
	m_player_assistant.getPlayerEntityMap(player->guid(), entity_guid_map, false);
	for (PlayerAssistant::EntityGuidMap_t::iterator it = entity_guid_map.begin(); it != entity_guid_map.end(); ++it)
	{
		trans_player_info->putMsg(it->first, it->second);
	}

	trans_player_info->putMsg(coin->guid(), coin);
	m_plugin_cfg.pool->remove(coin->guid(), false, false);

	trans_player_info->putMsg(player->guid(), player);

	GET_ATTACH_FROM_POOL(m_plugin_cfg.pool, player->attach(), attach);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(attach, );
	if (NULL != attach)
	{
		trans_player_info->putMsg(attach->guid(), attach);
		m_plugin_cfg.pool->remove(attach->guid(), false, false);

		for (int i = 0; i < attach->buyback_items_size(); ++i)
		{
			GET_ITEM_FROM_POOL(m_plugin_cfg.pool, attach->buyback_items(i), item);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);
			trans_player_info->putMsg(item->guid(), item);
			m_plugin_cfg.pool->remove(item->guid(), false, false);
		}
	}

	return trans_player_info;

	//for (int i = 0; i < player->roles_size(); ++i)
	//{
	//	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(i), role);
	//	PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);
	//	trans_player_info->putMsg(role->guid(), role);

	//	// flush role equipment
	//	for (int j = 0; j < role->equipments_size(); ++j)
	//	{
	//		if (role->equipments(j) > 1)
	//		{
	//			GET_ITEM_FROM_POOL(m_plugin_cfg.pool, role->equipments(j), item);
	//			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);
	//			trans_player_info->putMsg(item->guid(), item);
	//		}
	//	}
	//}

	//// flush leave team role
	//for (int i = 0; i < player->leave_team_roles_size(); ++i)
	//{
	//	GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->leave_team_roles(i), leave_role);
	//	if (NULL != leave_role)
	//	{
	//		trans_player_info->putMsg(leave_role->guid(), leave_role);
	//	}

	//	// flush role equipment
	//	for (int j = 0; j < leave_role->equipments_size(); ++j)
	//	{
	//		if (leave_role->equipments(j) > 1)
	//		{
	//			GET_ITEM_FROM_POOL(m_plugin_cfg.pool, leave_role->equipments(j), item);
	//			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);
	//			trans_player_info->putMsg(item->guid(), item);
	//		}
	//	}
	//}

	//// flush form
	//GET_FORM_FROM_POOL(m_plugin_cfg.pool, player->form(), form);
	//if (NULL != form)
	//{
	//	trans_player_info->putMsg(form->guid(), form);
	//}

	//GET_LAW_FROM_POOL(m_plugin_cfg.pool, player->law(), law);
	//if (NULL != law)
	//{
	//	trans_player_info->putMsg(law->guid(), law);
	//}

	//GET_SECT_FROM_POOL(m_plugin_cfg.pool, player->sect(), sect);
	//if (NULL != sect)
	//{
	//	trans_player_info->putMsg(sect->guid(), sect);
	//}

	//for (int i = 0; i < player->bag_size(); ++i)
	//{
	//	CHECK_ITEM_GUID(player->bag(i));
	//	GET_ITEM_FROM_POOL(m_plugin_cfg.pool, player->bag(i), item);
	//	PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);
	//	trans_player_info->putMsg(item->guid(), item);
	//}

	//for (int i = 0; i < player->storehouse_size(); ++i)
	//{
	//	CHECK_ITEM_GUID(player->storehouse(i));
	//	GET_ITEM_FROM_POOL(m_plugin_cfg.pool, player->storehouse(i), item);
	//	PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);
	//	trans_player_info->putMsg(item->guid(), item);
	//}

	//for (int i = 0; i < player->stheno_idle_list_size(); ++i)
	//{
	//	uint64 guid = player->stheno_idle_list(i);
	//	if (guid > 1)
	//	{
	//		GET_STHENO_FROM_POOL(m_plugin_cfg.pool, guid, stheno);
	//		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(stheno, continue);
	//		trans_player_info->putMsg(guid, stheno);
	//	}
	//}

	//for (int i = 0; i < player->stheno_equip_list_size(); ++i)
	//{
	//	uint64 guid = player->stheno_equip_list(i);
	//	if (guid > 1)
	//	{
	//		GET_STHENO_FROM_POOL(m_plugin_cfg.pool, guid, stheno);
	//		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(stheno, continue);
	//		trans_player_info->putMsg(guid, stheno);
	//	}
	//}

	//GET_STAGE_FROM_POOL(m_plugin_cfg.pool, player->stage(), stage);
	//if (NULL != stage)
	//{
	//	trans_player_info->putMsg(player->stage(), stage);
	//}

	//for (int i = 0; i < player->town_stage_size(); ++i)
	//{
	//	GET_TOWNSTAGE_FROM_POOL(m_plugin_cfg.pool, player->town_stage(i), town_stage);
	//	if (NULL != town_stage)
	//	{
	//		trans_player_info->putMsg(player->town_stage(i), town_stage);
	//	}
	//}


	//GET_ANNOUNCE_FROM_POOL(m_plugin_cfg.pool, player->announce(), announce);
	//if (NULL != announce)
	//{
	//	trans_player_info->putMsg(player->announce(), announce);
	//}




	//// task brief
	//GET_TASKBRIEF_FROM_POOL(m_plugin_cfg.pool, player->task_brief(), task_brief);
	//if (NULL != task_brief)
	//{
	//	trans_player_info->putMsg(player->task_brief(), task_brief);

	//	// task
	//	for (int i = 0; i < task_brief->unfinished_size(); ++i)
	//	{
	//		GET_TASK_FROM_POOL(m_plugin_cfg.pool, task_brief->unfinished(i), task_entity);
	//		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(task_entity, continue);
	//		trans_player_info->putMsg(task_brief->unfinished(i), task_entity);
	//	}

	//	for (int i = 0; i < task_brief->finished_size(); ++i)
	//	{
	//		GET_TASK_FROM_POOL(m_plugin_cfg.pool, task_brief->finished(i), task_entity);
	//		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(task_entity, continue);
	//		trans_player_info->putMsg(task_brief->finished(i), task_entity);
	//	}

	//	for (int i = 0; i < task_brief->failed_size(); ++i)
	//	{
	//		GET_TASK_FROM_POOL(m_plugin_cfg.pool, task_brief->failed(i), task_entity);
	//		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(task_entity, continue);
	//		trans_player_info->putMsg(task_brief->failed(i), task_entity);
	//	}
	//}

	//// town stage
	//for (int i = 0; i < player->town_stage_size(); ++i)
	//{
	//	GET_TOWNSTAGE_FROM_POOL(m_plugin_cfg.pool, player->town_stage(i), town_stage);
	//	if (NULL != town_stage)
	//	{
	//		trans_player_info->putMsg(player->town_stage(i), town_stage);
	//	}
	//}

	//// educate
	//GET_EDUCATETIMES_FROM_POOL(m_plugin_cfg.pool, player->educate(), educate_times);
	//if (NULL != educate_times)
	//{
	//	trans_player_info->putMsg(educate_times->guid(), educate_times);
	//}

	//GET_ATTACH_FROM_POOL(m_plugin_cfg.pool, player->attach(), attach);
	//if (NULL != attach)
	//{
	//	trans_player_info->putMsg(player->attach(), attach);
	//}

	//// spec shop
	//GET_SPECSHOP_FROM_POOL(m_plugin_cfg.pool, player->specshop(), spec_shop);
	//if (NULL != spec_shop)
	//{
	//	trans_player_info->putMsg(player->specshop(), spec_shop);
	//}

	//GET_CHAKRA_FROM_POOL(m_plugin_cfg.pool, player->chakra(), chakra);
	//if (NULL != chakra)
	//{
	//	trans_player_info->putMsg(player->chakra(), chakra);
	//	for (int i = 0; i < chakra->storehouse_size(); ++i)
	//	{
	//		if (chakra->storehouse(i) > 1)
	//		{
	//			GET_CHAKRE_FROM_POOL(m_plugin_cfg.pool, chakra->storehouse(i), chakre);
	//			if (NULL != chakre)
	//			{
	//				trans_player_info->putMsg(chakre->guid(), chakre);
	//			}
	//		}
	//	}
	//}

	//GET_WORSHIP_FROM_POOL(m_plugin_cfg.pool, player->worship(), worship);
	//if (NULL != worship)
	//{
	//	trans_player_info->putMsg(player->worship(), worship);
	//}

	//return trans_player_info;

	//for (Uint64Vec_t::iterator it = trans_player_info->guid_vec.begin(); it != trans_player_info->guid_vec.end(); ++it)
	//{
	//	m_plugin_cfg.pool->remove(*it, false, false);
	//}

	//m_plugin_cfg.line_scene->transferPlayer(trans_player_info);
}

void ManagePlayer::transferPlayerToLineScene(TransferPlayerInfo * transfer_info)
{
	m_plugin_cfg.line_scene->transferPlayer(transfer_info);
}

void ManagePlayer::playerFlushDataFinishAfterLostGateConnection(uint64 player_guid, bool is_success, RequestList * request_list)
{
	m_flush_back_player_after_lost_gate_connection.insert(player_guid);

	for (RequestList::RequestVec_t::const_iterator req_it = request_list->getRequestList().begin(); req_it !=request_list->getRequestList().end(); ++req_it)
	{
		Request * req = *req_it;
		m_plugin_cfg.pool->remove(req->guid, false, false);
	}

	if (m_need_flush_player_after_lost_gate_connection.size() == m_flush_back_player_after_lost_gate_connection.size())
	{
		PLAYER_LOG_INFO("all player is flushing finish, line is <%d>, map is <%d>, player num is <%d>", m_plugin_cfg.line_no, m_plugin_cfg.template_id, m_need_flush_player_after_lost_gate_connection.size());
		m_flush_entity_finish.flush_player_finish = true;
		checkFlushStatus();
	}
}

void ManagePlayer::checkFlushStatus()
{
	if (m_flush_entity_finish.is_finish(IS_LINE_SCENE()))
	{
		m_shutdown_success = true;
		//if (!m_plugin_cfg.scene->isAllPlayersFlushedAfterLostGateConnection())
		//{
		//	PLAYER_LOG_INFO("all entity is flushing finish, line is <%d>, map is <%d>", m_plugin_cfg.line_no, m_plugin_cfg.template_id);
		//	m_plugin_cfg.scene->allPlayersFlushedAfterLostGateConnection();
		//}
	}
}

void ManagePlayer::processNeedFlushPlayer()
{
	if (m_plugin_cfg.pool->isLastUpdateFinish())
	{
		if (m_lost_gate_connection)
		{
			if (!m_send_notify_gate_lost_connection)
			{
				m_send_notify_gate_lost_connection = true;
				MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_FLUSH_ALL_ENTITY, 0, NULL);
			}

			if (m_need_flush_player_map.size() == 0)
			{
				checkFlushStatus();
			}

			for (NeedFlushPlayerMap_t::iterator it = m_need_flush_player_map.begin(); it != m_need_flush_player_map.end(); ++it)
			{
				uint64 player = it->first;

				notifyPlayerIsOffline(player);

				//m_plugin_cfg.pool->clearUpdateEntity(player);

				m_player_assistant.request_flush_player(player, false);
			}
		}
		else
		{
			for (NeedFlushPlayerMap_t::iterator it = m_need_flush_player_map.begin(); it != m_need_flush_player_map.end(); ++it)
			{
				uint64 player = it->first;
				if (it->second)
				{
					// jump map
					m_player_assistant.request_flush_player(player, it->second);
				}
				else
				{
					PlayerOnlineStateMap_t::iterator it_staus = m_player_state.find(player);
					if (it_staus != m_player_state.end())
					{
						if (it_staus->second.player_state == POS_ONLINE)
						{
							continue;
						}
					}

					notifyPlayerIsOffline(player);

					//m_plugin_cfg.pool->clearUpdateEntity(player);

					m_player_assistant.request_flush_player(player, it->second);
				}
			}
		}


		m_need_flush_player_map.clear();
	}
}

void ManagePlayer::checkPlayerCleanDay(typed::entity::Player * player)
{
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

	if (m_current_day > player->last_clean_day_relational_time())
	{
		playerDayChange(player, 2);
	}
}

void ManagePlayer::checkPlayerCleanWeek(typed::entity::Player * player)
{
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

	if (m_current_week > player->last_clean_week_relational_time())
	{
		playerWeekChange(player);
	}
}

void ManagePlayer::updateVipExp(typed::entity::Player * player, int increase_exp, int source_from)
{
	PLAYER_LOG_INFO("player get ready to increase vip exp, player is <%llu>, vip exp is <%d>", player->guid(), increase_exp);
	uint32 from_vip_level = player->vip_level();
	int vip_exp = player->vip_exp() + increase_exp;
	PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(vip_exp > 0, );

	int vip_level = 0;
	for (VipexpResInfoMap_t::iterator it = m_vipexp_res_info_map->begin(); it != m_vipexp_res_info_map->end(); ++it)
	{
		VipexpInfo * vipexp_info = it->second;
		vip_level = it->first;
		if (vip_exp < vipexp_info->vip_exp)
		{
			vip_level = it->first - 1;
			break;
		}
	}

	player->set_vip_exp(vip_exp);

	typed::protocol::smsg_vip_levelup s_vip_levelup;
	s_vip_levelup.set_curr_vip_level(vip_level);
	s_vip_levelup.set_curr_vip_exp(vip_exp);

	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_VIP_LEVELUP, player->guid(), s_vip_levelup.SerializeAsString());

	if (vip_level > player->vip_level())
	{
		player->set_vip_level(vip_level);
		notifyPlayerVipLevelUp(player, from_vip_level);
		notifyPlayerVipLevelUpToLineScene(player, from_vip_level);
		// vip level
		m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 274, source_from, player->guid(), player->name(), player->level(), 
			from_vip_level, player->vip_level(), increase_exp, player->vip_exp() - increase_exp);
	}

	// vip exp
	m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 271, source_from, player->guid(), player->name(), player->level(), 
		increase_exp, player->vip_exp() - increase_exp, player->vip_exp(), player->vip_level());

	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());
}

void ManagePlayer::notifyPlayerVipLevelUp(typed::entity::Player * player, uint32 from_vip_level)
{
	typed::protocol::nmsg_player_vip_level_up * vip_level_up = new typed::protocol::nmsg_player_vip_level_up();
	vip_level_up->set_from_vip_level(from_vip_level);
	vip_level_up->set_to_vip_level(player->vip_level());
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_PLAYER_VIP_LEVEL_UP, player->guid(), vip_level_up);
}

void ManagePlayer::notifyPlayerVipLevelUpToLineScene(typed::entity::Player * player, uint32 from_vip_level)
{
	typed::protocol::lmsg_vip_levleup * vip_level_up = new typed::protocol::lmsg_vip_levleup();
	vip_level_up->set_from_vip_level(from_vip_level);
	vip_level_up->set_to_vip_level(player->vip_level());

	PackInfo * pi = new PackInfo(LMSG_VIP_LEVLEUP, player->guid(), vip_level_up);
	m_plugin_cfg.line_scene->scenePackInput(pi);
}

void ManagePlayer::attachRoleCharAbility(typed::entity::Role * role, uint32 char_ability)
{
	bool update_char_ability_to_client = false;
	if (0 != role->char_ability())
	{
		typed::protocol::pmsg_detach_skill * detach_skill = new typed::protocol::pmsg_detach_skill();
		detach_skill->set_role_guid(role->guid());
		detach_skill->set_skill_id(char_ability);
		PackInfo pi(PMSG_DETACH_SKILL, role->player(), detach_skill);

		REQUEST_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);

		bool update_char_ability_to_client = true;
	}

	role->set_char_ability(char_ability);

	typed::protocol::pmsg_attach_skill * attach_skill = new typed::protocol::pmsg_attach_skill();
	attach_skill->set_role_guid(role->guid());
	attach_skill->set_skill_id(role->char_ability());
	PackInfo pi(PMSG_ATTACH_SKILL, role->player(), attach_skill);
	REQUEST_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);
	
	if (update_char_ability_to_client)
	{
		typed::protocol::smsg_update_char_ability update_char_ability;
		update_char_ability.set_role_guid(role->guid());
		update_char_ability.set_char_ability(role->char_ability());
		MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_UPDATE_CHAR_ABILITY, role->player(), update_char_ability.SerializeAsString());
	}
}

bool ManagePlayer::is_valid_charge_request(typed::entity::Vip * vip, uint32 request_index, uint32 charge_value)
{
	for (int i = 0; i < vip->charge_info_size(); ++i)
	{
		if (vip->charge_info(i).request_index() == request_index)
		{
			PLAYER_LOG_ERROR("the player get reduplicate charge request index, player <%llu>, request index <%d>", vip->player(), request_index);
			return false;
		}
	}

	PLAYER_LOG_INFO("valid charge requset, player is <%llu>, request is <%d>, charge value is <%d>", vip->player(), request_index, charge_value);

	return true;
}

void ManagePlayer::recodeChargeInfo(typed::entity::Vip * vip, uint32 request_index, uint32 charge_value)
{
	typed::entity::Vip_ChargeInfo * vip_charge = vip->add_charge_info();
	vip_charge->set_request_index(request_index);
	vip_charge->set_charge_value(charge_value);

	uint64 curr_time = 0;
	ACE_OS::gettimeofday().to_usec(curr_time);
	vip_charge->set_charge_time(curr_time);

	vip->set_charge_coin(vip->charge_coin() + charge_value);

	POOL_OP_UPDATE(m_plugin_cfg.pool, vip->guid());
}

void ManagePlayer::playerDayChange(typed::entity::Player * player, int online_status)
{
	typed::protocol::nmsg_day_change * msg_day_change = new typed::protocol::nmsg_day_change();
	uint64 day_interval = (m_current_day - player->last_clean_day_relational_time()) / (24 * 60 * 60);
	msg_day_change->set_day_interval(day_interval);
	msg_day_change->set_online_status(online_status);

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_DAY_CHANGE, player->guid(), msg_day_change);
	player->set_last_clean_day_relational_time(m_current_day);
	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	typed::protocol::lmsg_day_change_for_player * ldc_for_player = new typed::protocol::lmsg_day_change_for_player();
	ldc_for_player->set_online_status(online_status);
	PackInfo * ls_pi = new PackInfo(LMSG_DAY_CHANGE_FOR_PLAYER, player->guid(), ldc_for_player);
	m_plugin_cfg.line_scene->scenePackInput(ls_pi);
}

void ManagePlayer::notifyClientDayChange(typed::entity::Player * player)
{
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_DAY_CHANGE, player->guid(), "");
}

void ManagePlayer::playerWeekChange(typed::entity::Player * player)
{
	typed::protocol::nmsg_week_change * nw_change = new typed::protocol::nmsg_week_change();
	nw_change->set_online_status(2);

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_WEEK_CHANGE, player->guid(), nw_change);
	player->set_last_clean_week_relational_time(m_current_week);
	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	typed::protocol::lmsg_week_change_for_player * lwc_for_player = new typed::protocol::lmsg_week_change_for_player();
	lwc_for_player->set_online_status(2);
	PackInfo * ls_pi = new PackInfo(LMSG_WEEK_CHANGE_FOR_PLAYER, player->guid(), lwc_for_player);
	m_plugin_cfg.line_scene->scenePackInput(ls_pi);
}

void ManagePlayer::playerOnlineAgain(typed::entity::Player * player)
{
	dispatchPlayerData(player);

	PackInfo * pi = new PackInfo(LMSG_PLAYER_ONLINE_AGAIN, player->guid(), NULL);
	m_plugin_cfg.line_scene->scenePackInput(pi);
}

void ManagePlayer::calcOfflineExp(typed::entity::Player * player)
{
	uint64 curr_time = 0;
	ACE_OS::gettimeofday().to_usec(curr_time);
	curr_time /= 1000000;
	player->set_last_login_time(curr_time);
	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	if (player->level() < m_offline_hangup_exp_unlock_level)
	{
		return ;
	}

	if (player->last_logoff_time() == 0)
	{
		return ;
	}

	int diff_time = curr_time - player->last_logoff_time();
	if (diff_time > 0)
	{
		// calc offline exp
		diff_time = diff_time <= m_offline_hangup_res->maxtime ? diff_time : m_offline_hangup_res->maxtime;
		double exp = (double)diff_time / m_offline_hangup_res->time_v * ((m_offline_hangup_res->a / 100.0 ) * player->level() + m_offline_hangup_res->b);
		if (exp > 0)
		{
			updateRoleExp(player->roles(0), exp, player->addiction_status(), 0, 0);
			typed::protocol::smsg_gain_offline_exp sgo_exp;
			sgo_exp.set_exp(exp);
			MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_GAIN_OFFLINE_EXP, player->guid(), sgo_exp.SerializeAsString());
		}
	}
	else
	{
		PLAYER_LOG_ERROR("calc player offline exp <%llu>, but the diff time is <%d>, last logoff time is <%llu>, curr time is <%llu>", player->guid(), diff_time, player->last_logoff_time(), curr_time);
	}
}

bool ManagePlayer::isGuildBattleMap(MapResInfo * map_res_info)
{
	return (8 == map_res_info->type);
}

bool ManagePlayer::canEnterGuildBattleMap(typed::entity::Player * player, MapResInfo * map_res_info)
{
	return m_plugin_cfg.line_scene->validatePlayerMapQualification(map_res_info->map_id, player->guid());
}

//#define CONVERT_RANK(attr_value, player_level)	attr_value * m_system_res_info->rank_param / player_level

//int ManagePlayer::getAttrRank(typed::entity::Role * role, int attr_index, int attr_rank_index, int player_level)
//{
//	int rank_value = role->attributes(attr_rank_index);
//	int rank_attr = CONVERT_RANK(rank_value, player_level);
//	int attr_value = role->attributes(attr_index);
//	int src_attr_value = attr_value - rank_attr;
//	if (src_attr_value < 0)
//	{
//		PLAYER_LOG_ERROR("get wrong attr rank value, player is <%llu>, role is <%llu>, attr index is <%d>, rank index is <%d>", 
//			role->player(), role->guid(), attr_index, attr_rank_index);
//		src_attr_value = 0;
//	}
//	rank_value += src_attr_value * 0.5 + 0.99;
//	return rank_value;
//}

void ManagePlayer::checkEnergyReward()
{
	uint64 curr_time_in_minute = getCurrTimeInMinute();
	uint64 day_base_in_minute = getCurrentDay();
	day_base_in_minute /= 60;

	for (int i = 0; i < m_system_res_info->energy_reward_time.size(); ++i)
	{
		uint64 reward_time = day_base_in_minute + m_system_res_info->energy_reward_time[i];
		if (curr_time_in_minute >= reward_time)
		{
			if (m_last_provide_energy_time < reward_time)
			{
				// ok, it's time to provide energy
				m_last_provide_energy_time = reward_time;
				PlayerGuidSet_t players = m_plugin_cfg.pool->getOnlinePlayer();
				for (PlayerGuidSet_t::iterator it = players.begin(); it != players.end(); ++it)
				{
					provideEnergy(*it, m_system_res_info->energy_reward_value, curr_time_in_minute);
				}
			}
		}
	}
}

void ManagePlayer::provideEnergy(uint64 player_guid, uint32 energy_value, uint64 provide_time)
{
	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, player_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

	player->set_last_energy_reward_time(provide_time);
	player->set_left_energy_reward(energy_value);

	POOL_OP_UPDATE(m_plugin_cfg.pool, player_guid);

	notifyClientProvideEnergy(player->guid(), energy_value);

	typed::protocol::lmsg_player_notify * lp_notify = new typed::protocol::lmsg_player_notify();
	lp_notify->set_id(422);
	PackInfo * pi = new PackInfo(LMSG_PLAYER_NOTIFY, player->guid(), lp_notify);
	m_plugin_cfg.line_scene->scenePackInput(pi);
}

void ManagePlayer::notifyClientProvideEnergy(uint64 player_guid, uint32 energy_value)
{
	typed::protocol::smsg_update_plentiful_energy sup_energy;
	sup_energy.set_energy_value(energy_value);
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_UPDATE_PLENTIFUL_ENERGY, player_guid, sup_energy.SerializeAsString());
}

void ManagePlayer::checkPlayerProvideEnergy(typed::entity::Player * player)
{
	if (player->last_energy_reward_time() == 0)
	{
		player->set_last_energy_reward_time(m_last_provide_energy_time);
		POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());
		return ;
	}

	if (player->last_energy_reward_time() < m_last_provide_energy_time)
	{
		provideEnergy(player->guid(), m_last_provide_energy_value, m_last_provide_energy_time);
		//player->set_last_energy_reward_time(m_last_provide_energy_time);
		//player->set_left_energy_reward(m_last_provide_energy_value);

		//POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	}
	else
	{
		if (player->left_energy_reward() > 0)
		{
			notifyClientProvideEnergy(player->guid(), player->left_energy_reward());
		}
	}
}

void ManagePlayer::initProvideEnergyInfo()
{
	uint64 curr_time_in_minute = getCurrTimeInMinute();
	uint64 day_base_in_minute = getCurrentDay();
	day_base_in_minute /= 60;

	bool is_processed = false;

	if (curr_time_in_minute < m_system_res_info->energy_reward_time[0] + day_base_in_minute)
	{
		m_last_provide_energy_time = day_base_in_minute - (24 * 60 - m_system_res_info->energy_reward_time[m_system_res_info->energy_reward_time.size() - 1]);
		m_last_provide_energy_value = m_system_res_info->energy_reward_value;
		is_processed = true;
	}

	if (curr_time_in_minute >= m_system_res_info->energy_reward_time[m_system_res_info->energy_reward_time.size() - 1] + day_base_in_minute)
	{
		m_last_provide_energy_time = day_base_in_minute + m_system_res_info->energy_reward_time[m_system_res_info->energy_reward_time.size() - 1];
		m_last_provide_energy_value = m_system_res_info->energy_reward_value;
		is_processed = true;
	}

	if (!is_processed)
	{
		for (int i = 1; i < m_system_res_info->energy_reward_time.size(); ++i)
		{
			uint64 reward_time = day_base_in_minute + m_system_res_info->energy_reward_time[i];
			if (curr_time_in_minute < reward_time)
			{
				m_last_provide_energy_time = day_base_in_minute + m_system_res_info->energy_reward_time[i - 1];
				m_last_provide_energy_value = m_system_res_info->energy_reward_value;
			}
		}
	}
}

void ManagePlayer::notifySystemUnlock(uint64 player_guid, uint32 system_id)
{
	typed::protocol::nmsg_system_unlock * ns_unlock = new typed::protocol::nmsg_system_unlock();
	ns_unlock->set_system_id(system_id);

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_SYSTEM_UNLOCK, player_guid, ns_unlock);
}

bool ManagePlayer::reduceInheritMoney(typed::entity::Player * player, uint32 from_role_level, uint32 inherit_type, uint32 & cost_money, uint32 & cost_coin)
{
	typed::protocol::pmsg_reduce_gold * pr_gold = new typed::protocol::pmsg_reduce_gold();
	pr_gold->set_player(player->guid());
	pr_gold->set_coin(m_inherite_gold_res->b_coin);
	pr_gold->set_function_id(m_inherite_gold_res->fun_id);
	cost_coin = m_inherite_gold_res->b_coin;

	pr_gold->set_source_from(2902);

	PackInfo pi(PMSG_REDUCE_GOLD, player->guid(), pr_gold);
	if (m_plugin_cfg.message->requestMsgToPlugins(pi))
	{
		if (pr_gold->result() == 1)
		{
			return true;
		}
	}
	
	return false;
}

GoldParamResInfo * ManagePlayer::getGoldParamResInfo(uint32 gold_id)
{
	GoldParamResInfoMap_t::iterator it = m_gold_param_info_res_map->find(gold_id);
	if (it != m_gold_param_info_res_map->end())
	{
		return it->second;
	}

	return NULL;
}

void ManagePlayer::notifyRoleAttrUpdate(typed::entity::Role * role)
{
	typed::protocol::smsg_update_role_attr update_role_attr;
	update_role_attr.add_attr_index(20);
	update_role_attr.add_attr_value(role->attributes(RAT_FIGHT_POWER));

	update_role_attr.add_attr_index(3);
	update_role_attr.add_attr_value(role->attributes(RAT_STR));

	update_role_attr.add_attr_index(4);
	update_role_attr.add_attr_value(role->attributes(RAT_AGI));

	update_role_attr.add_attr_index(5);
	update_role_attr.add_attr_value(role->attributes(RAT_INT));

	update_role_attr.add_attr_index(2);
	update_role_attr.add_attr_value(role->attributes(RAT_HP));

	update_role_attr.add_attr_index(1);
	update_role_attr.add_attr_value(role->attributes(RAT_CURR_HP));

	update_role_attr.set_role_guid(role->guid());
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_UPDATE_ROLE_ATTR, role->player(), update_role_attr.SerializeAsString());
}

//bool ManagePlayer::resetInheritCoinInfo()
//{
//	//for (LevelCostMap_t::iterator it = m_inherite_info_res->level_cost_map.begin(); it != m_inherite_info_res->level_cost_map.end(); ++it)
//	//{
//	//	LevelCost * level_cost = it->second;
//	//	GoldParamResInfo * gold_info = getGoldParamResInfo(level_cost->goldblessme_id);
//	//	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(gold_info, false);
//	//	level_cost->coin = gold_info->b_coin;
//	//}
//
//	return true;
//}


void ManagePlayer::checkDoubleexp(typed::entity::Player * player)
{
	if (player->doubleexp() == 0)
	{
		uint64 doubleexp_guid = 0;
		m_plugin_cfg.scene->get_guid(ET_DOUBLEEXP, doubleexp_guid);
		typed::entity::Doubleexp * doubleexp = new typed::entity::Doubleexp();
		doubleexp->set_guid(doubleexp_guid);
		doubleexp->set_player(player->guid());
		doubleexp->set_id(0);
		doubleexp->set_left_num(0);
		doubleexp->set_mutil_value(0);

		POOL_OP_ADD(m_plugin_cfg.pool, doubleexp->guid(), doubleexp, true, player->guid());

		player->set_doubleexp(doubleexp->guid());

		POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());
	}
}

void ManagePlayer::updateDoubleexpInfo(typed::entity::Player * player)
{
	GET_DOUBLE_FROM_POOL(m_plugin_cfg.pool, player->doubleexp(), doubleexp);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(doubleexp, );

//最后一次使用已经id赋值成0了,这句导致最后一次使用没有下发更新消息
//	if (doubleexp->id() != 0)
	{
		typed::protocol::smsg_update_doubleexp_info sud_info;
		sud_info.set_id(doubleexp->id());
		sud_info.set_left_num(doubleexp->left_num());
		sud_info.set_mutil_value(doubleexp->mutil_value());

		MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_UPDATE_DOUBLEEXP_INFO, player->guid(), sud_info.SerializeAsString());
	}
}

DoubleexpRes * ManagePlayer::getDoubleexpRes(uint32 id)
{
	DoubleexpResMap_t::iterator it = m_doubleexp_res_map->find(id);
	if (it != m_doubleexp_res_map->end())
	{
		return it->second;
	}

	return NULL;
}

bool ManagePlayer::useMutilexp(uint32 source_from)
{
	if ((302 == source_from) || (1402 == source_from))
	{
		return true;
	}

	return false;
}

void ManagePlayer::fillInheritSubToken(::google::protobuf::RepeatedPtrField< ::typed::protocol::smsg_select_inherit_role_SubToken > * dst_st, ::google::protobuf::RepeatedPtrField< ::typed::protocol::nmsg_inherit_select_SubToken > * src_st)
{
	for (int i = 0; i < src_st->size(); ++i)
	{
		::typed::protocol::nmsg_inherit_select_SubToken * src_sub_token = src_st->Mutable(i);
		
		::typed::protocol::smsg_select_inherit_role_SubToken * dst_sub_token = dst_st->Add();
		
		dst_sub_token->set_id(src_sub_token->id());
		dst_sub_token->set_stage(src_sub_token->stage());
		dst_sub_token->set_level(src_sub_token->level());
		dst_sub_token->set_exp(src_sub_token->exp());
		dst_sub_token->set_color(src_sub_token->color());
		dst_sub_token->set_stage_state(src_sub_token->stage_state());
	}
}

bool ManagePlayer::loadPreChargeInfo()
{
	if (10000 != m_plugin_cfg.template_id)
	{
		return true;
	}

	boost::filesystem::path path(pre_charge_file_name);
	if (!boost::filesystem::exists(path))
	{
		return true;
	}

	m_pre_charge_doc = XMLFactory::create_document();
	if (NULL == m_pre_charge_doc)
	{
		return false;
	}

	if (!m_pre_charge_doc->load_file(pre_charge_file_name))
	{
		return false;
	}

	Element * root_ele = m_pre_charge_doc->get_root();
	if (NULL == root_ele)
	{
		return false;
	}

	Elements eles = root_ele->get_elements();

	string account_id;
	uint32 charge_coin = 0;
	uint32 used_flag = 0;
	uint32 vipexp = 0;

	for (Elements::iterator it = eles.begin(); it != eles.end(); ++it)
	{
		if (!getAttrValue(*it, "account", account_id))
		{
			return false;
		}

		if (!getAttrValue(*it, "coin", charge_coin))
		{
			return false;
		}

		if (!getAttrValue(*it, "vipexp", vipexp))
		{
			return false;
		}

		if (!getAttrValue(*it, "used", used_flag))
		{
			return false;
		}

		define_unordered_map<string, Element *>::iterator ele_it = m_pre_charge_account_ele_map.find(account_id);
		if (ele_it != m_pre_charge_account_ele_map.end())
		{
			return false;
		}

		m_pre_charge_account_ele_map.insert(std::make_pair(account_id, *it));
	}

	return true;
}

void ManagePlayer::chargePreChargeInfo(typed::entity::Player * player)
{
	GET_COIN_FROM_POOL(m_plugin_cfg.pool, player->coin_guid(), coin);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, );

	uint32 source_from = 10000;

	int32 vip_exp_init = 0;
	define_unordered_map<string, Element *>::iterator it = m_pre_charge_account_ele_map.find(player->account());
	if (m_pre_charge_account_ele_map.end() != it)
	{
		// pre charge
		Element * ele = it->second;
		uint32 charge_coin = 0;
		uint32 vipexp = 0;
		if (!getAttrValue(ele, "coin", charge_coin))
		{
			return ;
		}

		if (!getAttrValue(ele, "vipexp", vipexp))
		{
			return ;
		}

		updateCoin(player, coin, charge_coin, source_from, false);

		typed::protocol::smsg_money_update money_update;
		money_update.set_curr_coin(player->coin());
		MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_MONEY_UPDATE, player->guid(), money_update.SerializeAsString());

		updateVipExp(player, vipexp, source_from);
		vip_exp_init = vipexp;

		ele->set_attribute("used", 1);

		m_pre_charge_doc->save_file(pre_charge_file_name);
	}
	//
	const WebVipInfo& web_vip_info = m_plugin_cfg.scene->getManageResource()->getWebVipInfo();
	if (!player->validate_str().empty())
	{
		char str_temp[256] = {0};
		sprintf(str_temp, "%d%d%d", player->vip_level_init(), player->year_card(), player->time_stamp());

		string md5_result = md5_encode(player->account() + string(str_temp) + m_plugin_cfg.line_scene->getVipInitSecurityKey());

		if (md5_result == player->validate_str())
		{
			Mail_formatInfo* format_info = getMailFormatInfo(54);
			if (format_info == NULL)
			{
				PLAYER_LOG_ERROR("getMailFormatInfo error id <54>!");
				return;
			}		
			string temp_content = format_info->word;
			string::size_type pos = temp_content.find("_");
			if (pos != string::npos)
			{
				temp_content.replace(pos, 1, m_plugin_cfg.server_cfg.platform);
			}

			//vip平台初始等级
			if (player->vip_level_init() >= 1)
			{
				sendMail(player->guid(), format_info->addresser, format_info->title, temp_content, web_vip_info.vip1_item, RECORD_VIP_FROM_PLATFORM);

				if (player->vip_level_init() >= web_vip_info.limit_require)
				{
					BOOST_AUTO(iter_vip, m_vipexp_res_info_map->find(web_vip_info.vip_transform));
					if (iter_vip != m_vipexp_res_info_map->end())
					{
						VipexpInfo * vipexp_info = iter_vip->second;
						if (vipexp_info != NULL)
						{
							if (vipexp_info->vip_exp > vip_exp_init)
							{
								updateVipExp(player, vipexp_info->vip_exp - vip_exp_init, RECORD_VIP_FROM_PLATFORM);
							}
						}
					}
					//
					vector<uint64> vec_guid;
					vec_guid.push_back(0);
					vec_guid.push_back(player->guid());
					vector<string> vec_name;
					vec_name.push_back(m_plugin_cfg.server_cfg.platform);
					vec_name.push_back(player->name());
					req_chat_notify(vec_guid, vec_name, TALK_PLATFORM_VIP_ADD);
				}
			}

			//vip平台年卡
			if (player->year_card() > 0)
			{
				sendMail(player->guid(), format_info->addresser, format_info->title, temp_content, web_vip_info.yearvip_item, RECORD_VIP_FROM_PLATFORM);
				//
				vector<uint64> vec_guid;
				vec_guid.push_back(0);
				vec_guid.push_back(player->guid());
				vector<string> vec_name;
				vec_name.push_back(m_plugin_cfg.server_cfg.platform);
				vec_name.push_back(player->name());
				req_chat_notify(vec_guid, vec_name, TALK_PLATFORM_YEAR_CARD);
			}
		}

	}//if
}
int ManagePlayer::req_chat_notify(const vector<uint64> vec_guid, const vector<string>& vec_name, uint32 talk_id)
{
	BOOST_AUTO(pmsg, new typed::protocol::pmsg_chat_notify);
	pmsg->set_id(talk_id);

	if (vec_guid.size() != vec_name.size())
	{
		return -1;
	}
	for (int i = 0; i < vec_guid.size(); ++i)
	{
		pmsg->add_param_id(vec_guid[i]);
		pmsg->add_param_name(vec_name[i]);
	}

	PackInfo ps(PMSG_CHAT_NOTIFY, 0, pmsg);
	if (!REQUEST_MSG_TO_PLUGINS(m_plugin_cfg.message, ps))
	{
		return -1;
	}
	return 0;
}
Mail_formatInfo* ManagePlayer::getMailFormatInfo(uint32 id)
{
	const Mail_formatInfoMap_t& mail_map = m_plugin_cfg.scene->getManageResource()->getMail_formatInfoMap();
	BOOST_AUTO(iter, mail_map.find(id));
	if (iter != mail_map.end())
	{
		return iter->second;
	}
	return NULL;
}
int ManagePlayer::sendMail(uint64 player_guid
						   , const string& sender
						   , const string& title
						   , const string& tontent
						   , uint32 item_id
						   , uint32 source_from)
{
	typed::protocol::pmsg_mail_send* pmsg = new typed::protocol::pmsg_mail_send;
	pmsg->add_receiver_guid(player_guid);
	pmsg->set_sender(sender);
	pmsg->set_title(title);
	pmsg->set_content(tontent);
	pmsg->add_item_ids(item_id);
	pmsg->add_item_nums(1);
	pmsg->set_mail_type(1);
	pmsg->set_src_from(source_from);

	PackInfo pack_info(PMSG_MAIL_SEND, player_guid, pmsg);
	return m_plugin_cfg.scene->requestMsgToPlugins(pack_info);
}
void ManagePlayer::sendHireZsly(typed::entity::Player * player, int hired)
{
	typed::protocol::smsg_hire_zslw sh_zslw;
	sh_zslw.set_hired(hired);
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_HIRE_ZSLW, player->guid(), sh_zslw.SerializeAsString());
}

void ManagePlayer::deliveryCoinToLS(typed::entity::Player * player)
{
	GET_COIN_FROM_POOL(m_plugin_cfg.pool, player->coin_guid(), coin_entity);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(coin_entity, );

	typed::entity::Coin * clone_coin = (typed::entity::Coin *)cloneMessage(coin_entity);
	clone_coin->set_coin(player->coin());
	clone_coin->set_money(player->money());
	PackInfo * pi = new PackInfo(STOL_DELIVERY_PLAYER_DATA, player->guid(), clone_coin);
	m_plugin_cfg.line_scene->scenePackInput(pi);
}
string ManagePlayer::md5_encode(const std::string& pw)
{
	md5_byte_t digest[16] = {0};
	md5_state_t state;
	md5_init(&state);
	md5_append(&state,(const md5_byte_t*)pw.c_str(),pw.length());
	md5_finish(&state,digest);

	char sOutput[33];
	sprintf(sOutput,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		digest[0],digest[1],digest[2],digest[3],digest[4],digest[5],digest[6],digest[7],
		digest[8],digest[9],digest[10],digest[11],digest[12],digest[13],digest[14],digest[15]);

	return std::string((char*)sOutput);
}
int ManagePlayer::init_robot_info(typed::entity::Player* player)
{
	if (player->robot_id() == 0)
	{
		return 0;
	}

	bool valid_robot = false;
	//
	if (!player->robot_validate().empty())
	{
		char str_temp[256] = {0};
		sprintf(str_temp, "%d", player->robot_id());
		string md5_result = md5_encode( string(str_temp) + m_plugin_cfg.line_scene->getRobotSecurityKey() + player->account());
		if (md5_result == player->robot_validate())
		{
			valid_robot = true;
		}
	}
	if (!valid_robot)
	{
		player->set_robot_id(0);
		POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());
	}
	return 0;
}
int ManagePlayer::gtos_query_qq_vip_info(const PackInfo & pack_info)
{
	typed::protocol::gtos_query_qq_vip_info * vip_info = (typed::protocol::gtos_query_qq_vip_info *)pack_info.msg;
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(vip_info, -1);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, SE_NGUID_CODE);

	player->set_is_yellow_vip(vip_info->is_yellow_vip());
	player->set_is_yelow_year_vip(vip_info->is_yelow_year_vip());
	player->set_yellow_vip_level(vip_info->yellow_vip_level());
	player->set_is_yellow_high_vip(vip_info->is_yellow_high_vip());

	//if (player->is_yellow_vip() == 0)
	//{
	//	player->set_yellow_vip_level(0);
	//}

	POOL_OP_UPDATE(m_plugin_cfg.pool, player->guid());

	PackInfo pi(NMSG_QQ_VIP_INFO, player->guid(), NULL);
	NOTIFY_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);

	m_plugin_cfg.manage_grid->updateYellowVipLevel(player);

	return 0;
}