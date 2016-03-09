
#include <boost/typeof/typeof.hpp>
#include "ManageShort.h"
#include <ace/Date_Time.h>
#include <ace/OS.h>
#include <boost/algorithm/string.hpp>
#include "PlayerFunction.h"
#include "protocol/msg_plugin.pb.h"
#include "typedef.h"
#include "entity/container.pb.h"
#include "entity/runes.pb.h"
#include "protocol/msg_runes.pb.h"
#include "entity/dodge.pb.h"
#include "protocol/msg_gate.pb.h"

#ifndef WIN32
	#define _atoi64(val)	strtol(val, NULL, 10)
#endif

#define SHORT_PLAYER_START_INDEX		100
#define SHORT_PLAYER_TOTAL_NUM			100
#define ONE_SHORT_PLAYER_CONTAIN_NUM	4000

#define UPDATE_CHECK_TIME				500

#define VIP_MAX_LEVEL					15

#define UPDATE_ENTITY_BY_GUID(guid)	\
	if (0 != guid)	\
	{	\
		POOL_OP_UPDATE(m_plugin_cfg.pool, guid);	\
	}

Plugin * make_plugin()
{
	return new ManageShort();
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

ManageShort::ManageShort()
: m_short_owner_guid(0)
, m_available_short_players(NULL)
, m_flush_short_total(0)
, m_curr_flush_short_num(0)
, m_curr_flush_shortplayer_num(0)
, m_is_load_failed(false)
, m_update_timer_id(0)
, m_current_day(0)
, m_current_week(0)
, m_is_shutdown(false)
, m_system_res_info(NULL)
{
	m_plugin_type = PT_MANAGE_SHORT;
	m_plugin_scene_type = PST_ONLY_LINE;
}

ManageShort::~ManageShort()
{

}

int ManageShort::lmsg_player_first_online(const PackInfo & pack_info)
{
	typed::protocol::lmsg_player_first_online * lmsg_pfo = (typed::protocol::lmsg_player_first_online *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(lmsg_pfo, SD_ILLEGAL_CODE);

	SHORT_LOG_DEBUG("first time to register player short info <%llu>", pack_info.guid);

	addNewPlayerShortInfo(pack_info.guid, lmsg_pfo);

	m_plugin_cfg.line_scene->playerLogin(pack_info.guid);

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NLMSG_PLAYER_IS_FIRST_ONLINE, pack_info.guid, NULL);

	return 0;
}

int ManageShort::lmsg_player_upgrade_level(const PackInfo & pack_info)
{
	typed::protocol::lmsg_player_upgrade_level * lmsg_pl = (typed::protocol::lmsg_player_upgrade_level *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(lmsg_pl, SD_ILLEGAL_CODE);

	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player_bi, SD_INTEGRITY_CODE);
	player_bi->short_info->set_level(lmsg_pl->curr_level());

	POOL_OP_UPDATE(m_plugin_cfg.pool, player_bi->short_info->guid());

	typed::protocol::nlmsg_player_upgrade_level * npu_level = new typed::protocol::nlmsg_player_upgrade_level();
	npu_level->set_curr_level(lmsg_pl->curr_level());
	npu_level->set_pre_level(lmsg_pl->pre_level());
	PackInfo pi(NLMSG_PLAYER_UPGRADE_LEVEL, pack_info.guid, npu_level);
	NOTIFY_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);

	return 0;
}

int ManageShort::lmsg_player_is_online(const PackInfo & pack_info)
{
	typed::protocol::lmsg_player_first_online * lmsg_pfo = (typed::protocol::lmsg_player_first_online *)pack_info.msg;

	bool is_first_online = false;
	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	if (NULL == player_bi)
	{
		SHORT_LOG_INFO("player is online, but did not find the brief info, add it again<%llu>\n", pack_info.guid);
		is_first_online = true;
		addNewPlayerShortInfo(pack_info.guid, lmsg_pfo);
		MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NLMSG_PLAYER_IS_FIRST_ONLINE, pack_info.guid, NULL);
		player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	}

	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player_bi, SE_NGUID_CODE);

	uint64 last_login_time = 0;
	ACE_OS::gettimeofday().to_usec(last_login_time);
	player_bi->short_info->set_last_login_time(last_login_time / (1000 * 1000));
	player_bi->is_online = true;
	player_bi->scene_id = lmsg_pfo->map_id();
	player_bi->short_info->set_curr_map_id(lmsg_pfo->map_id());

	POOL_OP_UPDATE(m_plugin_cfg.pool, player_bi->short_info->guid());

	m_plugin_cfg.line_scene->playerLogin(pack_info.guid);

	if (!is_first_online)
	{
		PackInfo pi(NLMSG_PLAYER_IS_ONLINE, pack_info.guid, NULL);
		NOTIFY_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);
	}

	playerCharge(player_bi);

	return 0;
}

int ManageShort::lmsg_player_is_offline(const PackInfo & pack_info)
{
	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player_bi, SD_INTEGRITY_CODE);

	//typed::entity::Coin * coin = (typed::entity::Coin *)pack_info.msg;

	//{
	//	PackInfo * pi = (PackInfo *)&pack_info;
	//	pi->msg = NULL;
	//}

	//POOL_OP_ADD(m_plugin_cfg.pool, coin->guid(), coin, false, coin->player());

	m_plugin_cfg.line_scene->playerLogoff(pack_info.guid);

	uint64 last_login_time = 0;
	ACE_OS::gettimeofday().to_usec(last_login_time);
	player_bi->short_info->set_last_logoff_time(last_login_time / (1000 * 1000));
	player_bi->is_online = false;

	UPDATE_ENTITY_BY_GUID(player_bi->short_info->guid());
	UPDATE_ENTITY_BY_GUID(player_bi->short_info->mail());
	UPDATE_ENTITY_BY_GUID(player_bi->short_info->friends());
	UPDATE_ENTITY_BY_GUID(player_bi->short_info->multi_copy());
	UPDATE_ENTITY_BY_GUID(player_bi->short_info->exchanges());
	UPDATE_ENTITY_BY_GUID(player_bi->short_info->splendid());
	UPDATE_ENTITY_BY_GUID(player_bi->short_info->qqspreadline());

	PackInfo pi(NLMSG_PLAYER_IS_OFFLINE, pack_info.guid, NULL);
	NOTIFY_MSG_TO_PLUGINS(m_plugin_cfg.message, pi);

	return 0;
}

int ManageShort::lmsg_player_is_jumpmap(const PackInfo & pack_info)
{
	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player_bi, SD_INTEGRITY_CODE);

	player_bi->is_jumpmap = true;
	player_bi->short_info->set_curr_map_id(player_bi->scene_id);

	POOL_OP_UPDATE(m_plugin_cfg.pool, player_bi->short_info->guid());

	return 0;
}

int ManageShort::lmsg_player_jumpmap_ready(const PackInfo & pack_info)
{
	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player_bi, SD_INTEGRITY_CODE);

	player_bi->is_jumpmap = false;

	//MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NLMSG_PLAYER_JUMPMAP_READY, pack_info.guid, NULL)
	return 0;
}

int ManageShort::lmsg_reduce_gold_return(const PackInfo & pack_info)
{
	typed::protocol::plmsg_line_reduce_gold * reduce_gold = (typed::protocol::plmsg_line_reduce_gold *)pack_info.msg;	
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(reduce_gold, SD_INTEGRITY_CODE);

	SHORT_LOG_DEBUG("line reduce gold return, player is <%llu>, notify code is <%d>, result is <%d>", pack_info.guid, reduce_gold->notify_opcode(), reduce_gold->result());

	//ManagePlayerMoney * manage_pm = getManagePlayerMoneyInfo(pack_info.guid);
	//SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(manage_pm, SD_INTEGRITY_CODE);

	//typed::protocol::plmsg_line_reduce_gold * src_rg = manage_pm->getReduceGoldInfo(reduce_gold->notify_opcode());
	//SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(src_rg, SD_INTEGRITY_CODE);

	//SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(src_rg->money() == reduce_gold->money(), SD_INTEGRITY_CODE);
	//SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(src_rg->coin() == reduce_gold->coin(), SD_INTEGRITY_CODE);
	//SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(src_rg->seniority() == reduce_gold->seniority(), SD_INTEGRITY_CODE);

	typed::protocol::plmsg_line_reduce_gold * reduce_g = new typed::protocol::plmsg_line_reduce_gold();
	reduce_g->CopyFrom(*reduce_gold);
	PackInfo * pi = new PackInfo(reduce_gold->notify_opcode(), pack_info.guid, reduce_g);
	m_plugin_cfg.line_scene->scenePackInput(pi);

	//manage_pm->removeReduceRequest(reduce_gold->notify_opcode());
	checkManagePlayerMoneyInfo(pack_info.guid);

	return 0;
}

int ManageShort::lmsg_update_reward_return(const PackInfo & pack_info)
{
	typed::protocol::plmsg_line_update_reward * update_gold = (typed::protocol::plmsg_line_update_reward *)pack_info.msg;	
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(update_gold, SD_INTEGRITY_CODE);

	SHORT_LOG_DEBUG("line update gold return, player is <%llu>, notify code is <%d>, result is <%d>", pack_info.guid, update_gold->notify_opcode(), update_gold->result());

	ManagePlayerMoney * manage_pm = getManagePlayerMoneyInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(manage_pm, SD_INTEGRITY_CODE);

	typed::protocol::plmsg_line_update_reward * src_rg = manage_pm->getUpdateGoldInfo(update_gold->notify_opcode());
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(src_rg, SD_INTEGRITY_CODE);

	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(src_rg->money() == update_gold->money(), SD_INTEGRITY_CODE);
	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(src_rg->coin() == update_gold->coin(), SD_INTEGRITY_CODE);
	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(src_rg->seniority() == update_gold->seniority(), SD_INTEGRITY_CODE);

	if (update_gold->notify_opcode() != 0)
	{
		typed::protocol::plmsg_line_update_reward * update_g = new typed::protocol::plmsg_line_update_reward();
		update_g->CopyFrom(*update_gold);
		PackInfo * pi = new PackInfo(update_gold->notify_opcode(), pack_info.guid, update_g);
		m_plugin_cfg.line_scene->scenePackInput(pi);
	}

	manage_pm->removeUpdateRequest(update_gold->notify_opcode());
	checkManagePlayerMoneyInfo(pack_info.guid);

	return 0;
}

int ManageShort::lmsg_vip_levleup(const PackInfo & pack_info)
{
	typed::protocol::lmsg_vip_levleup * vip_level_up = (typed::protocol::lmsg_vip_levleup *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(vip_level_up, SD_INTEGRITY_CODE);

	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player_bi, SD_INTEGRITY_CODE);

	player_bi->short_info->set_vip_level(vip_level_up->to_vip_level());

	POOL_OP_UPDATE(m_plugin_cfg.pool, player_bi->short_info->guid());

	collectVipNumber(vip_level_up->from_vip_level(), vip_level_up->to_vip_level());

	typed::protocol::nlmsg_vip_levleup * nv_levelup = new typed::protocol::nlmsg_vip_levleup();
	nv_levelup->set_from_vip_level(vip_level_up->from_vip_level());
	nv_levelup->set_to_vip_level(vip_level_up->to_vip_level());

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NLMSG_VIP_LEVLEUP, pack_info.guid, nv_levelup);

	return 0;
}

