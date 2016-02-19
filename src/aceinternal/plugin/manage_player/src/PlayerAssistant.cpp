
#include <boost/lexical_cast.hpp>
#include "PlayerAssistant.h"
#include "Pool.h"
#include "coredef.h"
#include "ManagePlayer.h"
#include "PlayerFunction.h"
#include "entity/achievement.pb.h"
#include "entity/practice.pb.h"
#include "entity/fish.pb.h"
#include "entity/dodge.pb.h"
#include "entity/wife.pb.h"
#include "entity/activity.pb.h"
#include "entity/pellet.pb.h"
#include "entity/maze.pb.h"
#include "entity/openact.pb.h"
#include "entity/openactivity.pb.h"
#include "entity/runes.pb.h"
#include "entity/shenghuoling.pb.h"
#include "entity/trial.pb.h"
#include "entity/doubleexp.pb.h"
#include "entity/logingift.pb.h"
#include "entity/superpower.pb.h"
#include "entity/goldcard.pb.h"
#include "entity/soul.pb.h"
#include "entity/soulpve.pb.h"
#include "entity/talent.pb.h"
#include "entity/horse.pb.h"
#include "entity/qqspread.pb.h"
#define GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, ENTITY_TYPE, guid, entity_ins)	\
	ENTITY_TYPE * entity_ins = NULL;	\
	{	\
	GOOGLE_MESSAGE_TYPE * msg = NULL;	\
	if (is_flush)	\
		msg = pool->getWhileFlush(guid);	\
	else	\
		msg = pool->get(guid);	\
	if (NULL != msg)	\
		{	\
		entity_ins = dynamic_cast<ENTITY_TYPE *>(msg);	\
		}	\
	}	\
	if (NULL == entity_ins)	\
	{	\
	}	

#define UPDATE_OR_FLUSH(guid, entity_ins, player_guid)	\
	if (is_updating)	\
	{	\
		if (is_in_set(guid, insert_guid))	\
		{	\
			ADD_REQUEST(guid, entity_ins, player_guid);	\
		}	\
		else	\
		{	\
			UPDATE_REQUEST(guid, entity_ins, player_guid);	\
		}	\
	}	\
	else	\
		FLUSH_REQUEST(guid, entity_ins, player_guid);

#define MAKE_ENTITY_GUID_MAP(guid, guid_entity, owner_guid)					entity_guid_map.insert(std::make_pair(guid, guid_entity))


#define GET_PLAYER_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Player, guid, entity_ins)

#define GET_ROLE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Role, guid, entity_ins)

#define GET_FORM_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Form, guid, entity_ins)

#define GET_BUFFER_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Buffer, guid, entity_ins)

#define GET_LAW_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Law, guid, entity_ins)

#define GET_ITEM_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Item, guid, entity_ins)

#define GET_SECT_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Sect, guid, entity_ins)

#define GET_STAGE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Stage, guid, entity_ins)

#define GET_STHENO_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Stheno, guid, entity_ins)

#define GET_ANNOUNCE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Announce, guid, entity_ins)

#define GET_TASK_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Task, guid, entity_ins)

#define GET_TASKBRIEF_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::TasksBrief, guid, entity_ins)

#define GET_TOWNSTAGE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::TownStage, guid, entity_ins)

#define GET_EDUCATE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Educate, guid, entity_ins)

#define GET_EDUCATETIMES_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::EducateTimes, guid, entity_ins)

#define GET_ATTACH_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Attach, guid, entity_ins)

#define GET_PVP_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Pvp, guid, entity_ins)

#define GET_PVPCONTAINER_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::PvpContainer, guid, entity_ins)

#define GET_SPECSHOP_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Specshop, guid, entity_ins)

#define GET_SHORTPLAYERS_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::ShortPlayers, guid, entity_ins)

#define GET_SHORT_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Short, guid, entity_ins)

#define GET_CHAKRA_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Chakra, guid, entity_ins)

#define GET_CHAKRE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Chakre, guid, entity_ins)

#define GET_WORSHIP_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Worship, guid, entity_ins)

#define GET_EGGS_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Eggs, guid, entity_ins)

#define GET_CONDOR_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Condor, guid, entity_ins)

#define GET_BOSS_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Boss, guid, entity_ins)

#define GET_ANSWER_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Answer, guid, entity_ins)

#define GET_ADDICTION_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Addiction, guid, entity_ins)

#define GET_GIFT_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Gift, guid, entity_ins)

#define GET_VIP_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Vip, guid, entity_ins)

#define GET_ACHIEVEMENT_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Achievement, guid, entity_ins)

#define GET_PRACTICE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Practice, guid, entity_ins)

#define GET_DAILYPOINTS_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::DailyPoints, guid, entity_ins)

#define GET_TOWER_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Tower, guid, entity_ins)	

#define GET_TAVERN_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Tavern, guid, entity_ins)	

#define GET_FISH_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Fish, guid, entity_ins)	

#define GET_DODGE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Dodge, guid, entity_ins)	

#define GET_WIFE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Wife, guid, entity_ins)	

#define GET_PLAYERWIFE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::PlayerWife, guid, entity_ins)	

#define GET_ACTIVIES_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Activities, guid, entity_ins)	

#define GET_PELLET_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Pellet, guid, entity_ins)	

#define GET_MAZE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::PlayerMaze, guid, entity_ins)	

#define GET_OPENACTTIVITY_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::OpenActivity, guid, entity_ins)	

#define GET_OPENACT_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::OpenAct, guid, entity_ins)	

#define GET_TRIAL_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Trial, guid, entity_ins)	

#define GET_RUNES_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Runes, guid, entity_ins)	

#define GET_SHLSWITCH_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::ShenghuolingSwitch, guid, entity_ins)

#define GET_SHLOWNER_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::ShenghuolingOwner, guid, entity_ins)

#define GET_DOUBLEEXP_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Doubleexp, guid, entity_ins)

#define GET_LOGINGIFT_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::LoginGift, guid, entity_ins)

#define GET_SUPERPOWER_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)		GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::SuperPower, guid, entity_ins)

#define GET_GOLDCARD_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::GoldCard, guid, entity_ins)

#define GET_SOUL_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Soul, guid, entity_ins)

#define GET_SOULPVE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::soulpve_scene, guid, entity_ins)

#define GET_TALENT_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Talent, guid, entity_ins)

#define GET_HORSE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Horse, guid, entity_ins)

#define GET_QQ_SPREAD_PLAYER_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)	GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::QQSpreadPlayer, guid, entity_ins)

