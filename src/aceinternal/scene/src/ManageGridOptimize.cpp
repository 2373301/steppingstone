
#include "ManageGridOptimize.h"
#include "opcode.h"
#include "Packet.h"
#include "share_fun.h"
#include "protocol/msg_guild.pb.h"
#include "protocol/msg_horse.pb.h"
#include "ace/OS_NS_unistd.h"

STAT_PACKET_IMP(PlayerGridInfo);

STAT_PACKET_IMP(PlayerMoveInfo);

STAT_PACKET_IMP(PlayerGridOperation);

PlayerGridInfo::PlayerGridInfo()
: guid(0)
, role_id(0)
, level(0)
, sex(0)
, src_x(0)
, src_y(0)
, dst_x(0)
, dst_y(0)
, direction(0)
, first_role(0)
, is_selected(false)
, flower_rank(0)
, guild_icon(0)
, follow_role_id(0)
, special_right(0)
, horse_id(0)
, wife_follow(0)
, yellow_vip_level(0)
, is_yellow_vip(0)
, is_yellow_year_vip(0)
, is_yellow_high_vip(0)
{
	STAT_PACKET_INC_NEW();
}

PlayerGridInfo::~PlayerGridInfo()
{
	STAT_PACKET_INC_DEL()
}

PlayerMoveInfo::PlayerMoveInfo()
: src_x(0)
, src_y(0)
, dst_x(0)
, dst_y(0)
{
	STAT_PACKET_INC_NEW();
}

PlayerMoveInfo::~PlayerMoveInfo()
{
	STAT_PACKET_INC_DEL();
}

PlayerGridOperation::PlayerGridOperation()
: guid(0)
, level(1)
, flower_rank(0)
, player_grid_op(PGOP_BASE)
, player_grid_info(NULL)
, player_move_info(NULL)
, packet(NULL)
, guild_icon(0)
, follow_role_id(0)
, special_right(0)
, horse_id(0)
, wife_follow(0)
, yellow_vip_level(0)
, is_yellow_vip(0)
, is_yellow_year_vip(0)
, is_yellow_high_vip(0)
{
	STAT_PACKET_INC_NEW();
}

PlayerGridOperation::~PlayerGridOperation()
{
	if (NULL != player_grid_info)
	{
		delete player_grid_info;
		player_grid_info = NULL;
	}

	if (NULL != player_move_info)
	{
		delete player_move_info;
		player_move_info = NULL;
	}

	if (NULL != packet)
	{
		delete packet;
		packet = NULL;
	}

	STAT_PACKET_INC_DEL();
}

ManageGridOptimize::ManageGridOptimize()
: m_scene_cfg(NULL)
, m_scene(NULL)
, m_selected_player_num(100)
, m_is_stop(false)
{
}

ManageGridOptimize::~ManageGridOptimize()
{
	cleanMapSecondContent(m_player_grid_info_map);

	cleanVectorContent(m_player_grid_op_vec);
}

int ManageGridOptimize::init(SceneCfg * scene_cfg, Scene * scene)
{
	m_scene_cfg= scene_cfg;
	m_scene = scene;

	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

int ManageGridOptimize::stop()
{
	m_is_stop = true;
	return 0;
}

int ManageGridOptimize::svc()
{
	REPORT_THREAD_INFO();

	PlayerGridOperationVec_t player_grid_op_vec;

	ACE_Time_Value sleep_time(0, 1000);

	while (!m_is_stop)
	{
		player_grid_op_vec.clear();
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_grid_op_vec_mutex, -1);
			std::copy(m_player_grid_op_vec.begin(), m_player_grid_op_vec.end(), std::back_inserter(player_grid_op_vec));
			m_player_grid_op_vec.clear();
		}

		if (player_grid_op_vec.empty())
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		for (PlayerGridOperationVec_t::iterator it = player_grid_op_vec.begin(); it != player_grid_op_vec.end(); ++it)
		{
			PlayerGridOperation * pgo = *it;
			process(*pgo);
			delete pgo;
		}
	}

	return 0;
}

void ManageGridOptimize::setMaxScreenNumber(int max_screen_number)
{
	m_selected_player_num = max_screen_number;
}

