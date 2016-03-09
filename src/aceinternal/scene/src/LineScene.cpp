
#include <ace/Time_Value.h>
#include <ace/Date_Time.h>
#include <boost/filesystem.hpp>
#include "opcode.h"
#include "LineScene.h"
#include "PlayerFunction.h"
#include "protocol/msg_exchange.pb.h"
#include "share_fun.h"
#include "Plugin.h"
#include "entity/runes.pb.h"
#include "entity/dodge.pb.h"
#include "protocol/msg_plugin.pb.h"

#define PLUGIN_SAVE_CONFIG_DIR	"plugin_data"

#define MK_LINES "[MK_LINESCENE] "

#define LINES_LOG_DEBUG(log_info, ...)		m_scene_cfg.logger->log(LL_DEBUG, "%s" MK_LINES log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LINES_LOG_INFO(log_info, ...)		m_scene_cfg.logger->log(LL_INFO, "%s" MK_LINES log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LINES_LOG_WARNING(log_info, ...)	m_scene_cfg.logger->log(LL_WARNING, "%s" MK_LINES log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LINES_LOG_TRACE(log_info, ...)		m_scene_cfg.logger->log(LL_TRACE, "%s" MK_LINES log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LINES_LOG_ERROR(log_info, ...)		m_scene_cfg.logger->log(LL_ERROR, "%s" MK_LINES log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define LINES_LOG_FATAL(log_info, ...)		m_scene_cfg.logger->log(LL_FATAL, "%s" MK_LINES log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)

#define LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(p, result)	\
	if (NULL == (p))	\
	{	\
	LINES_LOG_ERROR("failed to check point <%s>, function is <%s>, line is <%d>\n", #p, __FUNCTION__, __LINE__);	\
	return result;	\
	}

#define LINES_CHECK_NULL_POINT_WITH_LOG_CMD(p, command)	\
	if (NULL == (p))	\
	{	\
	LINES_LOG_ERROR("failed to check point <%s>, function is <%s>, line is <%d>\n", #p, __FUNCTION__, __LINE__);	\
	command;	\
	}


#define GET_PLAYER_BRIEF_INFO(player, result)	\
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_brief_info_map_mutex, result);	\
	PlayerBriefInfoEx * player_brief_info = NULL;	\
	PlayerBriefInfoExMap_t::iterator it = m_player_brief_info_map.find(player);	\
	if (it != m_player_brief_info_map.end())	\
	{	\
		player_brief_info = it->second;	\
	}	\
	else	\
	{	\
		LINES_LOG_ERROR("do not find player brief info , guid <%llu>", player);	\
	}

#define GET_PLAYER_BRIEF_INFO_BYNAME(player_name, result)	\
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_brief_info_map_mutex, result);	\
	PlayerBriefInfoEx * player_brief_info = NULL;	\
	PlayerBriefInfoExNameMap_t::iterator it = m_player_brief_info_map_byname.find(player_name);	\
	if (it != m_player_brief_info_map_byname.end())	\
	{	\
	player_brief_info = it->second;	\
	}	\
	else	\
	{	\
	LINES_LOG_ERROR("do not find player brief info , name is <%s>", player_name.c_str());	\
	}


PlayerBriefInfoEx::PlayerBriefInfoEx()
: player(0)
, transfer_player_info(NULL)
, player_bi(NULL)
{
	player_bi = new PlayerBriefInfo();
}

PlayerBriefInfoEx::PlayerBriefInfoEx(uint64 pguid, typed::entity::Short * short_info, uint32 s_id, bool on_line)
: player(pguid)
, transfer_player_info(NULL)
, player_bi(NULL)
{
	player_bi = new PlayerBriefInfo(pguid, short_info, s_id, on_line);
}