//////////////////////////////////////////////////////////////////////////
//#define GET_PLAYER_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Player, guid, entity_ins)
//
//#define GET_ROLE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Role, guid, entity_ins)
//
//#define GET_FORM_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Form, guid, entity_ins)
//
//#define GET_BUFFER_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Buffer, guid, entity_ins)
//
//#define GET_LAW_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Law, guid, entity_ins)
//
//#define GET_ITEM_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Item, guid, entity_ins)
//
//#define GET_SECT_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Sect, guid, entity_ins)
//
//#define GET_STAGE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Stage, guid, entity_ins)
//
//#define GET_TOWNSTAGE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::TownStage, guid, entity_ins)
//
//#define GET_STHENO_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Stheno, guid, entity_ins)
//
//#define GET_ANNOUNCE_FROM_POOL_WHILE_FLUSH(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL_WHILE_FLUSH(pool, typed::entity::Announce, guid, entity_ins)

STAT_PACKET_IMP(PlayerLoadingData);

bool is_in_set(uint64 guid, define_unordered_set<uint64> & guid_set)
{
	if (guid_set.size() == 0)
	{
		return false;
	}

	define_unordered_set<uint64>::iterator it = guid_set.find(guid);
	if (it != guid_set.end())
		return true;
	else
		return false;
}

PlayerLoadingData::PlayerLoadingData()
: m_player_guid(0)
{
	STAT_PACKET_INC_NEW();
}

PlayerLoadingData::~PlayerLoadingData()
{
	for (EntityMap_t::iterator it = m_entity_map.begin(); it != m_entity_map.end(); ++it)
	{
		delete it->second;
	}
	m_entity_map.clear();
	STAT_PACKET_INC_DEL();
}

void PlayerLoadingData::setPlayerGuid(uint64 guid)
{
	m_player_guid = guid;
}

bool PlayerLoadingData::addEntity(uint64 guid, GOOGLE_MESSAGE_TYPE * entity_msg)
{
	std::pair<EntityMap_t::iterator, bool> insert_result = m_entity_map.insert(std::make_pair(guid, entity_msg));
	return insert_result.second;
}

void PlayerLoadingData::addEntityToPool(Pool * pool)
{
	for (EntityMap_t::iterator it = m_entity_map.begin(); it != m_entity_map.end(); ++it)
	{
		POOL_OP_ADD(pool, it->first, it->second, false, m_player_guid);
	}
}

void PlayerLoadingData::clearEntity()
{
	m_entity_map.clear();
}

GOOGLE_MESSAGE_TYPE * PlayerLoadingData::getEntity(uint64 guid)
{
	EntityMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		return it->second;
	}

	return NULL;
}

PlayerAssistant::PlayerAssistant(ManagePlayer * manage_player)
: m_manage_player(manage_player)
{
}

void PlayerAssistant::load_player_role_equipment(RequestList * request_list, ManagePlayer * manage_player, uint64 player_guid)
{
	PluginCfg m_plugin_cfg = manage_player->getPluginCfg();

	if (request_list->is_success())
	{
		PlayerLoadingData * pld_ins = getPlayerLoadingData(player_guid);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pld_ins, );

		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			if (NULL != req->message)
			{
				//POOL_OP_ADD(manage_player->getPluginCfg().pool, req->guid, req->message, false, player_guid);
				pld_ins->addEntity(req->guid, req->message);
			}
		}

		//pld_ins->addEntityToPool(manage_player->getPluginCfg().pool);
		//pld_ins->clearEntity();
		//removePlayerLoadingData(player_guid);

		//manage_player->playerDataLoadFinish(player_guid, true);
		playerLoadSuccess(player_guid);
	}
	else
	{
		PLAYER_LOG_ERROR("player loading failed <%llu>", player_guid);
		playerLoadFailed(player_guid, request_list);
		//manage_player->playerDataLoadFinish(player_guid, false);
	}
}

void PlayerAssistant::load_player_role(RequestList * request_list, ManagePlayer * manage_player, uint64 player_guid)
{
	PluginCfg m_plugin_cfg = manage_player->getPluginCfg();
	if (request_list->is_success())
	{
		PlayerLoadingData * pld_ins = getPlayerLoadingData(player_guid);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pld_ins, );

		DECLARE_REQUEST_LIST(manage_player->getPluginCfg().pool);
		// todo , add data to pool
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			if (NULL != req->message)
			{
				//POOL_OP_ADD(manage_player->getPluginCfg().pool, req->guid, req->message, false, player_guid);
				pld_ins->addEntity(req->guid, req->message);
			}
		}

		GET_PLAYER_FROM_PLD(player_guid, player);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

		for (int i = 0; i < player->roles_size(); ++i)
		{
			GET_ROLE_FROM_PLD(player->roles(i), role);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);

			for (int j = 0; j < role->equipments_size(); ++j)
			{
				uint64 item_guid = role->equipments(j);
				if (item_guid > 1)
				{
					// load role item
					LOAD_REQUEST(role->equipments(j), player->guid());
				}
			}

			if (role->educate() != 0)
			{
				LOAD_REQUEST(role->educate(), player->guid());
			}

			if (role->pellet() != 0)
			{
				LOAD_REQUEST(role->pellet(), player->guid());
			}

			if (role->shl_owner() != 0)
			{
				LOAD_REQUEST(role->shl_owner(), player->guid());
			}
		}

		for (int i = 0; i < player->leave_team_roles_size(); ++i)
		{
			GET_ROLE_FROM_PLD(player->leave_team_roles(i), role);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);

			for (int j = 0; j < role->equipments_size(); ++j)
			{
				if (role->equipments(j) > 1)
				{
					LOAD_REQUEST(role->equipments(j), player->guid());
				}
			}

			if (role->educate() != 0)
			{
				LOAD_REQUEST(role->educate(), player->guid());
			}

			if (role->pellet() != 0)
			{
				LOAD_REQUEST(role->pellet(), player->guid());
			}

			if (role->shl_owner() != 0)
			{
				LOAD_REQUEST(role->shl_owner(), player->guid());
			}
		}

		GET_TASKBRIEF_FROM_PLD(player->task_brief(), task_brief);
		if (NULL != task_brief)
		{
			for (int i = 0; i < task_brief->unfinished_size(); ++i)
			{
				LOAD_REQUEST(task_brief->unfinished(i), player->guid());
			}
			for (int i = 0; i < task_brief->finished_size(); ++i)
			{
				LOAD_REQUEST(task_brief->finished(i), player->guid());
			}
			for (int i = 0; i < task_brief->failed_size(); ++i)
			{
				LOAD_REQUEST(task_brief->failed(i), player->guid());
			}
		}

		GET_CHAKRA_FROM_PLD(player->chakra(), chakra);
		if (NULL != chakra)
		{
			for (int i = 0; i < chakra->storehouse_size(); ++i)
			{
				if (chakra->storehouse(i) > 1)
				{
					LOAD_REQUEST(chakra->storehouse(i), player->guid());
				}
			}

			for (int i = 0; i < chakra->caches_size(); ++i)
			{
				if (chakra->caches(i) > 1)
				{
					LOAD_REQUEST(chakra->caches(i), player->guid());
				}
			}
		}
		
		//for (int i = 0; i < player->wifes_size(); ++i)
		//{
		//	if (player->wifes(i) == 0)
		//	{
		//		continue;
		//	}

		//	GET_WIFE_FROM_PLD(player->wifes(i), wife);
		//	PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(wife, continue);

		//	for (int j = 0; j < wife->equipments_size(); ++j)
		//	{
		//		if (wife->equipments(j) > 0)
		//		{
		//			LOAD_REQUEST(wife->equipments(j), player->guid());
		//		}
		//	}
		//}

		if (REQUEST_IS_EMPTY())
		{
			DELETE_REQUEST();
			//manage_player->playerDataLoadFinish(player_guid, true);
			playerLoadSuccess(player_guid);
		}
		else
		{
			POOL_OP_COMMIT(manage_player->getPluginCfg().pool, boost::bind(&PlayerAssistant::load_player_role_equipment, this, _1, manage_player, player_guid));
		}
	}
	else
	{
		// error
		//manage_player->playerDataLoadFinish(player_guid, false);
		PLAYER_LOG_ERROR("player loading failed <%llu>", player_guid);
		playerLoadFailed(player_guid, request_list);
	}
}