void ManageGridOptimize::add(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->player_grid_op = PGOP_ADD;
	player_grid_op->guid = player->guid();
	player_grid_op->player_grid_info = new PlayerGridInfo();
	fillPlayerGridInfo(player, player_grid_op->player_grid_info);
	putPlayerGridOp(player_grid_op);
}

void ManageGridOptimize::readd(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->player_grid_op = PGOP_READD;
	player_grid_op->guid = player->guid();
	//player_grid_op->player_move_info = new PlayerMoveInfo();
	//fillPlayerMoveInfo(player, player_grid_op->player_move_info);
	putPlayerGridOp(player_grid_op);
}

void ManageGridOptimize::move(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->player_grid_op = PGOP_MOVE;
	player_grid_op->guid = player->guid();
	player_grid_op->player_move_info = new PlayerMoveInfo();
	fillPlayerMoveInfo(player, player_grid_op->player_move_info);
	putPlayerGridOp(player_grid_op);
}

void ManageGridOptimize::remove(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->player_grid_op = PGOP_REMOVE;
	player_grid_op->guid = player->guid();
	//player_grid_op->player_move_info = new PlayerMoveInfo();
	putPlayerGridOp(player_grid_op);
}

void ManageGridOptimize::levelup(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->player_grid_op = PGOP_LEVELUP;
	player_grid_op->guid = player->guid();
	player_grid_op->level = player->level();
	putPlayerGridOp(player_grid_op);
}

void ManageGridOptimize::flowerRank(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->guid = player->guid();
	player_grid_op->player_grid_op = PGOP_FLOWERRANK;
	player_grid_op->flower_rank = player->flower_rank();
	putPlayerGridOp(player_grid_op);
}

void ManageGridOptimize::updateGuildName(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->guid = player->guid();
	player_grid_op->player_grid_op = PGOP_UPDATE_GUILD_NAME;
	player_grid_op->guild_name = player->guild_name();
	player_grid_op->guild_icon = player->guild_icon();
	putPlayerGridOp(player_grid_op);
}

void ManageGridOptimize::updateFollowRoleID(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->guid = player->guid();
	player_grid_op->player_grid_op = PGOP_UPDATE_FOLLOW_ROLE_ID;
	player_grid_op->follow_role_id = player->follow_role_id();
	putPlayerGridOp(player_grid_op);
}

void ManageGridOptimize::updateTitle(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->guid = player->guid();
	player_grid_op->player_grid_op = PGOP_UPDATE_TITLE;
	for (int i = 0; i < player->current_title_size(); ++i)
	{
		player_grid_op->title_vec.push_back(player->current_title(i));
	}
	putPlayerGridOp(player_grid_op);
}
void ManageGridOptimize::updateSpecialRight(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->guid = player->guid();
	player_grid_op->player_grid_op = PGOP_UPDATE_SPECIAL_RIGHT;
	player_grid_op->special_right = player->special_right();
	putPlayerGridOp(player_grid_op);
}
void ManageGridOptimize::updateHorse(typed::entity::Player * player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->guid = player->guid();
	player_grid_op->player_grid_op = PGOP_UPDATE_HORSE;

	player_grid_op->horse_id = player->horse_id();
	putPlayerGridOp(player_grid_op);
}
void ManageGridOptimize::updateWifeFollow(typed::entity::Player* player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->guid = player->guid();
	player_grid_op->player_grid_op = PGOP_UPDATE_WIFE_FOLLOW;

	player_grid_op->wife_follow = player->wife_follow();
	putPlayerGridOp(player_grid_op);
}
void ManageGridOptimize::updateYellowVipLevel(typed::entity::Player* player)
{
	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->guid = player->guid();
	player_grid_op->player_grid_op = PGOP_UPDATE_YELLOW_VIP_LEVEL;

	player_grid_op->yellow_vip_level = player->yellow_vip_level();

	player_grid_op->is_yellow_vip = player->is_yellow_vip();
	player_grid_op->is_yellow_year_vip = player->is_yelow_year_vip();
	player_grid_op->is_yellow_high_vip = player->is_yellow_high_vip();

	putPlayerGridOp(player_grid_op);
}
void ManageGridOptimize::broadcastPacket(Packet * packet)
{
	if (NULL == packet)
	{
		return ;
	}

	PlayerGridOperation * player_grid_op = new PlayerGridOperation();
	player_grid_op->guid = packet->guid();
	player_grid_op->player_grid_op = PGOP_BROADCAST;
	player_grid_op->packet = packet;
	putPlayerGridOp(player_grid_op);
}