PlayerBriefInfoEx::~PlayerBriefInfoEx()
{
	if (NULL != player_bi)
	{
		delete player_bi;
		player_bi = NULL;
	}

	if (NULL != transfer_player_info)
	{
		delete transfer_player_info;
		transfer_player_info = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////

LineScene::LineScene()
{
}

LineScene::~LineScene()
{
	cleanMapSecondContent(m_player_brief_info_map);
	cleanMapSecondContent(m_collect_msg_info_map);
}

int LineScene::init(const SceneCfg & scene_cfg)
{
	m_scene_cfg = scene_cfg;

	if (initLunchTime() == -1)
	{
		DEF_LOG_ERROR("failed to init lunch time\n");
		return -1;
	}

	int r = SceneImp::init(scene_cfg);

	SceneRequestCfg sr_cfg;
	sr_cfg.line = this;
	sr_cfg.logger = m_scene_cfg.logger;
	sr_cfg.pool = this->getPool();
	sr_cfg.scene = this;
	if (m_scene_request.init(sr_cfg) == -1)
	{
		DEF_LOG_ERROR("failed to init scene request\n");
		return -1;
	}

// 	if (m_data_record.init(this, m_scene_cfg.record_server_addr) == -1)
// 	{
// 		DEF_LOG_ERROR("failed to init data record <%s>\n", m_scene_cfg.record_server_addr.c_str());
// 		return -1;
// 	}

// 	m_manage_gm_session.setScene(this);
// 	if (m_manage_gm_session.init(m_scene_cfg.gm_server_addr) == -1)
// 	{
// 		DEF_LOG_ERROR("failed to init gm server <%s>\n", m_scene_cfg.gm_server_addr.c_str());
// 		return -1;
// 	}
// 
// 	m_scene_cfg.exchange_cfg.line_id = m_scene_cfg.line_no;
// 	m_scene_cfg.exchange_cfg.line_scene = this;
// 	if (m_manage_exchange.init(m_scene_cfg.exchange_cfg) == -1)
// 	{
// 		DEF_LOG_ERROR("failed to init manage exchange, addr is <%s>\n", m_scene_cfg.exchange_cfg.exchange_addr.c_str());
// 		return -1;
// 	}
// 
// 	if (m_manage_cross_server.init(m_scene_cfg.cross_server_cfg, m_scene_cfg.server_cfg, m_scene_cfg.cross_server_cfg.security_key, this) == -1)
// 	{
// 		DEF_LOG_ERROR("failed to init manage cross server\n");
// 		return -1;
// 	}
// 
// 	m_manage_battle_report.setSecurityKey(m_scene_cfg.server_cfg, m_scene_cfg.battle_report_cfg.security_key);
// 
// 	// todo
// 	if (m_manage_battle_report.init(m_scene_cfg.battle_report_cfg.addr) == -1)
// 	{
// 		DEF_LOG_ERROR("failed to init manage battle report\n");
// 		return -1;
// 	}
// 
// 	if (m_manage_cross_player_data.init(&m_scene_cfg) != 0)
// 	{
// 		DEF_LOG_ERROR("failed to init manage_cross_player_data\n");
// 		return -1;
// 	}

	return r;
}

int LineScene::stop()
{
	SceneImp::stop();
	m_manage_exchange.stop();
	m_data_record.stop();
	return 0;
}

bool LineScene::isLineScene()
{
	return true;
}

int LineScene::addScene(uint32 scene_id, Scene * scene)
{
	SceneMap_t::iterator it = m_scene_map.find(scene_id);
	if (it != m_scene_map.end())
	{
		// error
		LINES_LOG_ERROR("get the same scene id <%d>", scene_id);
		return -1;
	}

	m_scene_map.insert(std::make_pair(scene_id, scene));
	return 0;
}

Scene * LineScene::getScene(uint32 scene_id)
{
	SceneMap_t::iterator it = m_scene_map.find(scene_id);
	if (it != m_scene_map.end())
	{
		return it->second;
	}

	return NULL;
}

PlayerBriefInfo * LineScene::getPlayerBriefInfo(uint64 player)
{
	GET_PLAYER_BRIEF_INFO(player, NULL);
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(player_brief_info, NULL);
	return player_brief_info->player_bi;
}


uint32 LineScene::getAllVipShort(Uint64Vec_t &vec64)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_brief_info_map_mutex, 0);
	vec64.reserve(m_player_brief_info_map.size() * 0.1 + 20); // 10%的vip个数+保底20。
	
	PlayerBriefInfoEx * player_brief_info = NULL;
	for (PlayerBriefInfoExMap_t::iterator it = m_player_brief_info_map.begin();
		it != m_player_brief_info_map.end(); ++it)
	{
		if (NULL != it->second
			&& NULL != it->second->player_bi
			&& NULL != it->second->player_bi->short_info
			&& it->second->player_bi->short_info->vip_level() > 0)
			vec64.push_back(it->first);
	}

	return vec64.size();
}


bool LineScene::playerEnteredWorld(uint64 player)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_brief_info_map_mutex, true);
	PlayerBriefInfoExMap_t::iterator it = m_player_brief_info_map.find(player);
	if (it != m_player_brief_info_map.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

PlayerBriefInfo * LineScene::getPlayerBriefInfoByName(const string & player_name)
{
	GET_PLAYER_BRIEF_INFO_BYNAME(player_name, NULL);
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(player_brief_info, NULL);

	return player_brief_info->player_bi;
}

void LineScene::playerLogin(uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_online_players_mutex, );
	m_online_players.insert(player_guid);
}