void PlayerAssistant::load_player_from_db(RequestList * request_list, uint64 player_guid, ManagePlayer * manage_player)
{
	if (request_list->is_success())
	{
		PlayerLoadingData * pld = getPlayerLoadingData(player_guid);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pld, );

		DECLARE_REQUEST_LIST(manage_player->getPluginCfg().pool);
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			if (NULL != req->message)
			{
				typed::entity::Player * player = (typed::entity::Player *)req->message;
				this->load_player(player);
			}
			else
			{
				// error , should never happen
			}
		}
	}
	else
	{
		if ((manage_player->getPluginCfg().cache_type == 0) & request_list->is_all_failed())
		{
			// single terminal module, first enter
			const Request * first_request = * request_list->getRequestList().begin();
			manage_player->initPlayer(first_request->guid);
		}
		else
		{
			// error, should never happen
			PLAYER_LOG_ERROR("failed to load player info, guid is <%llu>", player_guid);
			playerLoadFailed(player_guid, request_list);
			//removePlayerLoadingData(player_guid);
			//m_manage_player->playerDataLoadFinish(player_guid, false);
		}
	}
}

void PlayerAssistant::load_player(typed::entity::Player * player)
{
	PlayerLoadingData * pld = getPlayerLoadingData(player->guid());
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pld, );

	DECLARE_REQUEST_LIST(m_plugin_cfg.pool);

		//POOL_OP_ADD(manage_player->getPluginCfg().pool, req->guid, req->message, false, player_guid);
	pld->addEntity(player->guid(), player);

	if (player->stage() != 0)
		LOAD_REQUEST(player->stage(), player->guid());

	if (0 != player->form())
		LOAD_REQUEST(player->form(), player->guid());

	for (int i = 0; i < player->roles_size(); ++i)
	{
		LOAD_REQUEST(player->roles(i), player->guid());
	}

	for (int i = 0; i < player->leave_team_roles_size(); ++i)
	{
		LOAD_REQUEST(player->leave_team_roles(i), player->guid());
	}

	for (int i = 0; i < player->bag_size(); ++i)
	{
		CHECK_ITEM_GUID(player->bag(i));
		LOAD_REQUEST(player->bag(i), player->guid());
	}

	for (int i = 0; i < player->storehouse_size(); ++i)
	{
		CHECK_ITEM_GUID(player->storehouse(i));
		LOAD_REQUEST(player->storehouse(i), player->guid());
	}

	if (player->law() != 0)
		LOAD_REQUEST(player->law(), player->guid());

	if (player->sect() != 0)
		LOAD_REQUEST(player->sect(), player->guid());

	for (int i = 0; i < player->stheno_idle_list_size(); ++i)
	{
		uint64 guid = player->stheno_idle_list(i);
		if (guid > 1)
		{
			LOAD_REQUEST(guid, player->guid());
		}
	}

	for (int i = 0; i < player->stheno_equip_list_size(); ++i)
	{
		uint64 guid = player->stheno_equip_list(i);
		if (guid > 1)
		{
			LOAD_REQUEST(guid, player->guid());
		}
	}

	if (player->announce() != 0)
		LOAD_REQUEST(player->announce(), player->guid());

	if (player->task_brief() != 0)
		LOAD_REQUEST(player->task_brief(), player->guid());

	for (int i = 0; i < player->town_stage_size(); ++i)
	{
		LOAD_REQUEST(player->town_stage(i), player->guid());
	}

	if (player->educate() != 0)
		LOAD_REQUEST(player->educate(), player->guid());

	if (player->specshop() != 0)
		LOAD_REQUEST(player->specshop(), player->guid());

	if (player->chakra() != 0)
		LOAD_REQUEST(player->chakra(), player->guid());

	if (player->worship() != 0)
		LOAD_REQUEST(player->worship(), player->guid());

	if (player->eggs() != 0)
		LOAD_REQUEST(player->eggs(), player->guid());

	if (player->condor() != 0)
		LOAD_REQUEST(player->condor(), player->guid());

	if (player->boss() != 0)
		LOAD_REQUEST(player->boss(), player->guid());

	if (player->answer() != 0)
		LOAD_REQUEST(player->answer(), player->guid());

	if (player->addiction() != 0)
		LOAD_REQUEST(player->addiction(), player->guid());

	if (player->gift() != 0)
		LOAD_REQUEST(player->gift(), player->guid());

	if (player->vip() != 0)
		LOAD_REQUEST(player->vip(), player->guid());

	if (player->achievement() != 0)
		LOAD_REQUEST(player->achievement(), player->guid());

	if (player->practice() != 0)
		LOAD_REQUEST(player->practice(), player->guid());

	if (player->daily_points() != 0)
		LOAD_REQUEST(player->daily_points(), player->guid());

	if (player->tower() != 0)
		LOAD_REQUEST(player->tower(), player->guid());

	if (player->tavern() != 0)
		LOAD_REQUEST(player->tavern(), player->guid());

	if (player->fish() != 0)
		LOAD_REQUEST(player->fish(), player->guid());

	if (player->dodge() != 0)
		LOAD_REQUEST(player->dodge(), player->guid());

	if (player->player_wife() != 0)
		LOAD_REQUEST(player->player_wife(), player->guid());

	for (int i = 0; i < player->wifes_size(); ++i)
	{
		if (player->wifes(i) > 0)
		{
			LOAD_REQUEST(player->wifes(i), player->guid());
		}
	}

	if (player->player_activies() != 0)
		LOAD_REQUEST(player->player_activies(), player->guid());

	if (player->maze() != 0)
		LOAD_REQUEST(player->maze(), player->guid());

	if (player->open_activity() != 0)
		LOAD_REQUEST(player->open_activity(), player->guid());

	if (player->open_act() != 0)
		LOAD_REQUEST(player->open_act(), player->guid());

	if (player->runes() != 0)
		LOAD_REQUEST(player->runes(), player->guid());

	if (player->trial() != 0)
		LOAD_REQUEST(player->trial(), player->guid());

	if (player->shl_switch() != 0)
		LOAD_REQUEST(player->shl_switch(), player->guid());

	if (player->doubleexp() != 0)
		LOAD_REQUEST(player->doubleexp(), player->guid());

	if (player->login_gift() != 0)
		LOAD_REQUEST(player->login_gift(), player->guid());

	if (player->super_power() != 0)
		LOAD_REQUEST(player->super_power(), player->guid());

	if (player->goldcard() != 0)
		LOAD_REQUEST(player->goldcard(), player->guid());

	if (player->soul() != 0)
		LOAD_REQUEST(player->soul(), player->guid());

	if (player->soul_pve_guid() != 0)
		LOAD_REQUEST(player->soul_pve_guid(), player->guid());

	if (player->talent() != 0)
		LOAD_REQUEST(player->talent(), player->guid());

	if (player->horse() != 0)
		LOAD_REQUEST(player->horse(), player->guid());

	if(player->qqspreadsen() != 0)
		LOAD_REQUEST(player->qqspreadsen(), player->guid());
	//PlayerRelateType player_rt = EXTRACT_PLAYER_RELATE_TYPE(req->guid);
	//switch(player_rt)
	//{
	//case PRT_PLAYER:	// do nothing
	//	break;
	//case PRT_LOCALTION:	// do nothing
	//	break;
	//case PRT_ITEM:
	//	{
	//		ENTITY_DYNAMIC_CAST(typed::entity::RoleItem, role_item, req->message);
	//		for (int i = 0; i < role_item->items_size(); ++i)
	//		{
	//			LOAD_REQUEST(role_item->items(i));
	//		}
	//	}
	//	break;
	//case PRT_SKILL:		/// do nothing
	//	break;
	//default:
	//	break;
	//}

	if (REQUEST_IS_EMPTY())
	{
		// todo memory leak
		DELETE_REQUEST();
		//manage_player->playerDataLoadFinish(player_guid, true);
		playerLoadSuccess(player->guid());
	}
	else
	{
		POOL_OP_COMMIT(m_plugin_cfg.pool, boost::bind(&PlayerAssistant::load_player_role, this, _1, m_manage_player, player->guid()));
	}
}