void ManageGridOptimize::putPlayerGridOp(PlayerGridOperation * player_grid_op)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_grid_op_vec_mutex, );
	m_player_grid_op_vec.push_back(player_grid_op);
}

void ManageGridOptimize::fillPlayerGridInfo(typed::entity::Player * player, PlayerGridInfo * player_grid_info)
{
	player_grid_info->guid = player->guid();
	player_grid_info->role_id = player->role_id();
	player_grid_info->level = player->level();
	player_grid_info->sex = player->sex();
	player_grid_info->src_x = player->src_x();
	player_grid_info->src_y = player->src_y();
	player_grid_info->dst_x = player->dst_x();
	player_grid_info->dst_y = player->dst_y();
	player_grid_info->direction = player->direction();
	player_grid_info->first_role = player->first_role();
	player_grid_info->flower_rank = player->flower_rank();
	player_grid_info->name = player->name();
	player_grid_info->guild_name = player->guild_name();
	player_grid_info->follow_role_id = player->follow_role_id();
	for (int i = 0; i < player->current_title_size(); ++i)
	{
		player_grid_info->title_vec.push_back(player->current_title(i));
	}
	player_grid_info->special_right = player->special_right();
	player_grid_info->horse_id = player->horse_id();
	player_grid_info->wife_follow = player->wife_follow();
	player_grid_info->yellow_vip_level = player->yellow_vip_level();
	player_grid_info->is_yellow_vip = player->is_yellow_vip();
	player_grid_info->is_yellow_year_vip = player->is_yelow_year_vip();
	player_grid_info->is_yellow_high_vip = player->is_yellow_high_vip();
}

void ManageGridOptimize::fillPlayerMoveInfo(typed::entity::Player * player, PlayerMoveInfo * player_move_info)
{
	player_move_info->src_x = player->src_x();
	player_move_info->src_y = player->src_y();
	player_move_info->dst_x = player->dst_x();
	player_move_info->dst_y = player->dst_y();
}

void ManageGridOptimize::process(PlayerGridOperation & player_grid_op)
{
	switch (player_grid_op.player_grid_op)
	{
	case PGOP_ADD:
		processAdd(player_grid_op);
		break;
	case PGOP_READD:
		processReadd(player_grid_op);
		break;
	case PGOP_MOVE:
		processMove(player_grid_op);
		break;
	case PGOP_REMOVE:
		processRemove(player_grid_op);
		break;
	case PGOP_LEVELUP:
		processLevelup(player_grid_op);
		break;
	case PGOP_BROADCAST:
		processBroadcast(player_grid_op);
		break;
	case PGOP_FLOWERRANK:
		processFlowerRank(player_grid_op);
		break;
	case PGOP_UPDATE_GUILD_NAME:
		processUpdateGuildName(player_grid_op);
		break;
	case PGOP_UPDATE_FOLLOW_ROLE_ID:
		processFollowRoleID(player_grid_op);
		break;
	case PGOP_UPDATE_TITLE:
		processUpdateTitle(player_grid_op);
		break;
	case PGOP_UPDATE_SPECIAL_RIGHT:
		processSpecialRight(player_grid_op);
		break;
	case PGOP_UPDATE_HORSE:
		processHorse(player_grid_op);
		break;
	case PGOP_UPDATE_WIFE_FOLLOW:
		processWifeFollow(player_grid_op);
		break;
	case PGOP_UPDATE_YELLOW_VIP_LEVEL:
		processYellowVipLevel(player_grid_op);
		break;
	default:
		break;
	}
}

void ManageGridOptimize::processAdd(PlayerGridOperation & player_grid_op)
{
	if (NULL == player_grid_op.player_grid_info)
	{
		// error
		return ;
	}

	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL != player_grid_info)
	{
		// error
		delete player_grid_op.player_grid_info;
		player_grid_op.player_grid_info = NULL;
		return;
	}

	m_player_grid_info_map.insert(std::make_pair(player_grid_op.guid, player_grid_op.player_grid_info));
	createOther(player_grid_op.guid);
	syncSelectedPlayer(player_grid_op.player_grid_info);
	player_grid_op.player_grid_info = NULL;
}