int ManageShort::mmsg_member_charge(const PackInfo & pack_info)
{
	typed::protocol::mmsg_member_charge * member_charge = (typed::protocol::mmsg_member_charge *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(member_charge, SE_EPLUGIN_CODE);

	DEF_LOG_INFO("recv player charge, player is <%s>, charge value is <%d>, request index is <%d>", member_charge->role_guid().c_str(), member_charge->point(), member_charge->request_index());

	bool charge_sucess = false;

	uint64 player_guid = _atoi64(member_charge->role_guid().c_str());

	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(player_guid);
	if (NULL != player_bi)
	{
		::typed::entity::Short_ChargeInfo* charge_info = player_bi->short_info->add_charge_info();
		charge_info->set_request_index(member_charge->request_index());
		charge_info->set_charge_value(member_charge->point());
		charge_info->set_charge_time(getCurrentTimeInSec());

		POOL_OP_UPDATE(m_plugin_cfg.pool, player_bi->short_info->guid());

		playerCharge(player_bi);
		charge_sucess = true;
	}
	else
	{
		charge_sucess = false;
		DEF_LOG_ERROR("do not find the player short info while charge, player is <%s>, charge value is <%d>, request index is <%d>", member_charge->role_guid().c_str(), member_charge->point(), member_charge->request_index());
	}

	typed::protocol::smsg_member_charge_confirm charge_confirm;
	charge_confirm.set_request_index(member_charge->request_index());
	charge_confirm.set_account(member_charge->account());
	if (charge_sucess)
	{
		charge_confirm.set_result(1);
	}
	else
	{
		charge_confirm.set_result(2);
	}

	MAKE_NEW_PACKET(ps, SMSG_MEMBER_CHARGE_CONFIRM, 0, charge_confirm.SerializeAsString());

	ps->setOwner(pack_info.owner);

	m_plugin_cfg.message->playerMsg(ps);
	return 0;
}

int ManageShort::mmsg_member_give_gift(const PackInfo & pack_info)
{
	typed::protocol::mmsg_member_give_gift * mem_give_gift = (typed::protocol::mmsg_member_give_gift *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(mem_give_gift, SE_EPLUGIN_CODE);

	DEF_LOG_INFO("recv player member give gift, player is <%s>, gift number is <%d>, gift type is <%d>, request index is <%d>"
		, mem_give_gift->role_guid().c_str(), mem_give_gift->gift_number(), mem_give_gift->gift_type(), mem_give_gift->request_index());

	// todo

	typed::protocol::smsg_member_give_gift smg_give_gift;
	smg_give_gift.set_request_index(mem_give_gift->request_index());
	smg_give_gift.set_line_id(mem_give_gift->line_id());
	smg_give_gift.set_account(mem_give_gift->account());
	smg_give_gift.set_role_guid(mem_give_gift->role_guid());
	smg_give_gift.set_result(1);

	MAKE_NEW_PACKET(ps, SMSG_MEMBER_GIVE_GIFT, 0, smg_give_gift.SerializeAsString());

	ps->setOwner(pack_info.owner);

	m_plugin_cfg.message->playerMsg(ps);

	return 0;
}

int ManageShort::lmsg_player_charge_confirm(const PackInfo & pack_info)
{
	typed::protocol::lmsg_player_charge_confirm * charge_confirm = (typed::protocol::lmsg_player_charge_confirm *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(charge_confirm, SE_EPLUGIN_CODE);

	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player_bi, SD_INTEGRITY_CODE);

	vector<std::pair<uint32, uint32> > left_charge_info;
	for (int i = 0; i < player_bi->short_info->charge_info_size(); ++i)
	{
		typed::entity::Short_ChargeInfo * charge_info = player_bi->short_info->mutable_charge_info(i);
		if (charge_info->request_index() != charge_confirm->request_index())
		{
			left_charge_info.push_back(std::make_pair(charge_info->request_index(), charge_info->charge_value()));
		}
		else
		{
			DEF_LOG_INFO("player charge confirm, player is <%llu>, request index is <%d>, charge value is <%d>", pack_info.guid, charge_confirm->request_index(), charge_confirm->charge_value());
		}
	}

	player_bi->short_info->clear_charge_info();
	for (vector<std::pair<uint32, uint32> >::iterator it = left_charge_info.begin(); it != left_charge_info.end(); ++it)
	{
		typed::entity::Short_ChargeInfo * charge_info = player_bi->short_info->add_charge_info();
		charge_info->set_request_index(it->first);
		charge_info->set_charge_value(it->second);
	}

	POOL_OP_UPDATE(m_plugin_cfg.pool, player_bi->short_info->guid());

	return 0;
}

int ManageShort::lmsg_week_change_for_player(const PackInfo & pack_info)
{
	typed::protocol::lmsg_week_change_for_player * lwc_for_player = (typed::protocol::lmsg_week_change_for_player *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(lwc_for_player, SE_EPLUGIN_CODE);

	typed::protocol::nlmsg_week_change_for_player * nwc_for_player = new typed::protocol::nlmsg_week_change_for_player();
	nwc_for_player->set_online_status(lwc_for_player->online_status());

	PackInfo pi(NLMSG_WEEK_CHANGE_FOR_PLAYER, pack_info.guid, nwc_for_player);
	m_plugin_cfg.message->notifyMsgToPlugins(pi);
	return 0;
}

int ManageShort::lmsg_day_change_for_player(const PackInfo & pack_info)
{
	typed::protocol::lmsg_day_change_for_player * ldc_for_player = (typed::protocol::lmsg_day_change_for_player *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(ldc_for_player, SE_EPLUGIN_CODE);

	typed::protocol::nlmsg_day_change_for_player * ndc_for_player = new typed::protocol::nlmsg_day_change_for_player();
	ndc_for_player->set_online_status(ldc_for_player->online_status());
	PackInfo pi(NLMSG_DAY_CHANGE_FOR_PLAYER, pack_info.guid, ndc_for_player);
	m_plugin_cfg.message->notifyMsgToPlugins(pi);
	return 0;
}

int ManageShort::lmsg_update_player_fight_power(const PackInfo & pack_info)
{
	typed::protocol::lmsg_update_player_fight_power * lup_fight_power = (typed::protocol::lmsg_update_player_fight_power *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(lup_fight_power, SE_EPLUGIN_CODE);

	uint64 short_guid = SWITCH_GUID_TYPE(ET_SHORT, pack_info.guid);
	GET_SHORT_FROM_POOL(m_plugin_cfg.pool, short_guid, short_entity);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(short_entity, SE_NGUID_CODE);

	short_entity->set_fight_power(lup_fight_power->fight_power());

	POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->guid());

	return 0;
}

int ManageShort::stol_update_reward(const PackInfo & pack_info)
{
	typed::protocol::stol_update_reward * stol_ur = (typed::protocol::stol_update_reward *)pack_info.msg;

	uint64 short_guid = SWITCH_GUID_TYPE(ET_SHORT, pack_info.guid);
	GET_SHORT_FROM_POOL(m_plugin_cfg.pool, short_guid, short_entity);

	typed::entity::Short_RewardInfo * short_ri = short_entity->mutable_reward_info();

	if (stol_ur->result() == 1)
	{
		if (short_ri->money() >= stol_ur->money())
		{
			short_ri->set_money(stol_ur->money() - stol_ur->money());
		}
		else
		{
			short_ri->set_money(0);
			SHORT_LOG_ERROR("money");
		}
		
		if (short_ri->coin() >= stol_ur->coin())
		{
			short_ri->set_coin(stol_ur->coin() - stol_ur->coin());
		}
		else
		{
			short_ri->set_coin(0);
			SHORT_LOG_ERROR("coin");
		}

		if (short_ri->seniority() >= stol_ur->seniority())
		{
			short_ri->set_seniority(stol_ur->seniority() - stol_ur->seniority());
		}
		else
		{
			short_ri->set_seniority(0);
			SHORT_LOG_ERROR("seniority");
		}

		if (short_ri->prestige() >= stol_ur->prestige())
		{
			short_ri->set_prestige(stol_ur->prestige() - stol_ur->prestige());
		}
		else
		{
			short_ri->set_prestige(0);
			SHORT_LOG_ERROR("prestige");
		}

		if (short_ri->reverence() >= stol_ur->reverence())
		{
			short_ri->set_reverence(stol_ur->reverence() - stol_ur->reverence());
		}
		else
		{
			short_ri->set_reverence(0);
			SHORT_LOG_ERROR("reverence");
		}

		if (short_ri->energy() >= stol_ur->energy())
		{
			short_ri->set_energy(stol_ur->energy() - stol_ur->energy());
		}
		else
		{
			short_ri->set_energy(0);
			SHORT_LOG_ERROR("energy");
		}

		for (int i = 0; i < stol_ur->item_id_size(); ++i)
		{
			for (int j = 0; j < short_ri->item_id_size(); ++j)
			{
				if (stol_ur->item_id(i) == short_ri->item_id(j))
				{
					// same
					if (short_ri->item_num(j) >= stol_ur->item_num(i))
					{
						short_ri->set_item_num(j, short_ri->item_num(j) - stol_ur->item_num(i));
					}
					else
					{
						short_ri->set_item_num(j, 0);
						SHORT_LOG_ERROR("item num");
					}

					break;
				}
			}
		}

		Uint64Vec_t item_ids;
		Uint32Vec_t item_num;
		Uint32Vec_t item_source_from;
		for (int i = 0; i < short_ri->item_id_size(); ++i)
		{
			if (short_ri->item_num(i) > 0)
			{
				item_ids.push_back(short_ri->item_id(i));
				item_num.push_back(short_ri->item_num(i));
				item_source_from.push_back(short_ri->source_from(i));
			}
		}
		short_ri->clear_item_id();
		short_ri->clear_item_num();
		for (int i = 0; i < item_ids.size(); ++i)
		{
			short_ri->add_item_id(item_ids[i]);
			short_ri->add_item_num(item_num[i]);
			short_ri->add_source_from(item_source_from[i]);
		}
	}
	else
	{
		// not online, do nothing
	}

	POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->guid());

	return 0;
}

int ManageShort::cmsg_lookup_player(const PackInfo & pack_info)
{
	typed::protocol::cmsg_lookup_player * cl_player = (typed::protocol::cmsg_lookup_player *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(cl_player, SE_EPLUGIN_CODE);

	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(cl_player->guid() != pack_info.guid, CE_NCHECK_CODE);

	if (!IS_PLAYER_GUID(cl_player->guid()))
	{
		SHORT_LOG_ERROR("wrong player guid <%llu>", cl_player->guid());
		return SE_NGUID_CODE;
	}

	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(cl_player->guid());
	if (NULL == player_bi)
	{
		lookupCrossPlayerInfo(pack_info.guid, cl_player->guid());
		SHORT_LOG_INFO("lookup cross player info <%llu>", pack_info.guid);
		return 0;
	}

	Uint64Vec_t guids;
	guids.push_back(cl_player->guid());
	m_plugin_cfg.scene_request->requestPlayerFightData(guids, boost::bind(&ManageShort::getPlayerLookupDataBack, this, pack_info.guid, cl_player->guid(), _1, _2));

	return 0;
}

int ManageShort::lmsg_player_online_again(const PackInfo & pack_info)
{
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NLMSG_PLAYER_ONLINE_AGAIN, pack_info.guid, NULL);

	return 0;
}

int ManageShort::lmsg_player_online_dispatch_msg(const PackInfo & pack_info)
{
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NLMSG_PLAYER_ONLINE_DISPATCH_MSG, pack_info.guid, NULL);

	return 0;
}

int ManageShort::emsg_exchange_remote_query(const PackInfo & pack_info)
{
	typed::protocol::emsg_exchange_remote_query * ee_query = (typed::protocol::emsg_exchange_remote_query *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(ee_query, SE_EPLUGIN_CODE);

	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player_bi, SE_NGUID_CODE);

	if (player_bi->is_online)
	{
		PackInfo * pi = new PackInfo(EMSG_EXCHANGE_REMOTE_QUERY, pack_info.guid, cloneMessage(ee_query));
		m_plugin_cfg.line_scene->sendSceneMsg(player_bi->scene_id, pi);
	}
	else
	{
		DEF_LOG_ERROR("exchange code query return, but the player is offline <%llu>\n", pack_info.guid);
	}
	return 0;
}

int ManageShort::emsg_exchange_remote_used(const PackInfo & pack_info)
{
	typed::protocol::emsg_exchange_remote_used * ee_query = (typed::protocol::emsg_exchange_remote_used *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(ee_query, SE_EPLUGIN_CODE);

	DEF_LOG_DEBUG("exchange used return, player is <%llu>\n", pack_info.guid);

	return 0;
}

int ManageShort::lmsg_unlock_town(const PackInfo & pack_info)
{
	typed::protocol::lmsg_unlock_town * lu_town = (typed::protocol::lmsg_unlock_town *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(lu_town, SE_EPLUGIN_CODE);

	typed::protocol::nlmsg_unlock_town * nl_town = new typed::protocol::nlmsg_unlock_town();
	nl_town->set_map_id(lu_town->map_id());
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NLMSG_UNLOCK_TOWN, pack_info.guid, nl_town);

	return 0;
}

int ManageShort::stol_system_unlock_notify(const PackInfo & pack_info)
{
	typed::protocol::stol_system_unlock_notify * ss_unlock_system = (typed::protocol::stol_system_unlock_notify *)pack_info.msg;

	typed::protocol::nlmsg_system_unlock * ns_unlock = new typed::protocol::nlmsg_system_unlock();
	ns_unlock->set_system_id(ss_unlock_system->system_id());

	PackInfo pi(NLMSG_SYSTEM_UNLOCK, pack_info.guid, ns_unlock);
	m_plugin_cfg.message->notifyMsgToPlugins(pi);

	return 0;
}

int ManageShort::smsg_cross_server_register(const PackInfo & pack_info)
{
	typed::protocol::smsg_cross_server_register * scs_register = (typed::protocol::smsg_cross_server_register *)pack_info.msg;

	typed::protocol::nmsg_line_server_connection * nls_conn = new typed::protocol::nmsg_line_server_connection();
	nls_conn->set_line_id(scs_register->line_id());
	nls_conn->set_platform(scs_register->platform());

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_LINE_SERVER_CONNECTION, pack_info.guid, nls_conn);

	m_plugin_cfg.line_scene->deliveryPlayerDataToGS(scs_register->platform(), scs_register->line_id());

	return 0;
}

int ManageShort::smsg_cross_server_disconnect(const PackInfo & pack_info)
{
	typed::protocol::smsg_cross_server_disconnect * scs_disconnect = (typed::protocol::smsg_cross_server_disconnect *)pack_info.msg;

	typed::protocol::nmsg_line_server_disconnection * nls_conn = new typed::protocol::nmsg_line_server_disconnection();
	nls_conn->set_line_id(scs_disconnect->line_id());
	nls_conn->set_platform(scs_disconnect->platform());

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_LINE_SERVER_DISCONNECTION, pack_info.guid, nls_conn);

	return 0;
}

int ManageShort::stol_request_player_data(const PackInfo & pack_info)
{
	typed::protocol::stol_request_player_data * srp_data = (typed::protocol::stol_request_player_data *)pack_info.msg;

	uint64 coin_guid = SWITCH_GUID_TYPE(ET_COIN, pack_info.guid);

	GET_COIN_FROM_POOL(m_plugin_cfg.pool, coin_guid, coin);
	SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(coin, );

	if (NULL == coin)
	{
		// still online at the other map
		PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
		if (NULL != player_bi)
		{
			if (player_bi->is_jumpmap)
			{
				SHORT_LOG_ERROR("player is jump map while clean gs unnormal player, map is <%d>, player is <%llu>", player_bi->scene_id, pack_info.guid);
				typed::protocol::gmsg_player_jump_map * gmsg_jump_map = new typed::protocol::gmsg_player_jump_map();
				gmsg_jump_map->set_map_id(player_bi->scene_id);
				gmsg_jump_map->set_line_no(m_plugin_cfg.line_no);
				PackInfo * pi = new PackInfo(GMSG_PLAYER_JUMP_MAP, pack_info.guid, gmsg_jump_map);
				m_plugin_cfg.line_scene->sendSceneMsg(player_bi->scene_id, pi);
			}
		}

		typed::protocol::stol_request_player_data * srpd = (typed::protocol::stol_request_player_data *)cloneMessage(srp_data);
		PackInfo * pi = new PackInfo(LTOS_INNER_REQUEST_LOGOFF, pack_info.guid, srpd);
		m_plugin_cfg.line_scene->broadCastSceneMsg(pi);

		loadCoinFromDb(pack_info.guid, srp_data->map_id());
	}
	else
	{
		typed::entity::Coin * clone_coin = (typed::entity::Coin *)cloneMessage(coin);
		PackInfo * pi = new PackInfo(LTOS_DELIVERY_PLAYER_DATA, coin->player(), clone_coin);
		m_plugin_cfg.line_scene->sendSceneMsg(srp_data->map_id(), pi);

		POOL_OP_REMOVE(m_plugin_cfg.pool, coin_guid, false);
	}

	return 0;
}

int ManageShort::atol_qualifybattle_champion(const PackInfo & pack_info)
{
	BOOST_AUTO(atol, static_cast<typed::protocol::atol_qualifybattle_champion*>(pack_info.msg));
	uint32 b_type = atol->utype();
	uint64 champoin_guid = atol->champion_guid();

	BOOST_AUTO(nlmsg, new typed::protocol::nlmsg_yinxiong_top);
	nlmsg->set_utype(b_type);
	nlmsg->add_top_player(champoin_guid);
	PackInfo packinfo(static_cast<uint16>(NLMSG_YINXIONG_TOP), 0, nlmsg);
	m_plugin_cfg.message->notifyMsgToPlugins(packinfo);

	return 0;
}

int ManageShort::stol_delivery_player_data(const PackInfo & pack_info)
{
	typed::entity::Coin * coin = (typed::entity::Coin *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, SE_EPLUGIN_CODE);

	{
		PackInfo * pi = (PackInfo *)&pack_info;
		pi->msg = NULL;
	}

	POOL_OP_ADD(m_plugin_cfg.pool, coin->guid(), coin, false, coin->player());

	return 0;
}

int ManageShort::smsg_delivery_cross_player_data_start(const PackInfo & pack_info)
{
	typed::protocol::smsg_cross_deliver_player_data_start * scd_start = (typed::protocol::smsg_cross_deliver_player_data_start *)pack_info.msg;
	// do nothing
	SHORT_LOG_INFO("start to delivery cross player data, platform is <%s>, line id is <%s>, player is <%llu>", scd_start->platform().c_str(), 
		scd_start->line_id().c_str(), scd_start->player_guid());

	return 0;
}

int ManageShort::smsg_delivery_cross_player_data_end(const PackInfo & pack_info)
{
	typed::protocol::smsg_cross_deliver_player_data_end * scd_end = (typed::protocol::smsg_cross_deliver_player_data_end *)pack_info.msg;
	// do nothing
	SHORT_LOG_INFO("end to delivery cross player data, platform is <%s>, line id is <%s>, player is <%llu>", scd_end->platform().c_str(), 
		scd_end->line_id().c_str(), scd_end->player_guid());

	m_plugin_cfg.line_scene->collectCrossPlayerDataFinish(scd_end->line_id(), scd_end->platform(), scd_end->player_guid());

	return 0;
}

int ManageShort::smsg_delivery_cross_player_data(const PackInfo & pack_info)
{
	typed::protocol::smsg_cross_deliver_player_data * scd_data = (typed::protocol::smsg_cross_deliver_player_data *)pack_info.msg;

	SHORT_LOG_INFO("delivery cross player data, platform is <%s>, line id is <%s>, player is <%llu>, entity guid is <%llu>", scd_data->platform().c_str(), 
		scd_data->line_id().c_str(), scd_data->player_guid(), scd_data->entity_guid());

	MSG_TYPE * entity_msg = NULL;
	if (extractEntity(scd_data->entity_guid(), scd_data->entity_stream(), &entity_msg))
	{
		// success
		m_plugin_cfg.line_scene->collectCrossPlayerData(scd_data->line_id(), scd_data->platform(), scd_data->player_guid(), scd_data->entity_guid(), 
			scd_data->entity_stream());
	}
	else
	{
		// failed
		SHORT_LOG_ERROR("error to extract entity while delivery cross player data, platform is <%s>, line id is <%s>, player is <%llu>, entity guid is <%llu>", scd_data->platform().c_str(), 
			scd_data->line_id().c_str(), scd_data->player_guid(), scd_data->entity_guid());
	}

	return 0;
}
int ManageShort::qmsg_reset_client_config(const PackInfo & pack_info)
{
	BOOST_AUTO(qmsg, (typed::protocol::qmsg_reset_client_config*)(pack_info.msg));
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(qmsg, -1);

	m_client_config = qmsg->cfg();

	//发送返回消息
	Packet * pi = new Packet(GMSG_RESET_CLIENT_CONFIG, pack_info.guid, "");
	m_plugin_cfg.line_scene->sendMasterMsg(pi);

	typed::protocol::smsg_client_config smsg;
	smsg.set_config_str(m_client_config);
	MAKE_NEW_PACKET(ps_notify, SMSG_CLIENT_CONFIG, 0, smsg.SerializeAsString());
	m_plugin_cfg.line_scene->broadcastPacketToAllOnlinePlayers(ps_notify);
	return 0;
}
int ManageShort::plmsg_line_reduce_gold(const PackInfo & pack_info)
{
	typed::protocol::plmsg_line_reduce_gold * reduce_gold = (typed::protocol::plmsg_line_reduce_gold *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(reduce_gold, SE_EPLUGIN_CODE);

	SHORT_LOG_DEBUG("line reduce gold, palyer is <%llu>, notify code is <%d>", pack_info.guid, reduce_gold->notify_opcode());

	PlayerBriefInfo * pb_info = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(pb_info, SE_EPLUGIN_CODE);

	if (!pb_info->is_online)
	{
		//reduce_gold->set_result(4);
		//SHORT_LOG_ERROR("line reduce gold, the player is not online <%llu>, notify code is <%d>", pack_info.guid, reduce_gold->notify_opcode());
		//return 0;
		uint32 result = 0;
		if (reduceOfflinePlayerCoin(pack_info.guid, reduce_gold, result))
		{
			// success
			reduce_gold->set_result(result);
			return 0;
		}
		else
		{
			// reduce from scene
		}
	}

	//ManagePlayerMoney * manage_player_money = getManagePlayerMoneyInfo(pack_info.guid);
	//if (NULL != manage_player_money)
	//{
	//	if (!manage_player_money->isAvailableReduceRequest(reduce_gold))
	//	{
	//		reduce_gold->set_result(2);
	//		SHORT_LOG_ERROR("line reduce gold, the request is repeated, the player is <%llu>, notify code is <%d>", pack_info.guid, reduce_gold->notify_opcode());
	//		return 0;
	//	}
	//}
	//else
	//{
	//	manage_player_money = new ManagePlayerMoney();
	//	addManagePlayerMoney(pack_info.guid, manage_player_money);
	//}

	//manage_player_money->addReduceRequest(reduce_gold);

	typed::protocol::plmsg_line_reduce_gold * to_scene_reduce = new typed::protocol::plmsg_line_reduce_gold();
	to_scene_reduce->CopyFrom(*reduce_gold);
	PackInfo * to_s_pi = new PackInfo(LMSG_REDUCE_GOLD, pack_info.guid, to_scene_reduce);
	m_plugin_cfg.line_scene->sendSceneMsg(pb_info->scene_id, to_s_pi);

	reduce_gold->set_result(1);

	return 0;
}

int ManageShort::plmsg_line_update_reward(const PackInfo & pack_info)
{
	typed::protocol::plmsg_line_update_reward * update_gold = (typed::protocol::plmsg_line_update_reward *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(update_gold, SE_EPLUGIN_CODE);

	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(update_gold->money() >= 0, SE_EPLUGIN_CODE);
	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(update_gold->coin() >= 0, SE_EPLUGIN_CODE);
	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(update_gold->seniority() >= 0, SE_EPLUGIN_CODE);
	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(update_gold->prestige() >= 0, SE_EPLUGIN_CODE);
	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(update_gold->reverence() >= 0, SE_EPLUGIN_CODE);
	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(update_gold->energy() >= 0, SE_EPLUGIN_CODE);

	PlayerBriefInfo * pb_info = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(pb_info, SE_EPLUGIN_CODE);

	// record
	if (update_gold->money() > 0)
	{
		m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 221, update_gold->source_from(), pb_info->short_info->player(), 
			pb_info->short_info->name(), pb_info->short_info->level(), update_gold->money());
	}
	if (update_gold->coin() > 0)
	{
		m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 261, update_gold->source_from(), pb_info->short_info->player(), 
			pb_info->short_info->name(), pb_info->short_info->level(), update_gold->coin());
	}
	if (update_gold->seniority() > 0)
	{
		m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 231, update_gold->source_from(), pb_info->short_info->player(), 
			pb_info->short_info->name(), pb_info->short_info->level(), update_gold->seniority());
	}
	if (update_gold->prestige() > 0)
	{
		m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 241, update_gold->source_from(), pb_info->short_info->player(), 
			pb_info->short_info->name(), pb_info->short_info->level(), update_gold->prestige());
	}
	if (update_gold->energy() > 0)
	{
		m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 251, update_gold->source_from(), pb_info->short_info->player(), 
			pb_info->short_info->name(), pb_info->short_info->level(), update_gold->energy());
	}

	typed::entity::Short * short_entity = pb_info->short_info;

	// save reward
	::typed::entity::Short_RewardInfo & short_reward = *short_entity->mutable_reward_info();
	short_reward.set_money(short_reward.money() + update_gold->money());
	short_reward.set_coin(short_reward.coin() + update_gold->coin());
	short_reward.set_seniority(short_reward.seniority() + update_gold->seniority());
	short_reward.set_prestige(short_reward.prestige() + update_gold->prestige());
	short_reward.set_reverence(short_reward.reverence() + update_gold->reverence());
	short_reward.set_energy(short_reward.energy() + update_gold->energy());

	// todo
	if (pb_info->is_online)
	{
		typed::protocol::ltos_update_reward * lu_reward = new typed::protocol::ltos_update_reward();
		lu_reward->set_money(update_gold->money());
		lu_reward->set_coin(update_gold->coin());
		lu_reward->set_seniority(update_gold->seniority());
		lu_reward->set_prestige(update_gold->prestige());
		lu_reward->set_reverence(update_gold->reverence());
		lu_reward->set_energy(update_gold->energy());

		PackInfo * pi = new PackInfo(LTOS_UPDATE_REWARD, pack_info.guid, lu_reward);
		m_plugin_cfg.line_scene->sendSceneMsg(pb_info->scene_id, pi);
	}
	else
	{
		// do nothing
	}

	POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->guid());

	return 0;
}