void LineScene::playerLogoff(uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_online_players_mutex, );
	m_online_players.erase(player_guid);
}

Uint64Set_t LineScene::getOnlinePlayers()
{
	Uint64Set_t online_players;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_online_players_mutex, online_players);
		online_players = m_online_players;
	}

	return online_players;
}

void LineScene::addPlayerShortInfo(uint64 player, typed::entity::Short * short_info, uint32 map_id, bool is_online)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_brief_info_map_mutex, );	
	PlayerBriefInfoExMap_t::iterator it = m_player_brief_info_map.find(player);
	if (m_player_brief_info_map.end() == it)
	{
		PlayerBriefInfoEx * pb_info = new PlayerBriefInfoEx(player, short_info, map_id, is_online);
		m_player_brief_info_map.insert(std::make_pair(player, pb_info));
		m_player_brief_info_map_byname.insert(std::make_pair(short_info->name(), pb_info));
	}
	else
	{
		// error
		LINES_LOG_ERROR("add player route reduplicate, player <%llu>", player);
		delete short_info;
	}
}

void LineScene::updatePlayerShortInfo(uint64 player, typed::entity::Short * short_info)
{
	if (!this->isCrossServer())
	{
		LINES_LOG_ERROR("this is not cross server, should call LineScene::updatePlayerShortInfo");
		return ;
	}

	LINES_LOG_DEBUG("update player short info <%llu>", player);

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_brief_info_map_mutex, );	
	PlayerBriefInfoExMap_t::iterator it = m_player_brief_info_map.find(player);
	if (m_player_brief_info_map.end() == it)
	{
		PlayerBriefInfoEx * pb_info = new PlayerBriefInfoEx(player, short_info, 0, false);
		m_player_brief_info_map.insert(std::make_pair(player, pb_info));
		m_player_brief_info_map_byname.insert(std::make_pair(short_info->name(), pb_info));
	}
	else
	{
		PlayerBriefInfoEx * pb_info = it->second;
		delete pb_info->player_bi->short_info;
		pb_info->player_bi->short_info = short_info;
	}
}

bool LineScene::isCrossServer()
{
	return 1 != m_scene_cfg.cross_server_cfg.server_type;
}

//void LineScene::playerLogin(uint64 player, uint32 map_id, int curr_level)
//{
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_brief_info_map_mutex, );	
//	PlayerBriefInfoExMap_t::iterator it = m_player_brief_info_map.find(player);
//	if (m_player_brief_info_map.end() != it)
//	{
//		PlayerBriefInfoEx * pbi_ex = it->second;
//		pbi_ex->player_bi->is_online = true;
//		pbi_ex->player_bi->scene_id = map_id;
//		pbi_ex->player_bi->short_info->set_level(curr_level);
//		pbi_ex->player_bi->short_info->set_curr_map_id(map_id);
//		uint64 curr_time = 0;
//		ACE_OS::gettimeofday().to_usec(curr_time);
//		pbi_ex->player_bi->short_info->set_last_login_time(curr_time);
//
//		m_online_players.insert(player);
//	}
//	else
//	{
//		// error
//		LINES_LOG_ERROR("enter LineScene::playerLogin, but did not find the player <%llu>", player);
//	}
//}
//
//void LineScene::playerLogoff(uint64 player)
//{
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_brief_info_map_mutex, );	
//	PlayerBriefInfoExMap_t::iterator it = m_player_brief_info_map.find(player);
//	if (m_player_brief_info_map.end() != it)
//	{
//		PlayerBriefInfoEx * pbi_ex = it->second;
//		pbi_ex->player_bi->is_online = false;
//		uint64 curr_time = 0;
//		ACE_OS::gettimeofday().to_usec(curr_time);
//		pbi_ex->player_bi->short_info->set_last_logoff_time(curr_time);
//
//		m_online_players.erase(player);
//	}
//	else
//	{
//		// error
//		LINES_LOG_ERROR("enter LineScene::playerLogoff, but did not find the player <%llu>", player);
//	}
//}
//
//void LineScene::playerLevelup(uint64 player, int curr_level)
//{
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_brief_info_map_mutex, );	
//	PlayerBriefInfoExMap_t::iterator it = m_player_brief_info_map.find(player);
//	if (m_player_brief_info_map.end() != it)
//	{
//		PlayerBriefInfoEx * pbi_ex = it->second;
//		pbi_ex->player_bi->short_info->set_level(curr_level);
//	}
//	else
//	{
//		// error
//		LINES_LOG_ERROR("enter LineScene::playerLogoff, but did not find the player <%llu>", player);
//	}
//}