void ManageGridOptimize::processReadd(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	createOther(player_grid_op.guid);
}

void ManageGridOptimize::processMove(PlayerGridOperation & player_grid_op)
{
	if (NULL == player_grid_op.player_move_info)
	{
		// error
		return;
	}

	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	player_grid_info->src_x = player_grid_op.player_move_info->src_x;
	player_grid_info->src_y = player_grid_op.player_move_info->src_y;
	player_grid_info->dst_x = player_grid_op.player_move_info->dst_x;
	player_grid_info->dst_y = player_grid_op.player_move_info->dst_y;

	//if (isSelectedPlayer(player_grid_op.guid))
	if (player_grid_info->is_selected)
	{
		playerMove(player_grid_info);
	}
}

void ManageGridOptimize::processRemove(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	//if (isSelectedPlayer(player_grid_op.guid))
	if (player_grid_info->is_selected)
	{
		playerRemove(player_grid_op.guid);
		//m_selected_player.erase(player_grid_op.guid);
		removeSelectedPlayerMap(player_grid_op.guid);
		if (m_not_selected_player.size() > 0)
		{
			Uint64Set_t::iterator it = m_not_selected_player.begin();
			uint64 guid = *it;
			PlayerGridInfo * sel_pgo = getPlayerGridInfo(guid);
			if (NULL != sel_pgo)
			{
				m_not_selected_player.erase(it);
				addSelectedPlayer(sel_pgo);
			}
		}
	}
	else
	{
		m_not_selected_player.erase(player_grid_op.guid);
	}

	removePlayerGridInfo(player_grid_op.guid);
}

void ManageGridOptimize::processLevelup(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	// todo

	player_grid_info->level = player_grid_op.level;
}

void ManageGridOptimize::processFlowerRank(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	// todo

	player_grid_info->flower_rank = player_grid_op.flower_rank;
	if (!player_grid_info->is_selected)
	{
		syncSelectedPlayer(player_grid_info);
	}
	else
	{
		// update the rank info
		typed::protocol::smsg_update_flower_rank suf_rank;
		suf_rank.set_guid(player_grid_info->guid);
		suf_rank.set_flower_rank(player_grid_info->flower_rank);
		Packet * ps = new Packet(SMSG_UPDATE_FLOWER_RANK, player_grid_info->guid, suf_rank.SerializeAsString());
		doBroadcastPacket(ps, true);
		delete ps;
	}
}

void ManageGridOptimize::processUpdateGuildName(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	// todo

	player_grid_info->guild_name = player_grid_op.guild_name;
	player_grid_info->guild_icon = player_grid_op.guild_icon;
	if (player_grid_info->is_selected)
	{
		typed::protocol::smsg_update_other_guild_name sug_name;
		sug_name.set_player_guid(player_grid_info->guid);
		sug_name.set_player_guild_name(player_grid_info->guild_name);
		sug_name.set_player_guild_icon(player_grid_info->guild_icon);

		MAKE_NEW_PACKET(ps, SMSG_UPDATE_OTHER_GUILD_NAME, player_grid_info->guid, sug_name.SerializeAsString());
		doBroadcastPacket(ps, false);
		delete ps;
	}
}

void ManageGridOptimize::processFollowRoleID(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	player_grid_info->follow_role_id = player_grid_op.follow_role_id;
	if (player_grid_info->is_selected)
	{
		typed::protocol::smsg_update_follow_role suf_role;
		suf_role.set_player_guid(player_grid_info->guid);
		suf_role.set_follow_role_id(player_grid_info->follow_role_id);
		MAKE_NEW_PACKET(ps, SMSG_UPDATE_FOLLOW_ROLE, player_grid_info->guid, suf_role.SerializeAsString());

		doBroadcastPacket(ps, false);
		delete ps;
	}
}