//int ManageShort::plmsg_line_update_reward(const PackInfo & pack_info)
//{
//	typed::protocol::plmsg_line_update_reward * update_gold = (typed::protocol::plmsg_line_update_reward *)pack_info.msg;
//	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(update_gold, SE_EPLUGIN_CODE);
//
//	SHORT_LOG_DEBUG("line update reward, palyer is <%llu>, notify code is <%d>", pack_info.guid, update_gold->notify_opcode());
//
//	PlayerBriefInfo * pb_info = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
//	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(pb_info, SE_EPLUGIN_CODE);
//
//	// todo
//	if (!pb_info->is_online)
//	{
//		update_gold->set_result(4);
//		SHORT_LOG_ERROR("line update reward, palyer is not online <%llu>, notify code is <%d>", pack_info.guid, update_gold->notify_opcode());
//		return 0;
//	}
//
//	ManagePlayerMoney * manage_player_money = getManagePlayerMoneyInfo(pack_info.guid);
//	if (NULL != manage_player_money)
//	{
//		if (!manage_player_money->isAvailableUpdateRequest(update_gold))
//		{
//			update_gold->set_result(2);
//			SHORT_LOG_ERROR("line update reward, request is repated, player is <%llu>, notify code is <%d>", pack_info.guid, update_gold->notify_opcode());
//			return 0;
//		}
//	}
//	else
//	{
//		manage_player_money = new ManagePlayerMoney();
//		addManagePlayerMoney(pack_info.guid, manage_player_money);
//	}
//
//	manage_player_money->addUpdateRequest(update_gold);
//
//	typed::protocol::plmsg_line_update_reward * to_scene_update = new typed::protocol::plmsg_line_update_reward();
//	to_scene_update->CopyFrom(*update_gold);
//	PackInfo * to_s_pi = new PackInfo(LMSG_UPDATE_REWARD, pack_info.guid, to_scene_update);
//	if (m_plugin_cfg.line_scene->sendSceneMsg(pb_info->scene_id, to_s_pi))
//	{
//		update_gold->set_result(1);
//	}
//	else 
//	{
//		// todo
//		update_gold->set_result(4);
//	}
//	
//	return 0;
//}