//void LineScene::modifyPlayerRoute(uint64 player, uint32 map_id)
//{
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_brief_info_map_mutex, );	
//	PlayerBriefInfoExMap_t::iterator it = m_player_brief_info_map.find(player);
//	if (m_player_brief_info_map.end() != it)
//	{
//		it->second->player_bi->scene_id = map_id;
//	}
//	else
//	{
//		// error
//		LINES_LOG_ERROR("modify player route, but does not find the player <%llu>", player);
//	}
//}
//
void LineScene::transferPlayer(TransferPlayerInfo * transfer_player_info)
{
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(transfer_player_info, );

	GET_PLAYER_BRIEF_INFO(transfer_player_info->player_guid, );
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(player_brief_info, );

	player_brief_info->player_bi->scene_id = transfer_player_info->target_map_id;
	if (NULL != player_brief_info->transfer_player_info)
	{
		LINES_LOG_ERROR("start to transfer player info, but the pre transfer info is not NULL, start to clean it, player <%llu>\n", transfer_player_info->player_guid);
		cleanTransferPlayerInfo(player_brief_info->transfer_player_info);
		player_brief_info->transfer_player_info = NULL;
	}
	player_brief_info->transfer_player_info = transfer_player_info;
	player_brief_info->player_bi->is_jumpmap = true;
	m_transfer_player_set.insert(transfer_player_info->player_guid);
}

TransferPlayerInfo * LineScene::obtainPlayer(uint64 player, uint32 map_id)
{
	GET_PLAYER_BRIEF_INFO(player, NULL);
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(player_brief_info, NULL);

	player_brief_info->player_bi->scene_id = map_id;
	player_brief_info->player_bi->is_jumpmap = false;
	TransferPlayerInfo * result = player_brief_info->transfer_player_info;
	player_brief_info->transfer_player_info = NULL;

	m_transfer_player_set.erase(player);

	return result;
}

const Uint64Set_t & LineScene::getTransferPlayerSet()
{
	return m_transfer_player_set;
}

uint32 LineScene::getPlayerFightData(uint64 player_guid, TransferPlayerInfo ** trans_player_info)
{
	GET_PLAYER_BRIEF_INFO(player_guid, 0);
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(player_brief_info, 0);

	if (0 == player_brief_info->player_bi->scene_id)
	{
		if (NULL != player_brief_info->transfer_player_info)
		{
			*trans_player_info = new TransferPlayerInfo();
			extractFightData(player_brief_info, *trans_player_info);
		}
		return 0;
	}
	else
	{
		if (player_brief_info->player_bi->is_online)
		{
			return player_brief_info->player_bi->scene_id;
		}
		else
		{
			return 0;
		}
	}
}

bool LineScene::sendSceneMsg(uint32 scene_id, PackInfo * pack_info)
{
	Scene * scene = getScene(scene_id);
	if (NULL == scene)
	{
		LINES_LOG_ERROR("failed to get scene by id <%d>, player is <%llu>, opcode is <%d>", scene_id, pack_info->guid, pack_info->opcode);
		delete pack_info;
		return false;
	}

	scene->packInput(pack_info);

	return true;
}

bool LineScene::sendScenePlayerMsg(uint64 player, PackInfo * pack_info)
{
	PlayerBriefInfo *  pb_info = getPlayerBriefInfo(player);
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(pb_info, false);
	if (pb_info->is_online)
	{
		return sendSceneMsg(pb_info->scene_id, pack_info);
	}
	else
	{
		return false;
	}
}

bool LineScene::broadCastSceneMsg(PackInfo * pack_info)
{
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(pack_info, false);

	for (SceneMap_t::iterator it = m_scene_map.begin(); it != m_scene_map.end(); ++it)
	{
		Scene * scene = it->second;
		if (!scene->isLineScene())
		{
			PackInfo * pi = new PackInfo(pack_info->opcode, pack_info->guid, cloneMessage(pack_info->msg));

			scene->packInput(pi);
		}
	}

	delete pack_info;

	return true;
}

void LineScene::scenePackInput(PackInfo * pack_info)
{
	SceneImp::packInput(pack_info);
}