void PlayerAssistant::load_player(typed::entity::Coin * coin)
{
	uint64 player_guid = coin->player();

	PlayerLoadingData * pld = getPlayerLoadingData(coin->player());
	if (NULL == pld)
	{
		PLAYER_LOG_ERROR("start to load player data but did not find it PlayerLoadingData, guid is <%llu>", coin->player());
		return ;
	}

	pld->addEntity(coin->guid(), coin);

	DECLARE_REQUEST_LIST(m_manage_player->getPluginCfg().pool);
	LOAD_REQUEST(player_guid, player_guid);

	POOL_OP_COMMIT(m_manage_player->getPluginCfg().pool, boost::bind(&PlayerAssistant::load_player_from_db, this, _1, player_guid, m_manage_player));
}

void PlayerAssistant::flush_player(RequestList * request_list, uint64 player_guid, ManagePlayer * manage_player)
{
	PluginCfg & m_plugin_cfg = manage_player->getPluginCfg();

	if (request_list->is_success())
	{
		//POOL_OP_REMOVE(manage_player->getPluginCfg().pool, player_guid);
		//for (int i = PRT_PLAYER; i < PRT_END; ++i)
		//{
		//	POOL_OP_REMOVE(manage_player->getPluginCfg().pool, player_guid + i);
		//}

		manage_player->playerFlushDataFinish(player_guid, true, request_list);
		// success
	}
	else
	{
		// error
		manage_player->playerFlushDataFinish(player_guid, false, request_list);
	}
}

//void PlayerAssistant::update_player(RequestList * request_list, uint64 player_guid, ManagePlayer * manage_player)
//{
//	if (request_list->is_success())
//	{
//		manage_player->playerUpdateDataFinish(player_guid, true);
//	}
//	else
//	{
//		// error
//	}
//}

void PlayerAssistant::init(PluginCfg & plugin_cfg)
{
	m_plugin_cfg = plugin_cfg;
}

void PlayerAssistant::request_load_player(uint64 player_guid)
{
	PlayerLoadingData * pld = getPlayerLoadingData(player_guid);
	if (NULL != pld)
	{
		PLAYER_LOG_ERROR("start to load player data but find it PlayerLoadingData, guid is <%llu>", player_guid);
		return ;
	}

	addPlayerLoadingData(player_guid);

	bool player_entered_world = m_plugin_cfg.line_scene->playerEnteredWorld(player_guid);
	if (player_entered_world)
	{
		typed::protocol::stol_request_player_data * srp_data = new typed::protocol::stol_request_player_data();
		srp_data->set_map_id(m_plugin_cfg.template_id);

		PackInfo * pi = new PackInfo(STOL_REQUEST_PLAYER_DATA, player_guid, srp_data);
		m_plugin_cfg.line_scene->scenePackInput(pi);
	}
	else
	{
		DECLARE_REQUEST_LIST(m_manage_player->getPluginCfg().pool);
		LOAD_REQUEST(player_guid, player_guid);

		POOL_OP_COMMIT(m_manage_player->getPluginCfg().pool, boost::bind(&PlayerAssistant::load_player_from_db, this, _1, player_guid, m_manage_player));
	}

	PLAYER_LOG_INFO("start to load player data, player guid is <%llu>", player_guid);
}

void PlayerAssistant::clearPlayerStatus(uint64 player_guid)
{
	PlayerLoadingDataMap_t::iterator it = m_player_loading_data_map.find(player_guid);
	if (it != m_player_loading_data_map.end())
	{
		delete it->second;
		m_player_loading_data_map.erase(it);
	}
}