void ManageGridOptimize::processUpdateTitle(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	player_grid_info->title_vec = player_grid_op.title_vec;
	
	if (player_grid_info->is_selected)
	{
		typed::protocol::smsg_update_current_title suc_title;
		suc_title.set_player(player_grid_info->guid);
		for (int i = 0; i < player_grid_op.title_vec.size(); ++i)
		{
			suc_title.add_current_title(player_grid_op.title_vec[i]);
		}
		
		MAKE_NEW_PACKET(ps, SMSG_UPDATE_CURRENT_TITLE, player_grid_info->guid, suc_title.SerializeAsString());

		doBroadcastPacket(ps, false);
		delete ps;
	}
}
void ManageGridOptimize::processSpecialRight(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	player_grid_info->special_right = player_grid_op.special_right;

	if (player_grid_info->is_selected)
	{
		typed::protocol::smsg_update_special_right spec_update;
		spec_update.set_player(player_grid_info->guid);
		spec_update.set_special_right(player_grid_op.special_right);
		MAKE_NEW_PACKET(ps, SMSG_UPDATE_SPECIAL_RIGHT, player_grid_info->guid, spec_update.SerializeAsString());

		doBroadcastPacket(ps, false);
		delete ps;
	}
}
void ManageGridOptimize::processHorse(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	player_grid_info->horse_id = player_grid_op.horse_id;

	if (player_grid_info->is_selected)
	{
		typed::protocol::smsg_update_horse_id horse_update;
		horse_update.set_player(player_grid_info->guid);
		horse_update.set_horse_id(player_grid_op.horse_id);
		MAKE_NEW_PACKET(ps, SMSG_UPDATE_HORSE_ID, player_grid_info->guid, horse_update.SerializeAsString());

		doBroadcastPacket(ps, false);
		delete ps;
	}
}
void ManageGridOptimize::processWifeFollow(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	player_grid_info->wife_follow = player_grid_op.wife_follow;

	if (player_grid_info->is_selected)
	{
		typed::protocol::smsg_wife_update_follow wife_update;
		wife_update.set_player(player_grid_info->guid);
		wife_update.set_wife_follow(player_grid_op.wife_follow);
		MAKE_NEW_PACKET(ps, SMSG_WIFE_UPDATE_FOLLOW, player_grid_info->guid, wife_update.SerializeAsString());

		doBroadcastPacket(ps, false);
		delete ps;
	}
}
void ManageGridOptimize::processYellowVipLevel(PlayerGridOperation & player_grid_op)
{
	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		return;
	}

	player_grid_info->yellow_vip_level = player_grid_op.yellow_vip_level;
	player_grid_info->is_yellow_vip = player_grid_op.is_yellow_vip;
	player_grid_info->is_yellow_year_vip = player_grid_op.is_yellow_year_vip;
	player_grid_info->is_yellow_high_vip = player_grid_op.is_yellow_high_vip;

	if (player_grid_info->is_selected)
	{
		typed::protocol::smsg_update_yellow_vip_level yellow_update;
		yellow_update.set_player(player_grid_info->guid);
		yellow_update.set_yellow_vip_level(player_grid_op.yellow_vip_level);
		yellow_update.set_is_yellow_vip(player_grid_op.is_yellow_vip);
		yellow_update.set_is_yellow_year_vip(player_grid_op.is_yellow_year_vip);
		yellow_update.set_is_yellow_high_vip(player_grid_op.is_yellow_high_vip);
		MAKE_NEW_PACKET(ps, SMSG_UPDATE_YELLOW_VIP_LEVEL, player_grid_info->guid, yellow_update.SerializeAsString());

		doBroadcastPacket(ps, false);
		delete ps;
	}
}
void ManageGridOptimize::processBroadcast(PlayerGridOperation & player_grid_op)
{
	if (NULL == player_grid_op.packet)
	{
		return;
	}

	PlayerGridInfo * player_grid_info = getPlayerGridInfo(player_grid_op.guid);
	if (NULL == player_grid_info)
	{
		// error
		doBroadcastPacket(player_grid_op.packet, false);
	}
	else if (player_grid_info->is_selected)
	{
		doBroadcastPacket(player_grid_op.packet, false);
	}
}

PlayerGridInfo * ManageGridOptimize::getPlayerGridInfo(uint64 guid)
{
	PlayerGridInfoMap_t::iterator it = m_player_grid_info_map.find(guid);
	if (it != m_player_grid_info_map.end())
	{
		return it->second;
	}

	return NULL;
}