void LineScene::transmitPlayerMsg(PackInfo * pack_info)
{
	LINES_LOG_DEBUG("start to transmit player msg, player guid is <%llu>, opcode is <%d>, attach data is <%d>", pack_info->guid, pack_info->opcode, pack_info->attach_data);
	if (pack_info->attach_data > 0)
	{
		PlayerBriefInfo * player_bi = getPlayerBriefInfo(pack_info->guid);
		if (NULL != player_bi)
		{
			if (player_bi->is_online)
			{
				pack_info->attach_data -= 1;
				PackInfo * pi = new PackInfo(pack_info->opcode, pack_info->guid, NULL, pack_info->attach_data);
				pi->msg = cloneMessage(pack_info->msg);

				sendSceneMsg(player_bi->scene_id, pi);
				return ;
			}
		}
	}

	LINES_LOG_ERROR("failed to transmit player msg, player guid is <%llu>, opcode is <%d>, attach data is <%d>", pack_info->guid, pack_info->opcode, pack_info->attach_data);
	
	// player offline or reach the limit of transmitting package number
	if (LMSG_REQUEST_PLAYER_FIGHT_DATA == pack_info->opcode)
	{
		PackInfo * pi = new PackInfo(LMSG_GET_DATA_FROM_OTHER_SCENE_BACK_FAILED, pack_info->guid, NULL);
		this->packInput(pi);
	}
	else if (LMSG_REDUCE_GOLD == pack_info->opcode)
	{
		// todo
		typed::protocol::plmsg_line_reduce_gold * reduce_gold = (typed::protocol::plmsg_line_reduce_gold *)pack_info->msg;
		LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(reduce_gold, );

		typed::protocol::plmsg_line_reduce_gold * rg_return = (typed::protocol::plmsg_line_reduce_gold *)cloneMessage(pack_info->msg);
		LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(rg_return, );

		rg_return->set_result(4);

		PackInfo * pi = new PackInfo(LMSG_REDUCE_GOLD_RETURN, pack_info->guid, rg_return);

		this->scenePackInput(pi);
	}
	else if (LMSG_UPDATE_REWARD == pack_info->opcode)
	{
		typed::protocol::plmsg_line_update_reward * update_gold = (typed::protocol::plmsg_line_update_reward *)pack_info->msg;
		LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(update_gold, );

		typed::protocol::plmsg_line_update_reward * ug_return = (typed::protocol::plmsg_line_update_reward *)cloneMessage(pack_info->msg);
		LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(ug_return, );

		ug_return->set_result(4);

		PackInfo * pi = new PackInfo(LMSG_UPDATE_REWARD_RETURN, pack_info->guid, ug_return);

		this->scenePackInput(pi);
	}
}

void LineScene::collectMessage(uint64 player_guid, CollectMsgInfo * collect_msg_info)
{
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_collect_msg_info_map_mutex, );
		CollectMsgInfoMap_t::iterator it = m_collect_msg_info_map.find(player_guid);
		if (it != m_collect_msg_info_map.end())
		{
			// error
			LINES_LOG_ERROR("get the duplicate player <%llu> while collect message", player_guid);
			delete collect_msg_info;
			return ;
		}

		m_collect_msg_info_map.insert(std::make_pair(player_guid, collect_msg_info));
	}
	PackInfo * pi = new PackInfo(LMSG_GET_DATA_FROM_OTHER_SCENE_BACK, player_guid, NULL);
	this->packInput(pi);
}

CollectMsgInfo * LineScene::getCollectMessage(uint64 player_guid)
{
	CollectMsgInfo * c_msg_info = NULL;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_collect_msg_info_map_mutex, NULL);
	CollectMsgInfoMap_t::iterator it = m_collect_msg_info_map.find(player_guid);
	if (it != m_collect_msg_info_map.end())
	{
		c_msg_info = it->second;
		m_collect_msg_info_map.erase(it);
	}
	else
	{
		LINES_LOG_ERROR("failed to get collect message by player guid <%llu>", player_guid);
	}

	return c_msg_info;
}

int LineScene::sendExchangeMsg(Packet * packet)
{
	m_manage_exchange.handleOutputPacket(packet);

	return 0;
}

SceneRequest * LineScene::getSceneRequest()
{
	return &m_scene_request;
}

DataRecord * LineScene::getDataRecord()
{
	return &m_data_record;
}

void LineScene::collectPlayerSect(uint32 sect_id, uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_sect_map_mutex, );
	PlayerSectMap_t::iterator it = m_player_sect_map.find(sect_id);
	if (it != m_player_sect_map.end())
	{
		Uint64Set_t & sect = it->second;
		sect.insert(player_guid);
	}
	else
	{
		Uint64Set_t sect;
		sect.insert(player_guid);
		m_player_sect_map.insert(std::make_pair(sect_id, sect));
		LINES_LOG_INFO("collect new sect id <%d>, player is <%llu>", sect_id, player_guid);
	}
}

void LineScene::removePlayerSect(uint32 sect_id, uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_sect_map_mutex, );
	PlayerSectMap_t::iterator it = m_player_sect_map.find(sect_id);
	if (it != m_player_sect_map.end())
	{
		Uint64Set_t & sect = it->second;
		sect.erase(player_guid);
	}
	else
	{
		LINES_LOG_ERROR("do not find the sect map while remove sect player, sect is <%d>, player is <%llu>", sect_id, player_guid);
	}
}