//void PlayerAssistant::request_flush_player(uint64 player_guid)
//{
//	DECLARE_REQUEST_LIST(m_manage_player->getPluginCfg().pool);
//
//	GET_ENTITY_FROM_POOL(m_manage_player->getPluginCfg().pool, GOOGLE_MESSAGE_TYPE, player_guid, entity_ins);
//	if (NULL != entity_ins)
//	{
//		FLUSH_REQUEST(player_guid, entity_ins);
//	}
//	else
//	{
//		// error, should never happen
//	}
//
//	//for (int i = PRT_PLAYER; i < PRT_END; ++i)
//	//{
//	//	uint64 guid = player_guid + i;
//	//	GET_ENTITY_FROM_POOL(m_manage_player->getPluginCfg().pool, GOOGLE_MESSAGE_TYPE, guid, entity_ins);
//	//	if (NULL != entity_ins)
//	//	{
//	//		FLUSH_REQUEST(player_guid + i, entity_ins);
//	//	}
//	//	else
//	//	{
//	//		// error, should never happen
//	//	}
//	//}
//
//	POOL_OP_COMMIT(m_manage_player->getPluginCfg().pool, boost::bind(&PlayerAssistant::flush_player, _1, player_guid, m_manage_player));
//
//	POOL_OP_REMOVE(m_manage_player->getPluginCfg().pool, player_guid, false);
//
//	PLAYER_LOG_INFO("start to flush player data, player guid is <%s>\n", boost::lexical_cast<string>(player_guid).c_str());
//}

void PlayerAssistant::request_flush_player(uint64 player_guid, bool is_updating)
{
	PLAYER_LOG_DEBUG("start to flush player <%llu>, is updating <%d>", player_guid, is_updating ? 1 : 0);

	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, player_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

	DECLARE_REQUEST_LIST(m_manage_player->getPluginCfg().pool);

	Uint64Set_t insert_entity_set;
	
	Uint64Set_t remove_entity_set;

	m_plugin_cfg.pool->clearUpdateEntity(player_guid, insert_entity_set, remove_entity_set);

	EntityGuidMap_t entity_guid_map;

	this->getPlayerEntityMap(player_guid, entity_guid_map, true);

	for (Uint64Set_t::iterator it = insert_entity_set.begin(); it != insert_entity_set.end(); ++it)
	{
		EntityGuidMap_t::iterator entity_it = entity_guid_map.find(*it);
		PLAYER_CHECK_BOOLEAN_WITH_LOG_CMD(entity_it != entity_guid_map.end(), continue);

		if (is_updating)
		{
			ADD_REQUEST(entity_it->first, entity_it->second, player_guid);
		}
		else
		{
			ADD_WITH_FLUSH_REQUEST(entity_it->first, entity_it->second, player_guid);
		}
		entity_guid_map.erase(entity_it);
	}

	for (Uint64Set_t::iterator it = remove_entity_set.begin(); it != remove_entity_set.end(); ++it)
	{
		REMOVE_REQUEST(*it, player_guid);
	}

	for (EntityGuidMap_t::iterator it = entity_guid_map.begin(); it != entity_guid_map.end(); ++it)
	{
		if (is_updating)
		{
			UPDATE_REQUEST(it->first, it->second, player_guid);
		}
		else
		{
			FLUSH_REQUEST(it->first, it->second, player_guid);
		}
	}

	UPDATE_REQUEST(player_guid, player, player_guid);

	POOL_OP_COMMIT(m_manage_player->getPluginCfg().pool, boost::bind(&PlayerAssistant::flush_player, this, _1, player_guid, m_manage_player));

	// remove commit entity from update entity of pool , do not need to remove it, process it while update of pool
	//POOL_OP_REMOVE_PENDING_UPDATE(m_plugin_cfg.pool);

	PLAYER_LOG_INFO("start to update player data, player guid is <%llu>", player_guid);
}

bool PlayerAssistant::initFirstEnterPlayer(uint64 player_guid)
{
	MAKE_ENTITY_INSTANCE(typed::entity::Player, role);
	initPlayer(player_guid, role);
	POOL_OP_ADD(m_manage_player->getPluginCfg().pool, player_guid, role, false, player_guid);

	//MAKE_ENTITY_INSTANCE(entity::RoleLocaltion, role_localtion);
	//initRoleLocaltion(player_guid + PRT_LOCALTION, role_localtion);
	//POOL_OP_ADD(m_manage_player->getPluginCfg().pool, player_guid + PRT_LOCALTION, role_localtion);

	//MAKE_ENTITY_INSTANCE(entity::RoleItem, role_item);
	//initRoleItem(player_guid + PRT_ITEM, role_item);
	//POOL_OP_ADD(m_manage_player->getPluginCfg().pool, player_guid + PRT_ITEM, role_item);

	//MAKE_ENTITY_INSTANCE(entity::RoleSkill, role_skill);
	//initRoleSkill(player_guid + PRT_SKILL, role_skill);
	//POOL_OP_ADD(m_manage_player->getPluginCfg().pool, player_guid + PRT_SKILL, role_skill);

	return true;
}

void PlayerAssistant::clearEntityFlushState(uint64 player_guid)
{
	//Pool * pool = m_manage_player->getPluginCfg().pool;
	//POOL_OP_CLEAR_FLUSH_STATE(pool, player_guid);
	//GET_PLAYER_FROM_POOL(pool, player_guid, player);
	//PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

	//for (int i = 0; i < player->roles_size(); ++i)
	//{
	//	POOL_OP_CLEAR_FLUSH_STATE(pool, player->roles(i));
	//	GET_ROLE_FROM_POOL(pool, player->roles(i), role);
	//	PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);
	//	for (int j = 0; j < role->equipments_size(); ++j)
	//	{
	//		if (role->equipments(j) > 1)
	//		{
	//			POOL_OP_CLEAR_FLUSH_STATE(pool, role->equipments(j));
	//		}
	//	}
	//}

	//for (int i = 0; i < player->leave_team_roles_size(); ++i)
	//{
	//	POOL_OP_CLEAR_FLUSH_STATE(pool, player->leave_team_roles(i));
	//	GET_ROLE_FROM_POOL(pool, player->leave_team_roles(i), role);
	//	PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);
	//	for (int j = 0; j < role->equipments_size(); ++j)
	//	{
	//		if (role->equipments(j) > 1)
	//		{
	//			POOL_OP_CLEAR_FLUSH_STATE(pool, role->equipments(j));
	//		}
	//	}
	//}

	//POOL_OP_CLEAR_FLUSH_STATE(pool, player->form());

	//POOL_OP_CLEAR_FLUSH_STATE(pool, player->law());

	//POOL_OP_CLEAR_FLUSH_STATE(pool, player->sect());

	//for (int i = 0; i < player->bag_size(); ++i)
	//{
	//	CHECK_ITEM_GUID(player->bag(i));
	//	POOL_OP_CLEAR_FLUSH_STATE(pool, player->bag(i));
	//}

	//for (int i = 0; i < player->storehouse_size(); ++i)
	//{
	//	CHECK_ITEM_GUID(player->storehouse(i));
	//	POOL_OP_CLEAR_FLUSH_STATE(pool, player->storehouse(i));
	//}

	//for (int i = 0; i < player->stheno_idle_list_size(); ++i)
	//{
	//	uint64 guid = player->stheno_idle_list(i);
	//	if (guid > 1)
	//	{
	//		POOL_OP_CLEAR_FLUSH_STATE(pool, guid);
	//	}
	//}

	//for (int i = 0; i < player->stheno_equip_list_size(); ++i)
	//{
	//	uint64 guid = player->stheno_equip_list(i);
	//	if (guid > 1)
	//	{
	//		POOL_OP_CLEAR_FLUSH_STATE(pool, guid);
	//	}
	//}

	//POOL_OP_CLEAR_FLUSH_STATE(pool, player->stage());

	//POOL_OP_CLEAR_FLUSH_STATE(pool, player->announce());
}