int ManageShort::plmsg_pick_up(const PackInfo & pack_info)
{
	typed::protocol::plmsg_pick_up * pp_up = (typed::protocol::plmsg_pick_up *)pack_info.msg;
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(pp_up, SE_EPLUGIN_CODE);

	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player_bi, SE_NGUID_CODE);

	typed::entity::Short * short_entity = player_bi->short_info;

	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(pp_up->item_id_size() == pp_up->item_num_size(), SE_EPLUGIN_CODE);

	typed::entity::Short_RewardInfo * short_reward = short_entity->mutable_reward_info();

	typed::protocol::ltos_update_reward * lu_reward = new typed::protocol::ltos_update_reward();

	lu_reward->set_source_from(pp_up->source_from());
	for (int i = 0; i < pp_up->item_id_size(); ++i)
	{
		lu_reward->add_item_id(pp_up->item_id(i));
		lu_reward->add_item_num(pp_up->item_num(i));
		for (int j = 0; j < short_reward->item_id_size(); ++j)
		{
			//if (short_reward->item_id(j) == pp_up->item_id(i))
			//{
			//	short_reward->set_item_num(j, short_reward->item_num(j) + pp_up->item_id(i));
			//	continue;
			//}

			// not find
			short_reward->add_item_id(pp_up->item_id(i));
			short_reward->add_item_num(pp_up->item_num(i));
			short_reward->add_source_from(pp_up->source_from());
		}
	}

	lu_reward->set_money(0);
	lu_reward->set_coin(0);
	lu_reward->set_seniority(0);
	lu_reward->set_prestige(0);
	lu_reward->set_reverence(0);
	lu_reward->set_energy(0);

	PackInfo * pi = new PackInfo(LTOS_UPDATE_REWARD, pack_info.guid, lu_reward);
	m_plugin_cfg.line_scene->sendSceneMsg(player_bi->scene_id, pi);

	POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->guid());
	return 0;
}

int ManageShort::plmsg_query_vip_number(const PackInfo & pack_info)
{
	typed::protocol::plmsg_query_vip_number * pqv_number = (typed::protocol::plmsg_query_vip_number *)pack_info.msg;

	uint64 guid = MAKE_GUID(ET_SHORTPLAYERS, 1, SHORT_PLAYER_START_INDEX);
	GET_SHORTPLAYERS_FROM_POOL(m_plugin_cfg.pool, guid, short_player);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(short_player, SE_NGUID_CODE);

	for (int i = 0; i < short_player->vip_number_size(); ++i)
	{
		typed::entity::ShortPlayers_VipNumber * vn = short_player->mutable_vip_number(i);
		if (vn->vip_number() == 0)
		{
			continue;
		}

		typed::protocol::plmsg_query_vip_number_VipNumber * vip_num = pqv_number->add_vip_number();
		vip_num->set_vip_level(vn->vip_level());
		vip_num->set_vip_number(vn->vip_number());
	}

	return 0;
}

int ManageShort::nmsg_scene_inited(const PackInfo & pack_info)
{
	IS_NOT_LINE_SCENE_RETURN();

	if (m_plugin_cfg.is_first_launch)
	{
		initContainer();
		initPlayerShortInfo();
	}
	else
	{
		loadContainerContainer();
		//loadPlayerShortInfo();
	}
	return 0;
}

int ManageShort::nmsg_flush_all_entity(const PackInfo & pack_info)
{
	// 
	m_is_shutdown = true;

	if (IS_LINE_SCENE())
	{
		//m_plugin_cfg.pool->clearUpdateEntity(m_short_owner_guid);
		MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_SHORT_FLUSH_FINISH, 0, NULL);		
		flushAllShortEntity();
		flushAllPlayerShortEntity();
	}

	return 0;
}

int ManageShort::nlmsg_player_is_online(const PackInfo & pack_info)
{
	PlayerBriefInfo * player_bi = m_plugin_cfg.line_scene->getPlayerBriefInfo(pack_info.guid);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player_bi, SE_NGUID_CODE);

	typed::entity::Short * short_entity = player_bi->short_info;

	typed::entity::Short_RewardInfo * short_reward = short_entity->mutable_reward_info();
	if (short_reward->money() > 0
		|| short_reward->coin() > 0
		|| short_reward->seniority() > 0
		|| short_reward->prestige() > 0
		|| short_reward->reverence() > 0
		|| short_reward->energy() > 0
		|| short_reward->item_id_size() > 0
		)
	{
		typed::protocol::ltos_update_reward * lu_reward = new typed::protocol::ltos_update_reward();
		lu_reward->set_money(short_reward->money());
		lu_reward->set_coin(short_reward->coin());
		lu_reward->set_seniority(short_reward->seniority());
		lu_reward->set_prestige(short_reward->prestige());
		lu_reward->set_reverence(short_reward->reverence());
		lu_reward->set_energy(short_reward->energy());
		lu_reward->set_source_from(0);

		PackInfo * pi = new PackInfo(LTOS_UPDATE_REWARD, pack_info.guid, lu_reward);
		m_plugin_cfg.line_scene->sendSceneMsg(player_bi->scene_id, pi);
	}

	if (short_reward->item_id_size() > 0)
	{
		typed::protocol::ltos_update_reward * lu_reward = new typed::protocol::ltos_update_reward();
		lu_reward->set_money(0);
		lu_reward->set_coin(0);
		lu_reward->set_seniority(0);
		lu_reward->set_prestige(0);
		lu_reward->set_reverence(0);
		lu_reward->set_energy(0);

		for (int i = 0; i < short_reward->item_id_size(); ++i)
		{
			lu_reward->clear_item_id();
			lu_reward->clear_item_num();

			lu_reward->add_item_id(short_reward->item_id(i));
			lu_reward->add_item_num(short_reward->item_num(i));
			lu_reward->set_source_from(short_reward->source_from(i));

			PackInfo * pi = new PackInfo(LTOS_UPDATE_REWARD, pack_info.guid, lu_reward);
			m_plugin_cfg.line_scene->sendSceneMsg(player_bi->scene_id, pi);
		}
	}

	return 0;
}

int ManageShort::nlmsg_player_online_dispatch_msg(const PackInfo & pack_info)
{
	typed::protocol::smsg_client_config smsg;
	smsg.set_config_str(m_client_config);
	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_CLIENT_CONFIG, pack_info.guid, smsg.SerializeAsString());
	return 0;
}
int ManageShort::nlmsg_player_is_offline(const PackInfo & pack_info)
{

	return 0;
}

int ManageShort::nlmsg_player_upgrade_level(const PackInfo & pack_info)
{
	return 0;
}

int ManageShort::nlmsg_player_jumpmap_ready(const PackInfo & pack_info)
{
	return 0;
}

int ManageShort::nmsg_short_init_finish(const PackInfo & pack_info)
{
	// send short init finish to other scene
	PackInfo * pi = new PackInfo(LTOS_SHORT_INIT_FINISH, 0, NULL);
	m_plugin_cfg.line_scene->broadCastSceneMsg(pi);
	return 0;
}