Uint64Set_t LineScene::getPlayerBySect(uint32 sect_id)
{
	Uint64Set_t sect;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_sect_map_mutex, sect);
		PlayerSectMap_t::iterator it = m_player_sect_map.find(sect_id);
		if (it != m_player_sect_map.end())
		{
			sect = it->second;
		}
	}

	return sect;
}

void LineScene::setPlayerMapQualification(uint32 map_id, uint64 player_guid)
{
	LINES_LOG_DEBUG("start to set player map qulification, player is <%llu>, map id is <%d>", player_guid, map_id);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_map_qualification_map_mutex, );
	m_player_map_qualification_map[map_id].insert(player_guid);
}

bool LineScene::validatePlayerMapQualification(uint32 map_id, uint64 player_guid)
{
	LINES_LOG_DEBUG("start to validate player map qulification, player is <%llu>, map id is <%d>", player_guid, map_id);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_map_qualification_map_mutex, false);
	PlayerMapQualificationMap_t::iterator it = m_player_map_qualification_map.find(map_id);
	if (it != m_player_map_qualification_map.end())
	{
		if (it->second.find(player_guid) != it->second.end())
		{
			return true;
		}
	}

	//LINES_LOG_ERROR("failed to validate player map qualification, player is <%llu>, map is <%d>", player_guid, map_id);
	return false;
}

void LineScene::clearPlayerMapQualification(uint32 map_id, uint64 player_guid)
{
	LINES_LOG_DEBUG("start to clear map qulification, player is <%llu>, map id is <%d>", player_guid, map_id);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_map_qualification_map_mutex, );
	PlayerMapQualificationMap_t::iterator it = m_player_map_qualification_map.find(map_id);
	if (it != m_player_map_qualification_map.end())
	{
		it->second.erase(player_guid);
	}
}

void LineScene::clearPlayerMapAllQualification(uint32 map_id)
{
	LINES_LOG_DEBUG("start to clear map all qulification, map id is <%d>", map_id);
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_map_qualification_map_mutex, );
	PlayerMapQualificationMap_t::iterator it = m_player_map_qualification_map.find(map_id);
	if (it != m_player_map_qualification_map.end())
	{
		m_player_map_qualification_map.erase(it);
	}
}

void LineScene::sendMasterMsg(Packet * packet)
{
	m_manage_gm_session.output(packet);
}

void LineScene::collectBattleGroundPlayer(uint64 player_guid, CollectMsgInfo * collect_msg_info)
{
	LINES_LOG_DEBUG("collect battle ground player <%llu>", player_guid);

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_battleground_player_map_mutex, );
	CollectMsgInfoMap_t::iterator it = m_battleground_player_map.find(player_guid);
	if (it != m_battleground_player_map.end())
	{
		LINES_LOG_ERROR("collect reduplicate battle ground player info <%llu>", player_guid);
		delete collect_msg_info;
	}
	else
	{
		m_battleground_player_map.insert(std::make_pair(player_guid, collect_msg_info));
	}
}

CollectMsgInfo * LineScene::getBattleGroundPlayer(uint64 player_guid)
{
	LINES_LOG_DEBUG("get battle ground player <%llu>", player_guid);

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_battleground_player_map_mutex, NULL);
	CollectMsgInfoMap_t::iterator it = m_battleground_player_map.find(player_guid);
	if (it != m_battleground_player_map.end())
	{
		return it->second;
	}

	return NULL;
}

bool LineScene::removeBattleGroundPlayer(uint64 player_guid)
{
	LINES_LOG_DEBUG("remove battle ground player <%llu>", player_guid);

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_battleground_player_map_mutex, false);
	CollectMsgInfoMap_t::iterator it = m_battleground_player_map.find(player_guid);
	if (it != m_battleground_player_map.end())
	{
		delete it->second;
		m_battleground_player_map.erase(it);
		return true;
	}

	return false;
}

void LineScene::removeBattleGroundPlayers()
{
	LINES_LOG_DEBUG("remove battle ground players ");

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_battleground_player_map_mutex, );
	for (CollectMsgInfoMap_t::iterator it = m_battleground_player_map.begin(); it != m_battleground_player_map.end(); ++it)
	{
		LINES_LOG_DEBUG("remove battle ground players <%llu>", it->first);
		delete it->second;
	}

	m_battleground_player_map.clear();
}

void LineScene::sendBattleReportMsg(Packet * packet)
{
	m_manage_battle_report.output(packet);
}