void PlayerAssistant::getPlayerEntityMap(uint64 player_guid, EntityGuidMap_t & entity_guid_map, bool is_flush)
{
	GET_PLAYER_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, player_guid, player);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );

	//MAKE_ENTITY_GUID_MAP(player_guid, player, player_guid);

	// stage
	GET_STAGE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->stage(), stage);
	if (NULL != stage)
	{
		MAKE_ENTITY_GUID_MAP(player->stage(), stage, player_guid);
	}

	// flush form
	GET_FORM_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, player->form(), form);
	if (NULL != form)
	{
		MAKE_ENTITY_GUID_MAP(form->guid(), form, player_guid);
	}

	// flush role
	for (int i = 0; i < player->roles_size(); ++i)
	{
		GET_ROLE_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, player->roles(i), role);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);
		MAKE_ENTITY_GUID_MAP(role->guid(), role, player_guid);

		// flush role equipment
		for (int j = 0; j < role->equipments_size(); ++j)
		{
			if (role->equipments(j) > 1)
			{
				GET_ITEM_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, role->equipments(j), item);
				PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);
				MAKE_ENTITY_GUID_MAP(item->guid(), item, player_guid);
			}
		}

		GET_EDUCATE_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, role->educate(), educate);
		if (NULL != educate)
		{
			MAKE_ENTITY_GUID_MAP(educate->guid(), educate, player_guid);
		}

		GET_PELLET_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, role->pellet(), pellet);
		if (NULL != pellet)
		{
			MAKE_ENTITY_GUID_MAP(pellet->guid(), pellet, player_guid);
		}

		if (0 != role->shl_owner())
		{
			GET_SHLOWNER_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, role->shl_owner(), shl_owner);
			if (NULL != shl_owner)
			{
				MAKE_ENTITY_GUID_MAP(role->shl_owner(), shl_owner, player_guid);
			}
		}

	}

	// flush leave team role
	for (int i = 0; i < player->leave_team_roles_size(); ++i)
	{
		GET_ROLE_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, player->leave_team_roles(i), leave_role);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(leave_role, continue);
		MAKE_ENTITY_GUID_MAP(leave_role->guid(), leave_role, player_guid);

		// flush role equipment
		for (int j = 0; j < leave_role->equipments_size(); ++j)
		{
			if (leave_role->equipments(j) > 1)
			{
				GET_ITEM_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, leave_role->equipments(j), item);
				PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);
				MAKE_ENTITY_GUID_MAP(item->guid(), item, player_guid);
			}
		}

		GET_EDUCATE_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, leave_role->educate(), educate);
		if (NULL != educate)
		{
			MAKE_ENTITY_GUID_MAP(educate->guid(), educate, player_guid);
		}

		GET_PELLET_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, leave_role->pellet(), pellet);
		if (NULL != pellet)
		{
			MAKE_ENTITY_GUID_MAP(pellet->guid(), pellet, player_guid);
		}

		if (0 != leave_role->shl_owner())
		{
			GET_SHLOWNER_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, leave_role->shl_owner(), shl_owner);
			if (NULL != shl_owner)
			{
				MAKE_ENTITY_GUID_MAP(leave_role->shl_owner(), shl_owner, player_guid);
			}
		}

	}

	// bag
	for (int i = 0; i < player->bag_size(); ++i)
	{
		CHECK_ITEM_GUID(player->bag(i));
		GET_ITEM_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, player->bag(i), item);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);
		MAKE_ENTITY_GUID_MAP(item->guid(), item, player_guid);
	}

	// storehouse
	for (int i = 0; i < player->storehouse_size(); ++i)
	{
		CHECK_ITEM_GUID(player->storehouse(i));
		GET_ITEM_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, player->storehouse(i), item);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);
		MAKE_ENTITY_GUID_MAP(item->guid(), item, player_guid);
	}

	// law
	GET_LAW_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, player->law(), law);
	if (NULL != law)
	{
		MAKE_ENTITY_GUID_MAP(law->guid(), law, player_guid);
	}

	// sect
	GET_SECT_FROM_POOL_WHILE_FLUSH(m_manage_player->getPluginCfg().pool, player->sect(), sect);
	if (NULL != sect)
	{
		MAKE_ENTITY_GUID_MAP(sect->guid(), sect, player_guid);
	}

	// stheno idle list
	for (int i = 0; i < player->stheno_idle_list_size(); ++i)
	{
		uint64 guid = player->stheno_idle_list(i);
		if (guid > 1)
		{
			GET_STHENO_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, guid, stheno);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(stheno, continue);
			MAKE_ENTITY_GUID_MAP(guid, stheno, player_guid);
		}
	}

	// equip list
	for (int i = 0; i < player->stheno_equip_list_size(); ++i)
	{
		uint64 guid = player->stheno_equip_list(i);
		if (guid > 1)
		{
			GET_STHENO_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, guid, stheno);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(stheno, continue);
			MAKE_ENTITY_GUID_MAP(guid, stheno, player_guid);
		}
	}

	// accounce
	GET_ANNOUNCE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->announce(), announce);
	if (NULL != announce)
	{
		MAKE_ENTITY_GUID_MAP(player->announce(), announce, player_guid);
	}

	// task brief
	GET_TASKBRIEF_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->task_brief(), task_brief);
	if (NULL != task_brief)
	{
		MAKE_ENTITY_GUID_MAP(player->task_brief(), task_brief, player_guid);

		// task
		for (int i = 0; i < task_brief->unfinished_size(); ++i)
		{
			GET_TASK_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, task_brief->unfinished(i), task_entity);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(task_entity, continue);
			MAKE_ENTITY_GUID_MAP(task_brief->unfinished(i), task_entity, player_guid);
		}

		for (int i = 0; i < task_brief->finished_size(); ++i)
		{
			GET_TASK_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, task_brief->finished(i), task_entity);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(task_entity, continue);
			MAKE_ENTITY_GUID_MAP(task_brief->finished(i), task_entity, player_guid);
		}

		for (int i = 0; i < task_brief->failed_size(); ++i)
		{
			GET_TASK_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, task_brief->failed(i), task_entity);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(task_entity, continue);
			MAKE_ENTITY_GUID_MAP(task_brief->failed(i), task_entity, player_guid);
		}
	}

	// town stage
	for (int i = 0; i < player->town_stage_size(); ++i)
	{
		GET_TOWNSTAGE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->town_stage(i), town_stage);
		if (NULL != town_stage)
		{
			MAKE_ENTITY_GUID_MAP(player->town_stage(i), town_stage, player_guid);
		}
	}

	// educate
	GET_EDUCATETIMES_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->educate(), educate_times);
	if (NULL != educate_times)
	{
		MAKE_ENTITY_GUID_MAP(educate_times->guid(), educate_times, player_guid);
	}

	// spec shop
	GET_SPECSHOP_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->specshop(), spec_shop);
	if (NULL != spec_shop)
	{
		MAKE_ENTITY_GUID_MAP(player->specshop(), spec_shop, player_guid);
	}

	GET_CHAKRA_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->chakra(), chakra);
	if (NULL != chakra)
	{
		MAKE_ENTITY_GUID_MAP(player->chakra(), chakra, player_guid);
		for (int i = 0; i < chakra->storehouse_size(); ++i)
		{
			if (chakra->storehouse(i) > 1)
			{
				GET_CHAKRE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, chakra->storehouse(i), chakre);
				if (NULL != chakre)
				{
					MAKE_ENTITY_GUID_MAP(chakre->guid(), chakre, player_guid);
				}
			}
		}

		for (int i = 0; i < chakra->caches_size(); ++i)
		{
			GET_CHAKRE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, chakra->caches(i), chakre);
			if (NULL != chakre)
			{
				MAKE_ENTITY_GUID_MAP(chakre->guid(), chakre, player_guid);
			}
		}
	}

	GET_WORSHIP_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->worship(), worship);
	if (NULL != worship)
	{
		MAKE_ENTITY_GUID_MAP(player->worship(), worship, player_guid);
	}

	GET_EGGS_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->eggs(), eggs);
	if (NULL != eggs)
	{
		MAKE_ENTITY_GUID_MAP(player->eggs(), eggs, player_guid);
	}

	GET_CONDOR_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->condor(), condor);
	if (NULL != condor)
	{
		MAKE_ENTITY_GUID_MAP(player->condor(), condor, player_guid);
	}

	GET_BOSS_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->boss(), boss);
	if (NULL != boss)
	{
		MAKE_ENTITY_GUID_MAP(player->boss(), boss, player_guid);
	}

	GET_ANSWER_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->answer(), answer);
	if (NULL != answer)
	{
		MAKE_ENTITY_GUID_MAP(player->answer(), answer, player_guid);
	}

	GET_ADDICTION_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->addiction(), addiction);
	if (NULL != addiction)
	{
		MAKE_ENTITY_GUID_MAP(addiction->guid(), addiction, player_guid);
	}

	GET_GIFT_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->gift(), gift);
	if (NULL != gift)
	{
		MAKE_ENTITY_GUID_MAP(gift->guid(), gift, player_guid);
	}

	GET_VIP_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->vip(), vip);
	if (NULL != vip)
	{
		MAKE_ENTITY_GUID_MAP(vip->guid(), vip, player_guid);
	}

	GET_ACHIEVEMENT_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->achievement(), achievement);
	if (NULL != achievement)
	{
		MAKE_ENTITY_GUID_MAP(player->achievement(), achievement, player_guid);
	}

	GET_PRACTICE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->practice(), practice);
	if (NULL != practice)
	{
		MAKE_ENTITY_GUID_MAP(player->practice(), practice, player_guid);
	}

	GET_DAILYPOINTS_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->daily_points(), daily_points);
	if (NULL != daily_points)
	{
		MAKE_ENTITY_GUID_MAP(player->daily_points(), daily_points, player_guid);
	}

	GET_TOWER_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->tower(), tower);
	if (NULL != tower)
	{
		MAKE_ENTITY_GUID_MAP(player->tower(), tower, player_guid);
	}

	GET_TAVERN_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->tavern(), tavern);
	if (NULL != tavern)
	{
		MAKE_ENTITY_GUID_MAP(player->tavern(), tavern, player_guid);
	}

	GET_FISH_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->fish(), fish);
	if (NULL != fish)
	{
		MAKE_ENTITY_GUID_MAP(player->fish(), fish, player_guid);
	}

	GET_DODGE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->dodge(), dodge);
	if (NULL != dodge)
	{
		MAKE_ENTITY_GUID_MAP(player->dodge(), dodge, player_guid);
	}

	GET_PLAYERWIFE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->player_wife(), player_wife);
	if (NULL != player_wife)
	{
		MAKE_ENTITY_GUID_MAP(player->player_wife(), player_wife, player_guid);
	}

	for (int i = 0; i < player->wifes_size(); ++i)
	{
		if (0 == player->wifes(i))
		{
			continue;
		}

		GET_WIFE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->wifes(i), wife);
		PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(wife, continue);
		MAKE_ENTITY_GUID_MAP(wife->guid(), wife, player_guid);

		for (int j = 0; j < wife->equipments_size(); ++j)
		{
			if (wife->equipments(j) == 0)
			{
				continue;
			}

			GET_ITEM_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, wife->equipments(j), wife_equipment);
			PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(wife_equipment, continue);
			MAKE_ENTITY_GUID_MAP(wife_equipment->guid(), wife_equipment, player_guid);
		}
	}

	if (0 != player->player_activies())
	{
		GET_ACTIVIES_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->player_activies(), activies);
		if (NULL != activies)
		{
			MAKE_ENTITY_GUID_MAP(player->player_activies(), activies, player_guid);
		}
	}

	if (0 != player->maze())
	{
		GET_MAZE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->maze(), maze);
		if (NULL != maze)
		{
			MAKE_ENTITY_GUID_MAP(player->maze(), maze, player_guid);
		}
	}

	if (0 != player->open_activity())
	{
		GET_OPENACTTIVITY_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->open_activity(), open_activity);
		if (0 != open_activity)
		{
			MAKE_ENTITY_GUID_MAP(player->open_activity(), open_activity, player_guid);
		}
	}

	if (0 != player->open_act())
	{
		GET_OPENACT_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->open_act(), open_act);
		if (NULL != open_act)
		{
			MAKE_ENTITY_GUID_MAP(player->open_act(), open_act, player_guid);
		}
	}

	if (0 != player->trial())
	{
		GET_TRIAL_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->trial(), trial);
		if (NULL != trial)
		{
			MAKE_ENTITY_GUID_MAP(player->trial(), trial, player_guid);
		}
	}

	if (0 != player->runes())
	{
		GET_RUNES_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->runes(), runes);
		if (NULL != runes)
		{
			MAKE_ENTITY_GUID_MAP(player->runes(), runes, player_guid);
		}
	}

	if (0 != player->shl_switch())
	{
		GET_SHLSWITCH_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->shl_switch(), shl_switch);
		if (NULL != shl_switch)
		{
			MAKE_ENTITY_GUID_MAP(player->shl_switch(), shl_switch, player_guid);
		}
	}

	if (0 != player->doubleexp())
	{
		GET_DOUBLEEXP_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->doubleexp(), doubleexp);
		if (NULL != doubleexp)
		{
			MAKE_ENTITY_GUID_MAP(player->doubleexp(), doubleexp, player_guid);
		}
	}

	if (0 != player->login_gift())
	{
		GET_LOGINGIFT_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->login_gift(), logingift);
		if (NULL != logingift)
		{
			MAKE_ENTITY_GUID_MAP(player->login_gift(), logingift, player_guid);
		}
	}

	if (0 != player->super_power())
	{
		GET_SUPERPOWER_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->super_power(), super_power);
		if (NULL != super_power)
		{
			MAKE_ENTITY_GUID_MAP(player->super_power(), super_power, player_guid);
		}
	}

	if (0 != player->goldcard())
	{
		GET_GOLDCARD_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->goldcard(), goldcard);
		if (NULL != goldcard)
		{
			MAKE_ENTITY_GUID_MAP(player->goldcard(), goldcard, player_guid);
		}
	}

	if (0 != player->soul())
	{
		GET_SOUL_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->soul(), soul);
		if (NULL != soul)
		{
			MAKE_ENTITY_GUID_MAP(player->soul(), soul, player_guid);
		}
	}
	if (0 != player->soul_pve_guid())
	{
		GET_SOULPVE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->soul_pve_guid(), soulpve);
		if (NULL != soulpve)
		{
			MAKE_ENTITY_GUID_MAP(player->soul_pve_guid(), soulpve, player_guid);
		}
	}
	if (0 != player->talent())
	{
		GET_TALENT_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->talent(), talent);
		if (NULL != talent)
		{
			MAKE_ENTITY_GUID_MAP(player->talent(), talent, player_guid);
		}
	}
	if (0 != player->horse())
	{
		GET_HORSE_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->horse(), horse);
		if (NULL != horse)
		{
			MAKE_ENTITY_GUID_MAP(player->horse(), horse, player_guid);
		}
	}
	if (0 != player->qqspreadsen())
	{
		GET_QQ_SPREAD_PLAYER_FROM_POOL_WHILE_FLUSH(m_plugin_cfg.pool, player->qqspreadsen(), qq_spread);
		if (NULL != qq_spread)
		{
			MAKE_ENTITY_GUID_MAP(player->qqspreadsen(), qq_spread, player_guid);
		}
	}
}