void ManageShort::loadPlayerShortFinish(RequestList * request_list)
{
	bool init_player_result = false;
	if (request_list->is_success())
	{
		init_player_result = true;
		bool short_is_empty = true;

		SHORT_LOG_INFO("ShortPlayers entity is successfully finishing to load");
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			if (NULL != req->message)
			{
				POOL_OP_ADD(m_plugin_cfg.pool, req->guid, req->message, false, m_short_owner_guid);
			}
			else
			{
				init_player_result = false;
				SHORT_LOG_ERROR("failed to get ShortPlayers, the msg is NULL, guid is <%llu>", req->guid);
			}
		}

		int check_short_player = 0;
		m_plugin_cfg.plugin_config.getInt("check_short_player", check_short_player);
		if (1 == check_short_player)
		{
			checkShortPlayer();
		}
	}
	else
	{
		SHORT_LOG_ERROR("it is failed of loading ShortPlayers entity, size is <%d>.", request_list->getRequestList().size());
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			SHORT_LOG_ERROR("loading ShortPlayers entity guid is <%llu>, result is <%d>.", req->guid, req->success);
		}
	}

	if (!init_player_result)
	{
		// todo
		SHORT_LOG_ERROR("fatal error, failed to get ShortPlayers entity.");
	}
	else
	{
		if (!assignAvailableShortPlayers())
		{
			// todo
			SHORT_LOG_ERROR("failed to call assignAvailableShortPlayers");
		}
		else
		{
			loadShortInfo();
		}
	}
}

void ManageShort::loadContainerContainerFinsh(RequestList * request_list)
{
	if (request_list->is_success())
	{
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			if (NULL != req->message)
			{
				POOL_OP_ADD(m_plugin_cfg.pool, req->guid, req->message, false, m_short_owner_guid);
				m_plugin_cfg.manage_container->addContainer(ET_CONTAINER_CONTAINER, req->message);
			}
			else
			{
				SHORT_LOG_ERROR("failed to get ContainerContainer, the msg is NULL, guid is <%llu>", req->guid);
			}
		}
		loadContainer();
	}
	else
	{
		// 
		while (true)
		{
			SHORT_LOG_ERROR("ContainerContainer load failed");
			ACE_OS::sleep(1);
		}
	}
}

void ManageShort::loadContainerFinsh(RequestList * request_list)
{
	if (request_list->is_success())
	{
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			if (NULL != req->message)
			{
				POOL_OP_ADD(m_plugin_cfg.pool, req->guid, req->message, false, m_short_owner_guid);

				//int et_type = EXTRACT_ENTITY_TYPE(req->guid);
				EntityType et_type = (EntityType)(req->guid & 0x000000000000FFFF);

				m_plugin_cfg.manage_container->addContainer((EntityType)et_type, req->message);
			}
			else
			{
				SHORT_LOG_ERROR("failed to get Container, the msg is NULL, guid is <%llu>", req->guid);
			}
		}

		loadContainerEntity();

		if (m_loading_container_set.size() == 0)
		{
			loadPlayerShortInfo();
		}
	}
	else
	{
		SHORT_LOG_ERROR("failed to load Container");
	}
}

void ManageShort::loadContainerEntityFinsh(RequestList * request_list, uint64 container_guid)
{
	EntityType et_type = (EntityType)(container_guid & 0x000000000000FFFF);
	if (request_list->is_success())
	{
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			m_plugin_cfg.manage_container->addContainerEntity(et_type, req->message);
		}

		m_loading_container_set.erase(container_guid);
		if (m_loading_container_set.size() == 0)
		{
			loadPlayerShortInfo();
		}
	}
	else
	{
		SHORT_LOG_ERROR("failed to load container entity, guid is <%llu>", container_guid);
	}
}

#define IS_SHORT_GUID(guid)	(GUID_TYPE(guid) == ET_SHORT)

void ManageShort::loadShortFinish(RequestList * request_list)
{
	uint64 player_guid = 0;
	if (request_list->is_success())
	{
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			SHORT_LOG_DEBUG("finish to load short entity <%llu>, success, pending size <%d>", req->guid, m_loading_short_guid.size());
			if (NULL != req->message)
			{
				player_guid = SWITCH_GUID_TYPE(ET_PLAYER, req->guid);
				POOL_OP_ADD(m_plugin_cfg.pool, req->guid, req->message, false, player_guid);

				if (IS_SHORT_GUID(req->guid))
				{
					m_plugin_cfg.line_scene->addPlayerShortInfo(player_guid, (typed::entity::Short *)req->message, 0, false);

					typed::entity::Short * short_entity = (typed::entity::Short *)req->message;
					if (short_entity->amah() > 0)
					{
						m_active_amah_player_guid.insert(short_entity->player());
					}
					if (short_entity->robot_id() > 0)
					{
						insert_robot(short_entity->player(), short_entity->robot_id());
					}

					insert_openid_to_guid(short_entity->open_id(), short_entity->player());

					loadShortOtherGuid((typed::entity::Short *)req->message);

					removeLoadingShortGuid(req->guid);
				}
			}
			else
			{
				SHORT_LOG_ERROR("failed to get short entity, loading success, but the msg is NULL, guid is <%llu>", req->guid);
			}
		}
	}
	else
	{
		// error
		m_is_load_failed = true;

		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			removeLoadingShortGuid(req->guid);
			SHORT_LOG_ERROR("finish to load short entity <%llu>, failed", req->guid);
		}
	}
}

void ManageShort::loadShortOtherGuidFinish(RequestList * request_list, uint64 player_guid)
{
	if (request_list->is_success())
	{
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;

			removeLoadingShortGuid(req->guid);

			SHORT_LOG_DEBUG("finish to load short other entity <%llu>, success", req->guid);
			if (NULL != req->message)
			{
				POOL_OP_ADD(m_plugin_cfg.pool, req->guid, req->message, false, player_guid);

			}
			else
			{
				SHORT_LOG_ERROR("failed to get short other entity, loading success, but the msg is NULL, guid is <%llu>", req->guid);
			}
		}
	}
	else
	{
		// error
		m_is_load_failed = true;

		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			SHORT_LOG_ERROR("finish to load short other entity <%llu>, failed", req->guid);
		}
	}
}

void ManageShort::update()
{
	if (IS_LINE_SCENE())
	{
		uint64 curr_day = getCurrentDay();
		if (curr_day > m_current_day)
		{
			m_current_day = curr_day;
			increaseLunchDay();
			notifyLineDayChange();
			notifyOtherSceneDayChange();
			checkWeekChange();
		}

		if (m_is_shutdown && (!m_shutdown_success))
		{
			if (m_plugin_cfg.pool->updateEntityNumber() == 0)
			{
				m_shutdown_success = true;
			}
		}

		checkUpdatedContainer();
	}

}

int ManageShort::initing()
{
	m_short_owner_guid = MAKE_GUID(ET_SHORTPLAYERS, 1, 2);
	m_startup_success = false;
	m_shutdown_success = false;

	m_update_timer_id = m_plugin_cfg.scene->schemeTimer(UPDATE_CHECK_TIME, boost::bind(&ManageShort::update, this));

	m_system_res_info = (SystemResInfo *)&m_plugin_cfg.scene->getManageResource()->getSystemResInfo();

	m_current_day = getCurrentDay();

	m_current_week = getCurrentWeek();

	return 0;
}

void ManageShort::initPlayerShortInfo()
{
	uint64 short_guid = 0;
	for (int i = SHORT_PLAYER_START_INDEX; i < SHORT_PLAYER_START_INDEX + SHORT_PLAYER_TOTAL_NUM; ++i)
	{
		short_guid = MAKE_GUID(ET_SHORTPLAYERS, 1, i);
		typed::entity::ShortPlayers * short_players = new typed::entity::ShortPlayers();
		short_players->set_guid(short_guid);
		POOL_OP_ADD(m_plugin_cfg.pool, short_guid, short_players, true, m_short_owner_guid);

		if (NULL == m_available_short_players)
		{
			m_available_short_players = short_players;
		}
	}

	shortLoadFinishNotify();
	//MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_SHORT_INIT_FINISH, 0, NULL);

	m_startup_success = true;
}

void ManageShort::initContainer()
{
	uint64 cc_guid = MAKE_ENTITY_GUID(ET_CONTAINER_CONTAINER, 1, 1);
	typed::entity::ContainerContainer * cc_entity = new typed::entity::ContainerContainer();
	cc_entity->set_guid(cc_guid);
	POOL_OP_ADD(m_plugin_cfg.pool, cc_entity->guid(), cc_entity, true, m_short_owner_guid);

	m_plugin_cfg.manage_container->addContainer(ET_CONTAINER_CONTAINER, cc_entity);
}

void ManageShort::loadPlayerShortInfo()
{
	DECLARE_REQUEST_LIST(m_plugin_cfg.pool);
	for (int i = SHORT_PLAYER_START_INDEX; i < SHORT_PLAYER_START_INDEX + SHORT_PLAYER_TOTAL_NUM; ++i)
	{
		uint64 short_guid = MAKE_GUID(ET_SHORTPLAYERS, 1, i);
		LOAD_REQUEST(short_guid, m_short_owner_guid);
	}

	POOL_OP_COMMIT(m_plugin_cfg.pool, boost::bind(&ManageShort::loadPlayerShortFinish, this, _1));
}

void ManageShort::loadContainerContainer()
{
	DECLARE_REQUEST_LIST(m_plugin_cfg.pool);
	uint64 cc_guid = MAKE_ENTITY_GUID(ET_CONTAINER_CONTAINER, 1, 1);
	LOAD_REQUEST(cc_guid, m_short_owner_guid);
	POOL_OP_COMMIT(m_plugin_cfg.pool, boost::bind(&ManageShort::loadContainerContainerFinsh, this, _1));
}

void ManageShort::loadContainerEntity()
{
	uint64 cc_guid = MAKE_ENTITY_GUID(ET_CONTAINER_CONTAINER, 1, 1);
	GET_CONTAINER_CON_FROM_POOL(m_plugin_cfg.pool, cc_guid, cc_entity);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(cc_entity, );

	for (int i = 0; i < cc_entity->con_guid_size(); ++i)
	{
		uint64 con_guid = cc_entity->con_guid(i);
		GET_CONTAINER_FROM_POOL(m_plugin_cfg.pool, con_guid, con_entity);
		SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(con_entity, );
		loadContainerEntity(con_entity);
	}
}

void ManageShort::loadContainerEntity(typed::entity::Container * con_entity)
{
	if (con_entity->ent_guid_size() == 0)
	{
		return ;
	}

	m_loading_container_set.insert(con_entity->guid());

	DECLARE_REQUEST_LIST(m_plugin_cfg.pool);
	for (int i = 0; i < con_entity->ent_guid_size(); ++i)
	{
		LOAD_REQUEST(con_entity->ent_guid(i), con_entity->ent_guid(i));
	}

	POOL_OP_COMMIT(m_plugin_cfg.pool, boost::bind(&ManageShort::loadContainerEntityFinsh, this, _1, con_entity->guid()));
}

void ManageShort::loadContainer()
{
	uint64 cc_guid = MAKE_ENTITY_GUID(ET_CONTAINER_CONTAINER, 1, 1);
	GET_CONTAINER_CON_FROM_POOL(m_plugin_cfg.pool, cc_guid, cc_entity);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(cc_entity, );
	if (cc_entity->con_guid_size() == 0)
	{
		// todo
		loadPlayerShortInfo();
		return ;
	}

	DECLARE_REQUEST_LIST(m_plugin_cfg.pool);
	for (int i = 0; i < cc_entity->con_guid_size(); ++i)
	{
		uint64 guid = cc_entity->con_guid(i);
		LOAD_REQUEST(guid, m_short_owner_guid);
	}
	POOL_OP_COMMIT(m_plugin_cfg.pool, boost::bind(&ManageShort::loadContainerFinsh, this, _1));
}