void ManageGridOptimize::removePlayerGridInfo(uint64 guid)
{
	PlayerGridInfoMap_t::iterator it = m_player_grid_info_map.find(guid);
	if (it != m_player_grid_info_map.end())
	{
		delete it->second;
		m_player_grid_info_map.erase(it);
	}
}

void ManageGridOptimize::createOther(uint64 guid)
{
	typed::protocol::smsg_create_other sc_other;
	for (PlayerGridInfoMap_t::iterator it = m_selected_player.begin(); it != m_selected_player.end(); ++it)
	{
		//PlayerGridInfo * player_grid_info = getPlayerGridInfo(*it);
		PlayerGridInfo * player_grid_info = it->second;
		if (NULL == player_grid_info)
		{
			// error
			continue;
		}

		if (guid == player_grid_info->guid)
		{
			continue;
		}

		fillSmsgCreateOther(player_grid_info, sc_other);
		{
			MAKE_NEW_PACKET(ps, SMSG_CREATE_OTHER, guid, sc_other.SerializeAsString());
			SEND_PLAYER_MESSAGE(m_scene->getMessage(), ps);
		}
	}
}

void ManageGridOptimize::syncSelectedPlayer(PlayerGridInfo * player_grid_info)
{
	if (m_selected_player.size() < m_selected_player_num)
	{
		addSelectedPlayer(player_grid_info);
	}
	else
	{
		m_not_selected_player.insert(player_grid_info->guid);
	}
}

void ManageGridOptimize::fillSmsgCreateOther(PlayerGridInfo * player_grid_info, typed::protocol::smsg_create_other & sc_other)
{
	sc_other.set_guid(player_grid_info->guid);
	sc_other.set_role_id(player_grid_info->role_id);
	sc_other.set_name(player_grid_info->name);
	sc_other.set_level(player_grid_info->level);
	sc_other.set_sex(player_grid_info->sex);
	sc_other.set_src_x(player_grid_info->dst_x);
	sc_other.set_src_y(player_grid_info->dst_y);
	sc_other.set_dst_x(player_grid_info->dst_x);
	sc_other.set_dst_y(player_grid_info->dst_y);
	sc_other.set_direction(player_grid_info->direction);
	sc_other.set_first_role(player_grid_info->first_role);
	sc_other.set_flower_rank(player_grid_info->flower_rank);
	sc_other.set_guild_name(player_grid_info->guild_name);
	sc_other.set_guild_icon(player_grid_info->guild_icon);
	sc_other.set_follow_role_id(player_grid_info->follow_role_id);
	sc_other.clear_current_title();
	for (int i = 0; i < player_grid_info->title_vec.size(); ++i)
	{
		sc_other.add_current_title(player_grid_info->title_vec[i]);
	}
	sc_other.set_special_right(player_grid_info->special_right);
	sc_other.set_horse_id(player_grid_info->horse_id);
	sc_other.set_wife_follow(player_grid_info->wife_follow);
	sc_other.set_yellow_vip_level(player_grid_info->yellow_vip_level);
	sc_other.set_is_yellow_vip(player_grid_info->is_yellow_vip);
	sc_other.set_is_yellow_year_vip(player_grid_info->is_yellow_year_vip);
	sc_other.set_is_yellow_high_vip(player_grid_info->is_yellow_high_vip);

}

void ManageGridOptimize::playerMove(PlayerGridInfo * player_grid_info)
{
	typed::protocol::smsg_player_move sp_move;
	sp_move.set_guid(player_grid_info->guid);
	sp_move.set_src_x(player_grid_info->src_x);
	sp_move.set_src_y(player_grid_info->src_y);
	sp_move.set_dst_x(player_grid_info->dst_x);
	sp_move.set_dst_y(player_grid_info->dst_y);

	for (PlayerGridInfoMap_t::iterator it = m_selected_player.begin(); it != m_selected_player.end(); ++it)
	{
		MAKE_NEW_PACKET(ps, SMSG_PLAYER_MOVE, it->first, sp_move.SerializeAsString());
		SEND_PLAYER_MESSAGE(m_scene->getMessage(), ps);
	}

	for (Uint64Set_t::iterator it = m_not_selected_player.begin(); it != m_not_selected_player.end(); ++it)
	{
		MAKE_NEW_PACKET(ps, SMSG_PLAYER_MOVE, *it, sp_move.SerializeAsString());
		SEND_PLAYER_MESSAGE(m_scene->getMessage(), ps);
	}
}