uint64 LineScene::getBattleReportId()
{
	return m_manage_battle_report.getBattleReportId();
}
string LineScene::getVipInitSecurityKey()
{
	return m_scene_cfg.vip_init_security_key;
}

string LineScene::getRobotSecurityKey()
{
	return m_scene_cfg.robot_security_key;
}

CollectMsgInfo * LineScene::collectPlayerFightDataMsgInfo(PluginCfg & plugin_cfg, typed::entity::Player * player)
{
	CollectMsgInfo * collect_msg_info = new CollectMsgInfo();
	collect_msg_info->player_guid = player->guid();
	collect_msg_info->addMsg(player->guid(), cloneMessage(player));

	for (int i = 0; i < player->roles_size(); ++i)
	{
		if (player->roles(i) < 10)
		{
			continue;
		}

		GET_ROLE_FROM_POOL(plugin_cfg.pool, player->roles(i), role);
		LINES_CHECK_NULL_POINT_WITH_LOG_CMD(role, continue);
		collect_msg_info->addMsg(player->roles(i), cloneMessage(role));

		for (int j = 0; j < role->equipments_size(); ++j)
		{
			if (role->equipments(j) < 10)
			{
				continue;
			}

			GET_ITEM_FROM_POOL(plugin_cfg.pool, role->equipments(j), item);
			LINES_CHECK_NULL_POINT_WITH_LOG_CMD(item, continue);
			collect_msg_info->addMsg(role->equipments(j), cloneMessage(item));
		}
	}

	GET_FORM_FROM_POOL(plugin_cfg.pool, player->form(), form);
	LINES_CHECK_NULL_POINT_WITH_LOG_CMD(form, );
	if (NULL != form)
	{
		collect_msg_info->addMsg(player->form(), cloneMessage(form));
	}

	if (player->player_wife() > 10)
	{
		GET_PLAYERWIFE_FROM_POOL(plugin_cfg.pool, player->player_wife(), player_wife);
		if (NULL != player_wife)
		{
			collect_msg_info->addMsg(player->player_wife(), cloneMessage(player_wife));
		}
	}

	for (int i = 0; i < player->wifes_size(); ++i)
	{
		GET_WIFE_FROM_POOL(plugin_cfg.pool, player->wifes(i), wifes);
		if (NULL != wifes)
		{
			collect_msg_info->addMsg(player->wifes(i), cloneMessage(wifes));
		}
	}

	if (player->runes() != 0)
	{
		GET_RUNES_FROM_POOL(plugin_cfg.pool, player->runes(), runes);
		if (NULL != runes)
		{
			collect_msg_info->addMsg(player->runes(), cloneMessage(runes));
		}
	}

	if (player->dodge() != 0)
	{
		GET_DODGE_FROM_POOL(plugin_cfg.pool, player->dodge(), dodge);
		if (NULL != dodge)
		{
			collect_msg_info->addMsg(player->dodge(), cloneMessage(dodge));
		}
	}

	return collect_msg_info;
}

void LineScene::addForbidPlayer(uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_forbid_speak_player_mutex, );
	m_forbid_speak_player.insert(player_guid);
}

void LineScene::removeForbidPlayer(uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_forbid_speak_player_mutex, );
	m_forbid_speak_player.erase(player_guid);
}