void ManageShort::loadShortInfo()
{
	bool short_is_empty = true;
	for (int i = SHORT_PLAYER_START_INDEX; i < SHORT_PLAYER_START_INDEX + SHORT_PLAYER_TOTAL_NUM; ++i)
	{
		uint64 short_guid = MAKE_GUID(ET_SHORTPLAYERS, 1, i);
		GET_SHORTPLAYERS_FROM_POOL(m_plugin_cfg.pool, short_guid, short_players);
		SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(short_players, continue);
		for (int i = 0; i < short_players->player_guids_size(); ++i)
		{
			short_is_empty = false;

			uint64 guid = SWITCH_GUID_TYPE(ET_SHORT, short_players->player_guids(i));
			uint64 player_guid = SWITCH_GUID_TYPE(ET_PLAYER, short_players->player_guids(i));
			DECLARE_REQUEST_LIST(m_plugin_cfg.pool);
			LOAD_REQUEST(guid, player_guid);
			
			uint64 coin_guid = SWITCH_GUID_TYPE(ET_COIN, player_guid);
			LOAD_REQUEST(coin_guid, player_guid);

			POOL_OP_COMMIT(m_plugin_cfg.pool, boost::bind(&ManageShort::loadShortFinish, this, _1));
			collectLoadingShortGuid(guid);
			SHORT_LOG_DEBUG("start to load short entity <%llu>", guid);
			m_all_short_player_set.insert(player_guid);
		}
	}

	if (short_is_empty)
	{
		SHORT_LOG_DEBUG("short is empty, success to init short data");

		shortLoadFinishNotify();
		//MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_SHORT_INIT_FINISH, 0, NULL);

		m_startup_success = true;
	}
}

void ManageShort::loadShortOtherGuid(typed::entity::Short * short_entity)
{
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(short_entity, );

	DECLARE_REQUEST_LIST(m_plugin_cfg.pool);

	uint64 owner_guid = short_entity->player();

	// todo
	//if (short_entity->multi_copy() != 0)
	//{
	//	LOAD_REQUEST(short_entity->multi_copy(), m_short_owner_guid);
	//}

	if (short_entity->mail() != 0)
	{
		LOAD_REQUEST(short_entity->mail(), owner_guid);
		collectLoadingShortGuid(short_entity->mail());
	}

	if (short_entity->multi_copy() != 0)
	{
		LOAD_REQUEST(short_entity->multi_copy(), owner_guid);
		collectLoadingShortGuid(short_entity->multi_copy());
	}

	if (short_entity->qqspreadline() != 0)
	{
		LOAD_REQUEST(short_entity->qqspreadline(), owner_guid);
		collectLoadingShortGuid(short_entity->qqspreadline());
	}

	if (short_entity->friends() != 0)
	{
		LOAD_REQUEST(short_entity->friends(), owner_guid);
		collectLoadingShortGuid(short_entity->friends());
	}

	if (short_entity->escort() != 0)
	{
		LOAD_REQUEST(short_entity->escort(), owner_guid);
		collectLoadingShortGuid(short_entity->escort());
	}

	if (short_entity->arrest_guid() != 0)
	{
		LOAD_REQUEST(short_entity->arrest_guid(), owner_guid);
		collectLoadingShortGuid(short_entity->arrest_guid());
	}

	// 切磋系统自己加载
	//if (short_entity->exchanges() != 0)
	//{
	//	LOAD_REQUEST(short_entity->exchanges(), owner_guid);
	//	collectLoadingShortGuid(short_entity->exchanges());
	//}

	if (short_entity->splendid() != 0)
	{
		LOAD_REQUEST(short_entity->splendid(), owner_guid);
		collectLoadingShortGuid(short_entity->splendid());
	}

	if (short_entity->openact1() != 0)
	{
		LOAD_REQUEST(short_entity->openact1(), owner_guid);
		collectLoadingShortGuid(short_entity->openact1());
	}
	if (short_entity->jianzhong() != 0)
	{
		LOAD_REQUEST(short_entity->jianzhong(), owner_guid);
		collectLoadingShortGuid(short_entity->jianzhong());
	}
	if (short_entity->pair_self() != 0)
	{
		LOAD_REQUEST(short_entity->pair_self(), owner_guid);
		collectLoadingShortGuid(short_entity->pair_self());
	}
	
	POOL_OP_COMMIT(m_plugin_cfg.pool, boost::bind(&ManageShort::loadShortOtherGuidFinish, this, _1, owner_guid));

}

bool ManageShort::assignAvailableShortPlayers()
{
	if ((NULL != m_available_short_players) && (m_available_short_players->player_guids_size() < ONE_SHORT_PLAYER_CONTAIN_NUM))
	{
		return true;
	}

	bool result = false;
	for (int i = SHORT_PLAYER_START_INDEX; i < SHORT_PLAYER_START_INDEX + SHORT_PLAYER_TOTAL_NUM; ++i)
	{
		uint64 short_guid = MAKE_GUID(ET_SHORTPLAYERS, 1, i);
		GET_SHORTPLAYERS_FROM_POOL(m_plugin_cfg.pool, short_guid, short_players);
		if (NULL == short_players)
		{
			SHORT_LOG_ERROR("failed to get ShortPlayers from pool, guid is <%llu>", short_guid);
			break;
		}

		if (short_players->player_guids_size() < ONE_SHORT_PLAYER_CONTAIN_NUM)
		{
			m_available_short_players = short_players;
			result = true;
			break;
		}
	}

	return result;
}

void ManageShort::collectLoadingShortGuid(uint64 short_guid)
{
	Uint64Set_t::iterator it = m_loading_short_guid.find(short_guid);
	if (it != m_loading_short_guid.end())
	{
		// error
		SHORT_LOG_ERROR("loading reduplicate short guid <%llu>", short_guid);
	}
	else
	{
		m_loading_short_guid.insert(short_guid);
	}
}

void ManageShort::removeLoadingShortGuid(uint64 short_guid)
{
	Uint64Set_t::iterator it = m_loading_short_guid.find(short_guid);
	if (it != m_loading_short_guid.end())
	{
		m_loading_short_guid.erase(it);
	}
	else
	{
		SHORT_LOG_ERROR("failed to remove loading short guid, do not find it <%llu>", short_guid);
	}

	if (m_loading_short_guid.size() == 0)
	{
		SHORT_LOG_INFO("short info is finishing to load");
		if (!m_is_load_failed)
		{
			shortLoadFinishNotify();
			//MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_SHORT_INIT_FINISH, 0, NULL);
			m_startup_success = true;
		}
		else
		{
			m_startup_success = false;
		}

		// todo
		m_startup_success = true;
	}
}

void ManageShort::flushAllShortEntity()
{
	// only update
	m_flush_short_total  = 0;
	m_curr_flush_short_num = 0;

	for (int i = SHORT_PLAYER_START_INDEX; i < SHORT_PLAYER_START_INDEX + SHORT_PLAYER_TOTAL_NUM; ++i)
	{
		uint64 short_guid = MAKE_GUID(ET_SHORTPLAYERS, 1, i);
		GET_SHORTPLAYERS_FROM_POOL(m_plugin_cfg.pool, short_guid, short_players);
		SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(short_players, continue);

		for (int j = 0; j < short_players->player_guids_size(); ++j)
		{
			uint64 guid = SWITCH_GUID_TYPE(ET_SHORT, short_players->player_guids(j));

			GET_SHORT_FROM_POOL(m_plugin_cfg.pool, guid, short_entity);
			SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(short_entity, continue);

			POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->guid());
			UPDATE_ENTITY_BY_GUID(short_entity->multi_copy());
			UPDATE_ENTITY_BY_GUID(short_entity->mail());
			UPDATE_ENTITY_BY_GUID(short_entity->friends());
			UPDATE_ENTITY_BY_GUID(short_entity->escort());
			UPDATE_ENTITY_BY_GUID(short_entity->exchanges());
			UPDATE_ENTITY_BY_GUID(short_entity->splendid());
			UPDATE_ENTITY_BY_GUID(short_entity->openact1());
			UPDATE_ENTITY_BY_GUID(short_entity->qqspreadline());
			//DECLARE_REQUEST_LIST(m_plugin_cfg.pool);
			//FLUSH_REQUEST(guid, short_entity, m_short_owner_guid);
			//POOL_OP_COMMIT(m_plugin_cfg.pool, boost::bind(&ManageShort::flushShortFinish, this, _1));

			m_flush_short_total += 1;
		}
	}
}

void ManageShort::flushAllPlayerShortEntity()
{
	m_curr_flush_shortplayer_num = 0;
	for (int i = SHORT_PLAYER_START_INDEX; i < SHORT_PLAYER_START_INDEX + SHORT_PLAYER_TOTAL_NUM; ++i)
	{
		uint64 short_guid = MAKE_GUID(ET_SHORTPLAYERS, 1, i);
		GET_SHORTPLAYERS_FROM_POOL(m_plugin_cfg.pool, short_guid, short_players);
		SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(short_players, continue);

		POOL_OP_UPDATE(m_plugin_cfg.pool, short_players->guid());
		for (int j = 0; j < short_players->player_guids_size(); ++j)
		{
			GET_SHORT_FROM_POOL(m_plugin_cfg.pool, short_players->player_guids(j), short_entity);
			SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(short_entity, continue);

			POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->guid());
			if (short_entity->mail() != 0)
			{
				POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->mail());
			}

			if (short_entity->multi_copy() != 0)
			{
				POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->multi_copy());
			}
			if (short_entity->qqspreadline() != 0)
			{
				POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->qqspreadline());
			}
			if (short_entity->friends() != 0)
			{
				POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->friends());
			}

			if (short_entity->escort() != 0)
			{
				POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->escort());
			}

			if (short_entity->arrest_guid() != 0)
			{
				POOL_OP_UPDATE(m_plugin_cfg.pool, short_entity->arrest_guid());
			}
		}
		//DECLARE_REQUEST_LIST(m_plugin_cfg.pool);
		//FLUSH_REQUEST(short_guid, short_players, m_short_owner_guid);
		//POOL_OP_COMMIT(m_plugin_cfg.pool, boost::bind(&ManageShort::flushShortPlayersFinish, this, _1));
	}
}

void ManageShort::flushShortFinish(RequestList * request_list)
{
	m_curr_flush_short_num += 1;
	if (request_list->is_success())
	{
		for (RequestList::RequestVec_t::const_iterator req_it = request_list->getRequestList().begin(); req_it !=request_list->getRequestList().end(); ++req_it)
		{
			Request * req = *req_it;
			
			SHORT_LOG_INFO("success to flush short <%llu>", req->guid);
			m_plugin_cfg.pool->remove(req->guid, false, false);
		}
	}
	else
	{
		for (RequestList::RequestVec_t::const_iterator req_it = request_list->getRequestList().begin(); req_it !=request_list->getRequestList().end(); ++req_it)
		{
			Request * req = *req_it;
			//POOL_OP_REMOVE(m_plugin_cfg.pool, req->guid, false);
			SHORT_LOG_ERROR("failed to flush short <%llu>", req->guid);
		}
	}

	checkFlushStatus();

}

void ManageShort::flushShortPlayersFinish(RequestList * request_list)
{
	m_curr_flush_shortplayer_num += 1;
	if (request_list->is_success())
	{
		for (RequestList::RequestVec_t::const_iterator req_it = request_list->getRequestList().begin(); req_it !=request_list->getRequestList().end(); ++req_it)
		{
			Request * req = *req_it;

			SHORT_LOG_INFO("success to flush shortplayer <%llu>", req->guid);
			m_plugin_cfg.pool->remove(req->guid, false, false);
		}
	}
	else
	{
		for (RequestList::RequestVec_t::const_iterator req_it = request_list->getRequestList().begin(); req_it !=request_list->getRequestList().end(); ++req_it)
		{
			Request * req = *req_it;
			//POOL_OP_REMOVE(m_plugin_cfg.pool, req->guid, false);
			SHORT_LOG_ERROR("failed to flush shortplayer <%llu>", req->guid);
		}
	}

	checkFlushStatus();
}