void ManageGridOptimize::doBroadcastPacket(Packet * packet, bool for_selected_player)
{
	// for_selected_player not used
	
	//MAKE_NEW_PACKET(ps, packet->opcode(), packet->guid(), string(packet->ch_body(), packet->body_size()));
	//SEND_PLAYER_MESSAGE(m_scene->getMessage(), ps);
	for (PlayerGridInfoMap_t::iterator it = m_selected_player.begin(); it != m_selected_player.end(); ++it)
	{
		MAKE_NEW_PACKET(ps, packet->opcode(), it->first, string(packet->ch_body(), packet->body_size()));
		SEND_PLAYER_MESSAGE(m_scene->getMessage(), ps);
	}

	for (Uint64Set_t::iterator it = m_not_selected_player.begin(); it != m_not_selected_player.end(); ++it)
	{
		MAKE_NEW_PACKET(ps, packet->opcode(), *it, string(packet->ch_body(), packet->body_size()));
		SEND_PLAYER_MESSAGE(m_scene->getMessage(), ps);
	}
}

void ManageGridOptimize::playerRemove(uint64 guid)
{
	typed::protocol::smsg_player_remove sp_remove;
	sp_remove.set_guid(guid);

	for (PlayerGridInfoMap_t::iterator it = m_selected_player.begin(); it != m_selected_player.end(); ++it)
	{
		if (guid == it->first)
		{
			continue;
		}

		MAKE_NEW_PACKET(ps, SMSG_PLAYER_REMOVE, it->first, sp_remove.SerializeAsString());
		SEND_PLAYER_MESSAGE(m_scene->getMessage(), ps);
	}

	for (Uint64Set_t::iterator it = m_not_selected_player.begin(); it != m_not_selected_player.end(); ++it)
	{
		if (guid == *it)
		{
			continue;
		}

		MAKE_NEW_PACKET(ps, SMSG_PLAYER_REMOVE, *it, sp_remove.SerializeAsString());
		SEND_PLAYER_MESSAGE(m_scene->getMessage(), ps);
	}
}

//bool ManageGridOptimize::isSelectedPlayer(uint64 guid)
//{
//	Uint64Set_t::iterator it = m_selected_player.find(guid);
//	if (it != m_selected_player.end())
//	{
//		return true;
//	}
//
//	return false;
//}

void ManageGridOptimize::addSelectedPlayer(PlayerGridInfo * player_grid_info)
{
	player_grid_info->is_selected = true;

	m_selected_player.insert(std::make_pair(player_grid_info->guid, player_grid_info));

	typed::protocol::smsg_create_other sc_other;
	fillSmsgCreateOther(player_grid_info, sc_other);

	for (PlayerGridInfoMap_t::iterator it = m_selected_player.begin(); it != m_selected_player.end(); ++it)
	{
		if (it->first == player_grid_info->guid)
		{
			continue;
		}

		{
			MAKE_NEW_PACKET(ps, SMSG_CREATE_OTHER, it->first, sc_other.SerializeAsString());
			SEND_PLAYER_MESSAGE(m_scene->getMessage(), ps);
		}
	}

	for (Uint64Set_t::iterator it = m_not_selected_player.begin(); it != m_not_selected_player.end(); ++it)
	{
		if (*it == player_grid_info->guid)
		{
			continue;
		}

		{
			MAKE_NEW_PACKET(ps, SMSG_CREATE_OTHER, *it, sc_other.SerializeAsString());
			SEND_PLAYER_MESSAGE(m_scene->getMessage(), ps);
		}
	}
}

void ManageGridOptimize::removeSelectedPlayerMap(uint64 player_guid)
{
	PlayerGridInfoMap_t::iterator it = m_selected_player.find(player_guid);
	if (it != m_selected_player.end())
	{
		m_selected_player.erase(it);
	}
	else
	{
		/// error
	}
}