void PlayerAssistant::initPlayer(uint64 guid, typed::entity::Player * role)
{
	static int index = 0;
	role->set_guid(guid);
	role->set_name("test_" + boost::lexical_cast<string>(++index));
}

PlayerLoadingData * PlayerAssistant::getPlayerLoadingData(uint64 guid)
{
	PlayerLoadingDataMap_t::iterator it = m_player_loading_data_map.find(guid);
	if (it != m_player_loading_data_map.end())
	{
		return it->second;
	}

	return NULL;
}

void PlayerAssistant::addPlayerLoadingData(uint64 guid)
{
	PlayerLoadingData * pld = new PlayerLoadingData();
	pld->setPlayerGuid(guid);
	m_player_loading_data_map.insert(std::make_pair(guid, pld));
}

void PlayerAssistant::removePlayerLoadingData(uint64 guid)
{
	PlayerLoadingDataMap_t::iterator it = m_player_loading_data_map.find(guid);
	if (it != m_player_loading_data_map.end())
	{
		delete it->second;
		m_player_loading_data_map.erase(it);
	}
	else
	{
		PLAYER_LOG_ERROR("start to remvoe player loading data info, but did not find it <%llu>", guid);
	}
}

void PlayerAssistant::playerLoadFailed(uint64 guid, RequestList * request_list)
{
	for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it != request_list->getRequestList().end(); ++it)
	{
		Request * request = *it;
		if (request->complated && (!request->success))
		{
			PLAYER_LOG_ERROR("failed to load player, player is <%llu>, failed guid is <%llu>", guid, request->guid);
		}
		else if (request->complated && request->success)
		{
			//removeCacheEntity(request->guid);
			m_plugin_cfg.pool->onlyRemoveEntity(request->guid, guid);
		}
	}
	removePlayerLoadingData(guid);
	m_manage_player->playerDataLoadFinish(guid, false);
}

void PlayerAssistant::playerLoadSuccess(uint64 guid)
{
	PlayerLoadingData * pld_ins = getPlayerLoadingData(guid);
	PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(pld_ins, );

	pld_ins->addEntityToPool(m_plugin_cfg.pool);
	pld_ins->clearEntity();
	removePlayerLoadingData(guid);

	m_manage_player->playerDataLoadFinish(guid, true);
}

//void PlayerAssistant::initRoleLocaltion(uint64 guid, entity::RoleLocaltion * role_localtion)
//{
//	role_localtion->set_guid(guid);
//}
//
//void PlayerAssistant::initRoleItem(uint64 guid, entity::RoleItem * role_item)
//{
//	role_item->set_guid(guid);
//}
//
//void PlayerAssistant::initRoleSkill(uint64 guid, entity::RoleSkill * role_skill)
//{
//	role_skill->set_guid(guid);
//}
//
//void PlayerAssistant::initRoleBuffer(uint64 guid, entity::RoleBuffer * role_buffer)
//{
//	role_buffer->set_guid(guid);
//}