void ManageShort::checkFlushStatus()
{
	SHORT_LOG_INFO("check for short flush, total short <%d>, curr flush short <%d>, curr flush short player <%d>, line is <%d>, map is <%d>", m_flush_short_total, m_curr_flush_short_num, m_curr_flush_shortplayer_num, m_plugin_cfg.line_no, m_plugin_cfg.template_id);

	if ((m_flush_short_total == m_curr_flush_short_num) && (SHORT_PLAYER_TOTAL_NUM == m_curr_flush_shortplayer_num))
	{
		SHORT_LOG_INFO("all short is flushing finish, line is <%d>, map is <%d>", m_plugin_cfg.line_no, m_plugin_cfg.template_id);
		MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_SHORT_FLUSH_FINISH, 0, NULL);
	}
}

ManagePlayerMoney * ManageShort::getManagePlayerMoneyInfo(uint64 player_guid)
{
	ManagePlayerMoneyMap_t::iterator it = m_manage_player_money_map.find(player_guid);
	if (it != m_manage_player_money_map.end())
	{
		return it->second;
	}
	
	return NULL;
}

void ManageShort::addManagePlayerMoney(uint64 player, ManagePlayerMoney * manage_player_money)
{
	m_manage_player_money_map.insert(std::make_pair(player, manage_player_money));
}

void ManageShort::checkManagePlayerMoneyInfo(uint64 player)
{
	ManagePlayerMoneyMap_t::iterator it = m_manage_player_money_map.find(player);
	if (it != m_manage_player_money_map.end())
	{
		if (it->second->isEmpty())
		{
			delete it->second;
			m_manage_player_money_map.erase(it);
		}
	}
}

void ManageShort::increaseLunchDay()
{
	LunchInfo * lunch_info = (LunchInfo *)&m_plugin_cfg.line_scene->getLunchInfo();
	lunch_info->lunch_day += 1;
}

void ManageShort::notifyLineDayChange()
{
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NLMSG_DAY_CHANGE, 0, NULL);
}

void ManageShort::notifyOtherSceneDayChange()
{
	typed::protocol::lmsg_day_change * day_change = new typed::protocol::lmsg_day_change();
	day_change->set_new_day(m_current_day);
	PackInfo * pi = new PackInfo(LMSG_DAY_CHANGE, 0, day_change);
	m_plugin_cfg.line_scene->broadCastSceneMsg(pi);
}

void ManageShort::checkWeekChange()
{
	uint64 curr_week = getCurrentWeek();
	if (curr_week > m_current_week)
	{
		m_current_week = curr_week;
		notifyLineWeekChange();
		notifyOtherSceneWeekChange();
	}
}

void ManageShort::notifyLineWeekChange()
{
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NLMSG_WEEK_CHANGE, 0, NULL);
}

void ManageShort::notifyOtherSceneWeekChange()
{
	typed::protocol::lmsg_week_change * week_change = new typed::protocol::lmsg_week_change();
	week_change->set_new_week(m_current_week);
	PackInfo * pi = new PackInfo(LMSG_WEEK_CHANGE, 0, week_change);
	m_plugin_cfg.line_scene->broadCastSceneMsg(pi);
}

void ManageShort::playerCharge(PlayerBriefInfo * player_bi)
{
	if (player_bi->is_online)
	{
		typed::protocol::lmsg_player_charge * player_charge = NULL;
		PackInfo * pi = NULL;
		for (int i = 0; i < player_bi->short_info->charge_info_size(); ++i)
		{
			player_charge = new typed::protocol::lmsg_player_charge();

			::typed::entity::Short_ChargeInfo* charge_info = player_bi->short_info->mutable_charge_info(i);
			player_charge->set_request_index(charge_info->request_index());
			player_charge->set_charge_value(charge_info->charge_value());
			player_charge->set_charge_time(charge_info->charge_time());

			pi = new PackInfo(LMSG_PLAYER_CHARGE, player_bi->player, player_charge);
			m_plugin_cfg.line_scene->sendSceneMsg(player_bi->scene_id, pi);
		}
	}
}

void ManageShort::addNewPlayerShortInfo(uint64 guid, typed::protocol::lmsg_player_first_online * lmsg_pfo)
{
	uint64 short_guid = SWITCH_GUID_TYPE(ET_SHORT, guid);
	typed::entity::Short * short_info = new typed::entity::Short();
	short_info->set_guid(short_guid);
	short_info->set_name(lmsg_pfo->name());
	short_info->set_level(lmsg_pfo->level());
	short_info->set_sex(lmsg_pfo->sex());
	short_info->set_job(lmsg_pfo->job());
	short_info->set_role_id(lmsg_pfo->role_id());
	short_info->set_player(guid);
	short_info->set_curr_map_id(lmsg_pfo->map_id());
	short_info->set_platform(m_plugin_cfg.server_cfg.platform);
	short_info->set_line_id(atoi(m_plugin_cfg.server_cfg.server_id.c_str()));
	short_info->set_robot_id(lmsg_pfo->robot_id());
	short_info->set_open_id(lmsg_pfo->open_id());
	short_info->set_inviter_guid(get_inviter_guid_by_open_id(lmsg_pfo->open_id_referrer()));
	for (int i = 0; i < 3; ++i)
	{
		short_info->add_boss_id(0);
		short_info->add_boss_ranking(0);
	}

	uint64 last_login_time = 0;
	ACE_OS::gettimeofday().to_usec(last_login_time);
	short_info->set_last_login_time(last_login_time / (1000 * 1000));

	insert_openid_to_guid(short_info->open_id(), short_info->player());

	m_plugin_cfg.line_scene->addPlayerShortInfo(guid, short_info, lmsg_pfo->map_id(), true);

	POOL_OP_ADD(m_plugin_cfg.pool, short_guid, short_info, true, guid);

	if (!isCollectedShort(guid))
	{
		assignAvailableShortPlayers();
		SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(m_available_short_players, );

		m_available_short_players->add_player_guids(guid);
		POOL_OP_UPDATE(m_plugin_cfg.pool, m_available_short_players->guid());

		m_all_short_player_set.insert(guid);
	}
	if (short_info->robot_id() > 0)
	{	
		insert_robot(short_info->player(), short_info->robot_id());
	}
}

void ManageShort::getPlayerLookupDataBack(uint64 request_guid, uint64 lookup_player, uint32 request_id, bool is_success)
{
	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(is_success, );

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, lookup_player, player);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

	sendLookupPlayer(request_guid, player);
	sendLookupRole(request_guid, player);
	sendLookupEquipment(request_guid, player);
	sendLookupRunes(request_guid, player);
	sendLookupGuidName(request_guid, player);
	sendLookupDodge(request_guid, player);
}

void ManageShort::lookupCrossPlayerInfo(uint64 observer, uint64 lookup_player)
{
	map<uint64, MSG_TYPE *> msg_map;
	m_plugin_cfg.line_scene->getCrossPlayerDataInfo(lookup_player, msg_map);
	for (map<uint64, MSG_TYPE *>::iterator it = msg_map.begin(); it != msg_map.end(); ++it)
	{
		m_plugin_cfg.pool->add(it->first, it->second, false, lookup_player);
	}

	getPlayerLookupDataBack(observer, lookup_player, 1, true);

	for (map<uint64, MSG_TYPE *>::iterator it = msg_map.begin(); it != msg_map.end(); ++it)
	{
		m_plugin_cfg.pool->remove(it->first, false, false);
	}
}

void ManageShort::sendLookupPlayer(uint64 request_guid, typed::entity::Player * player)
{
	typed::protocol::smsg_lookup_player sl_player;
	sl_player.set_guid(player->guid());
	sl_player.set_name(player->name());
	sl_player.set_level(player->level());
	sl_player.set_profession(player->profession());
	sl_player.set_sex(player->sex());
	sl_player.set_role_id(player->role_id());
	sl_player.set_prestige(player->prestige());
	for (int i = 0; i < player->roles_size(); ++i)
	{
		sl_player.add_roles(player->roles(i));
	}

	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_LOOKUP_PLAYER, request_guid, sl_player.SerializeAsString());
}

void ManageShort::sendLookupRole(uint64 request_guid, typed::entity::Player * player)
{
	typed::protocol::smsg_create_role create_role;
	for (int i = 0; i < player->roles_size(); ++i)
	{
		if (player->roles(i) < 10)
		{
			continue;
		}

		GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(i), role);
		SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);

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

		if (role->attributes_size() < RAT_ENDING)
		{
			for (int i = role->attributes_size(); i < RAT_ENDING; ++i)
			{
				role->add_attributes(0);
			}
		}

		create_role.clear_attributes();
		create_role.clear_attrs_rank();
		fillRoleAttrRank(create_role.mutable_attrs_rank(), role, player, m_system_res_info);

		create_role.add_attributes(role->attributes(RAT_CURR_HP));
		create_role.add_attributes(role->attributes(RAT_HP));
		create_role.add_attributes(role->attributes(RAT_STR));
		create_role.add_attributes(role->attributes(RAT_AGI));
		create_role.add_attributes(role->attributes(RAT_INT));
		create_role.add_attributes(role->attributes(RAT_STR_ATTACK));
		create_role.add_attributes(role->attributes(RAT_AGI_ATTACK));
		create_role.add_attributes(role->attributes(RAT_INT_ATTACK));
		create_role.add_attributes(role->attributes(RAT_STR_DEFENSE));
		create_role.add_attributes(role->attributes(RAT_AGI_DEFENSE));
		create_role.add_attributes(role->attributes(RAT_INT_DEFENSE));
		create_role.add_attributes(role->attributes(RAT_CRITICAL));
		create_role.add_attributes(role->attributes(RAT_CRITICAL_MULTIPLE));
		create_role.add_attributes(role->attributes(RAT_BLOCK));
		create_role.add_attributes(role->attributes(RAT_BLOCK_DEFENCE_MULTIPLE));
		create_role.add_attributes(role->attributes(RAT_BLOCK_DAMAGE_MULTIPLE));
		create_role.add_attributes(role->attributes(RAT_DODGE));
		create_role.add_attributes(role->attributes(RAT_HIT));
		create_role.add_attributes(role->attributes(RAT_STRIKE_BACK));
		create_role.add_attributes(role->attributes(RAT_FIGHT_POWER));

		if (role->attributes_size() > RAT_INITIATIVE_ALL + 2)
		{
			create_role.add_attributes(role->attributes(RAT_INITIATIVE_ALL));
			create_role.add_attributes(role->attributes(RAT_INITIATIVE_COLOR));
			create_role.add_attributes(role->attributes(RAT_INITIATIVE_STHEN));
			create_role.add_attributes(role->attributes(RAT_INITIATIVE_LAW));
			create_role.add_attributes(role->attributes(RAT_INITIATIVE_EDUCATE));
			create_role.add_attributes(role->attributes(RAT_INITIATIVE_DODGE));
			create_role.add_attributes(role->attributes(RAT_INITIATIVE_CHAKRE));
			create_role.add_attributes(role->attributes(RAT_INITIATIVE_TRAINING));
			create_role.add_attributes(role->attributes(RAT_INITIATIVE_LEVEL));		
		}
		else
		{
			create_role.add_attributes(0);
			create_role.add_attributes(0);
			create_role.add_attributes(0);
			create_role.add_attributes(0);
			create_role.add_attributes(0);
			create_role.add_attributes(0);
			create_role.add_attributes(0);
			create_role.add_attributes(0);
			create_role.add_attributes(0);
		}

		MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_LOOKUP_CREATE_ROLE, request_guid, create_role.SerializeAsString());
	}
}