bool LineScene::isForbidSpeak(uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_forbid_speak_player_mutex, false);
	if (m_forbid_speak_player.find(player_guid) != m_forbid_speak_player.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

CrossServer * LineScene::getCrossServer()
{
	return &m_manage_cross_server;
}

LunchInfo & LineScene::getLunchInfo()
{
	return m_lunch_info;
}

ManageContainer * LineScene::getManageContainer()
{
	return &m_manage_container;
}

void LineScene::broadcastPacketToAllOnlinePlayers(Packet * packet)
{
	string stream_str("");
	if (packet->body_size() > 0)
	{
		stream_str = string(packet->ch_body(), packet->body_size());
	}

	Uint64Set_t online_players = this->getOnlinePlayers();
	for (Uint64Set_t::iterator it = online_players.begin(); it != online_players.end(); ++it)
	{
		Packet * ps = new Packet(packet->opcode(), *it, stream_str);
		this->playerMsg(ps);
	}

	delete packet;
}

void LineScene::collectCrossPlayerData(const string & line_id, const string & platform, uint64 player_guid, uint64 entity_guid, const string & entity_stream)
{
	m_manage_cross_player_data.collectCrossPlayerData(line_id, platform, player_guid, entity_guid, entity_stream);
}

void LineScene::collectCrossPlayerDataFinish(const string & line_id, const string & platform, uint64 player_guid)
{
	m_manage_cross_player_data.collectCrossPlayerDataFinish(line_id, platform, player_guid);
}

void LineScene::deliveryPlayerDataToAllGS()
{
	m_manage_cross_player_data.deliveryCrossPlayerDataToGS();
}

void LineScene::deliveryPlayerDataToGS(const string & platform, const string & line_id)
{
	m_manage_cross_player_data.deliveryCrossPlayerDataToGS(platform, line_id);
}

void LineScene::getCrossPlayerDataInfo(uint64 player_guid, map<uint64, MSG_TYPE *> & msg_map)
{
	m_manage_cross_player_data.getCrossPlayerDataInfo(player_guid, msg_map);
}

void LineScene::extractFightData(PlayerBriefInfoEx * player_brief_info, TransferPlayerInfo * trans_player_info)
{
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(player_brief_info, );
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(trans_player_info, );
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(player_brief_info->transfer_player_info, );

	trans_player_info->player_guid = player_brief_info->transfer_player_info->player_guid;
	trans_player_info->player = cloneMessage(player_brief_info->transfer_player_info->player);

	typed::entity::Player * player = (typed::entity::Player *)player_brief_info->player;
	LINES_CHECK_NULL_POINT_WITH_LOG_RETURN(player, );
	for (int i = 0; i < player->roles_size(); ++i)
	{
		GuidMsgMap_t::iterator it = player_brief_info->transfer_player_info->msg_map.find(player->roles(i));
		if (it != player_brief_info->transfer_player_info->msg_map.end())
		{
			trans_player_info->putMsg(it->first, cloneMessage(it->second));
		}
		else
		{
			LINES_LOG_ERROR("failed to get role, player is <%llu>, role guid is <%llu>", player->guid(), player->roles(i));
			return ;
		}
	}

	GuidMsgMap_t::iterator it = player_brief_info->transfer_player_info->msg_map.find(player->form());
	if (it != player_brief_info->transfer_player_info->msg_map.end())
	{
		trans_player_info->putMsg(it->first, cloneMessage(it->second));
	}
	else
	{
		LINES_LOG_ERROR("failed to get form, player is <%llu>, form is <%llu>", player->guid(), player->form());
		return ;
	}
}

void LineScene::cleanTransferPlayerInfo(TransferPlayerInfo * transfer_player_info)
{
	for (GuidMsgMap_t::iterator it = transfer_player_info->msg_map.begin(); it != transfer_player_info->msg_map.end(); ++it)
	{
		delete it->second;
	}

	delete transfer_player_info;
}

int LineScene::initLunchTime()
{
	boost::filesystem::path plugin_data(PLUGIN_SAVE_CONFIG_DIR);
	if (!boost::filesystem::exists(plugin_data))
	{
		boost::filesystem::create_directory(plugin_data);
	}

	uint64 curr_time_in_sec = 0;
	ACE_Time_Value curr_time_v = ACE_OS::gettimeofday();
	ACE_Date_Time curr_dt(curr_time_v);
	curr_time_v.to_usec(curr_time_in_sec);
	curr_time_in_sec /= 1000000;

	m_lunch_info.current_lunch_time = curr_time_in_sec;

	if (m_scene_cfg.is_first_launch)
	{
		m_lunch_info.first_lunch_time = curr_time_in_sec;
		m_lunch_info.first_lunch_day_zero_time = curr_time_in_sec - curr_dt.hour() * 3600 - curr_dt.minute() * 60 - curr_dt.second();
		m_lunch_info.lunch_day = 1;
		if (!this->savePluginConfig(PT_MANAGE_SHORT, 100, (char *)&m_lunch_info, sizeof(m_lunch_info)))
		{
			LINES_LOG_ERROR("failed to save first lunch time to plugin data");
		}
	}
	else
	{
		char * buf_stream = NULL;
		int buf_size = 0;
		if (!this->loadPluginConfig(PT_MANAGE_SHORT, 100, &buf_stream, buf_size))
		{
			DEF_LOG_ERROR("failed to load lunch date\n");
			return -1;
		}

		if (buf_size != sizeof(m_lunch_info))
		{
			DEF_LOG_ERROR("failed to load lunch date, wrong size <%d>\n", buf_size);
			return -1;
		}

		LunchInfo * li = (LunchInfo *)buf_stream;
		m_lunch_info.first_lunch_time = li->first_lunch_time;
		m_lunch_info.first_lunch_day_zero_time = li->first_lunch_day_zero_time;

		m_lunch_info.lunch_day = getTimeDifferentDay(curr_time_in_sec, m_lunch_info.first_lunch_time) + 1;
	}

	return 0;
}