void ManageShort::sendLookupEquipment(uint64 request_guid, typed::entity::Player * player)
{
	typed::protocol::smsg_create_item create_item;

	for (int i = 0; i < player->roles_size(); ++i)
	{
		if (player->roles(i) < 10)
		{
			continue;
		}

		GET_ROLE_FROM_POOL(m_plugin_cfg.pool, player->roles(i), role);
		SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);
		
		for (int j = 0; j < role->equipments_size(); ++j)
		{
			if (role->equipments(j) < 10)
			{
				continue;
			}

			GET_ITEM_FROM_POOL(m_plugin_cfg.pool, role->equipments(j), item);
			SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);

			create_item.clear_attribute_id();
			create_item.clear_attribute_value();

			fillClientItemContent(item, &create_item);

			MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_LOOKUP_CRATE_ITEM, request_guid, create_item.SerializeAsString());
		}
	}
}

void ManageShort::sendLookupRunes(uint64 request_guid, typed::entity::Player * player)
{
	GET_RUNES_FROM_POOL(m_plugin_cfg.pool, player->runes(), runes);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(runes, );

	typed::protocol::smsg_open_runes so_runes;
	so_runes.set_player_guid(request_guid);
	for (int i = 0; i < runes->formation_list_size(); ++i)
	{
		typed::entity::Runes_Formation * ent_formation = runes->mutable_formation_list(i);
		typed::protocol::smsg_open_runes_Formation * pro_formation = so_runes.add_formation_list();
		pro_formation->set_role_guid(ent_formation->role_guid());

		for (int j = 0; j < ent_formation->unit_list_size(); ++j)
		{
			typed::entity::Runes_Unit * ent_unit = ent_formation->mutable_unit_list(j);
			typed::protocol::smsg_open_runes_Unit * pro_unit = pro_formation->add_unit_list();
			pro_unit->set_position(ent_unit->position());
			pro_unit->set_class_(ent_unit->class_());
			pro_unit->set_level(ent_unit->level());
		}
	}

	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_OPEN_RUNES, request_guid, so_runes.SerializeAsString());
}

void ManageShort::sendLookupGuidName(uint64 request_guid, typed::entity::Player * player)
{
	typed::protocol::smsg_lookup_guild_name slg_name;
	slg_name.set_player_guid(player->guid());
	slg_name.set_guild_name(player->guild_name());

	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_LOOKUP_GUILD_NAME, request_guid, slg_name.SerializeAsString());
}

void ManageShort::sendLookupDodge(uint64 request_guid, typed::entity::Player * player)
{
	GET_DODGE_FROM_POOL(m_plugin_cfg.pool, player->dodge(), dodge);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

	typed::protocol::smsg_lookup_dodge sl_dodge;
	sl_dodge.set_player_guid(player->guid());
	sl_dodge.set_dodge_id(dodge->dodge_id());

	MAKE_AND_SEND_PLAYER_MESSAGE(SMSG_LOOKUP_DODGE, request_guid, sl_dodge.SerializeAsString());
}

void ManageShort::checkShortPlayer()
{
	Uint64Set_t guids;
	for (int i = SHORT_PLAYER_START_INDEX; i < SHORT_PLAYER_START_INDEX + SHORT_PLAYER_TOTAL_NUM; ++i)
	{
		uint64 short_guid = MAKE_GUID(ET_SHORTPLAYERS, 1, i);
		GET_SHORTPLAYERS_FROM_POOL(m_plugin_cfg.pool, short_guid, short_players);
		SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(short_players, continue);

		for (int i  = 0; i < short_players->player_guids_size(); ++i)
		{
			uint64 player_guid = short_players->player_guids(i);
			if (IS_PLAYER_GUID(player_guid))
			{
				guids.insert(player_guid);
			}
		}

		short_players->clear_player_guids();
	}

	int num = 0;
	for (int i = SHORT_PLAYER_START_INDEX; i < SHORT_PLAYER_START_INDEX + SHORT_PLAYER_TOTAL_NUM; ++i)
	{
		uint64 short_guid = MAKE_GUID(ET_SHORTPLAYERS, 1, i);
		GET_SHORTPLAYERS_FROM_POOL(m_plugin_cfg.pool, short_guid, short_players);
		SHORT_CHECK_NULL_POINT_WITH_LOG_CMD(short_players, continue);

		num = 0;
		while(guids.size() > 0)
		{
			if (num >= ONE_SHORT_PLAYER_CONTAIN_NUM)
			{
				break;
			}

			short_players->add_player_guids(*guids.begin());
			guids.erase(guids.begin());

			++num;
		}

		POOL_OP_UPDATE(m_plugin_cfg.pool, short_players->guid());
	}
}

bool ManageShort::isCollectedShort(uint64 player_guid)
{
	Uint64Set_t::iterator it = m_all_short_player_set.find(player_guid);
	if (it != m_all_short_player_set.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ManageShort::shortLoadFinishNotify()
{
	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NMSG_SHORT_INIT_FINISH, 0, NULL);

	typed::protocol::nlmsg_active_amah_player_guid * naap_guid = new typed::protocol::nlmsg_active_amah_player_guid();
	for (Uint64Set_t::iterator it = m_active_amah_player_guid.begin(); it != m_active_amah_player_guid.end(); ++it)
	{
		naap_guid->add_player_guid(*it);
	}

	MAKE_AND_NOTIFY_MSG_TO_PLUGINS(NLMSG_ACTIVE_AMAH_PLAYER_GUID, 0, naap_guid);
}

bool ManageShort::reduceOfflinePlayerCoin(uint64 player_guid, typed::protocol::plmsg_line_reduce_gold * reduce_gold, uint32 & return_code)
{
	uint64 coin_guid = SWITCH_GUID_TYPE(ET_COIN, player_guid);
	GET_COIN_FROM_POOL(m_plugin_cfg.pool, coin_guid, coin);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(coin, false);

	bool can_reduce_coin = false;
	bool can_reduce_money = false;
	if (reduce_gold->coin() >= 0)
	{
		if (coin->coin() >= reduce_gold->coin())
		{
			can_reduce_coin = true;
		}
	}

	if (reduce_gold->money() >= 0)
	{
		if (coin->money() >= reduce_gold->money())
		{
			can_reduce_money = true;
		}
	}

	if (can_reduce_coin && can_reduce_money)
	{
		coin->set_coin(coin->coin() - reduce_gold->coin());
		coin->set_money(coin->money() - reduce_gold->money());
		POOL_OP_UPDATE(m_plugin_cfg.pool, coin->guid());

		if (reduce_gold->coin() > 0)
		{
			m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 262, 9000 + reduce_gold->function_id(), player_guid, reduce_gold->coin(), coin->coin() + reduce_gold->coin(), coin->coin());
		}

		if (reduce_gold->money() > 0)
		{
			m_plugin_cfg.data_record->record(m_plugin_cfg.template_id, 222, reduce_gold->function_id(), player_guid, reduce_gold->money(), coin->money() + reduce_gold->money(), coin->money());
		}
		
		return_code = 2;
	}
	else
	{
		return_code = 3;
	}

	return true;
}

void ManageShort::collectVipNumber(uint32 from_vip_level, uint32 to_vip_level)
{
	uint64 guid = MAKE_GUID(ET_SHORTPLAYERS, 1, SHORT_PLAYER_START_INDEX);
	GET_SHORTPLAYERS_FROM_POOL(m_plugin_cfg.pool, guid, short_player);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(short_player, );

	SHORT_CHECK_BOOLEAN_WITH_LOG_RETURN(from_vip_level >= 0, );

	for (int i = short_player->vip_number_size(); i < VIP_MAX_LEVEL; ++i)
	{
		typed::entity::ShortPlayers_VipNumber * vip_number = short_player->add_vip_number();
		vip_number->set_vip_level(i + 1);
		vip_number->set_vip_number(0);
	}

	for (int i = from_vip_level + 1; i <= to_vip_level; ++i)
	{
		if (i <= short_player->vip_number_size())
		{
			typed::entity::ShortPlayers_VipNumber * vip_number = short_player->mutable_vip_number(i - 1);
			vip_number->set_vip_number(vip_number->vip_number() + 1);
		}
		else
		{
			// error
			SHORT_LOG_ERROR("the vip level is too big <%d>", i);
		}
	}

	POOL_OP_UPDATE(m_plugin_cfg.pool, short_player->guid());
}

void ManageShort::checkUpdatedContainer()
{
	Uint64Set_t update_guid_set = m_plugin_cfg.manage_container->getUpdateGuids();
	for (Uint64Set_t::iterator it = update_guid_set.begin(); it != update_guid_set.end(); ++it)
	{
		POOL_OP_UPDATE(m_plugin_cfg.pool, *it);
	}

	ManageContainer::MsgSet_t msg_set = m_plugin_cfg.manage_container->getAddedContainer();

	for (ManageContainer::MsgSet_t::iterator it = msg_set.begin(); it != msg_set.end(); ++it)
	{
		typed::entity::Container * con = (typed::entity::Container *)*it;
		POOL_OP_ADD(m_plugin_cfg.pool, con->guid(), con, true, m_short_owner_guid);
	}
}

void ManageShort::loadCoinFromDb(uint64 player_guid, uint32 map_id)
{
	uint64 coin_guid = SWITCH_GUID_TYPE(ET_COIN, player_guid);

	DECLARE_REQUEST_LIST(m_plugin_cfg.pool);
	LOAD_REQUEST(coin_guid, player_guid);

	POOL_OP_COMMIT(m_plugin_cfg.pool, boost::bind(&ManageShort::loadPlayerCoinFinish, this, _1, map_id));

}

void ManageShort::loadPlayerCoinFinish(RequestList * request_list, uint32 map_id)
{
	if (request_list->is_success())
	{
		uint64 player_guid = SWITCH_GUID_TYPE(ET_PLAYER, map_id);
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			if (NULL != req->message)
			{
				typed::entity::Coin * clone_coin = (typed::entity::Coin *)cloneMessage(req->message);
				PackInfo * pi = new PackInfo(LTOS_DELIVERY_PLAYER_DATA, player_guid, clone_coin);
				m_plugin_cfg.line_scene->sendSceneMsg(map_id, pi);
			}

			break;
		}
	}
}
void ManageShort::insert_robot(uint64 player_guid, uint32 robot_type)
{
	BOOST_AUTO(iter_type, m_all_robot.find(robot_type));
	if (iter_type == m_all_robot.end())
	{
		m_all_robot.insert(std::make_pair(robot_type, define_unordered_set<uint64>()));
		iter_type = m_all_robot.find(robot_type);
	}
	else
	{

	}
	iter_type->second.insert(player_guid);
}
int ManageShort::plmsg_request_robot_by_type(const PackInfo & pack_info)
{
	typed::protocol::plmsg_request_robot_by_type* plmsg = (typed::protocol::plmsg_request_robot_by_type*)(pack_info.msg);
	SHORT_CHECK_NULL_POINT_WITH_LOG_RETURN(plmsg, -1);

	BOOST_AUTO(iter_type, m_all_robot.find(plmsg->robot_type()));
	if (iter_type == m_all_robot.end())
	{
		return 0;
	}
	else
	{
		define_unordered_set<uint64>& set_by_type = iter_type->second;
		BOOST_AUTO(iter_set, set_by_type.begin());
		for (; iter_set != set_by_type.end(); ++iter_set)
		{
			plmsg->add_robot_set(*iter_set);
		}
	}
	return 0;
}
void ManageShort::insert_openid_to_guid(const string& open_id, uint64 player_guid)
{
	if (open_id.empty())
	{
		return;
	}

	m_open_id_to_guid.insert(std::make_pair(open_id, player_guid));
}
uint64 ManageShort::get_inviter_guid_by_open_id(const string& open_id)
{
	if (open_id.empty())
	{
		return 0;
	}

	BOOST_AUTO(iter, m_open_id_to_guid.find(open_id));
	if (iter == m_open_id_to_guid.end())
	{
		return 0;
	}
	return iter->second;
}