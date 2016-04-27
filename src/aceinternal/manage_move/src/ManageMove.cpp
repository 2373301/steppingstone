
#include <boost/bind.hpp>
#include "ManageMove.h"
#include "opcode.h"

#define UPDATE_CHECK_TIME 500

IPlugin * make_plugin()
{
	return new ManageMove();
}

ManageMove::ManageMove()
: m_update_timer_id(0)
{
	m_plugin_type = PT_MANAGE_MOVE;
}

ManageMove::~ManageMove()
{
}

int ManageMove::IPlugin_init(const PluginCfg & plugin_cfg)
{
// 	if (NULL == plugin_cfg.scene)
// 	{
// 		return -1;
// 	}

	m_plugin_cfg = plugin_cfg;
	return IPlugin_initing();
}

//int ManageMove::input(const PackInfo & pack_info)
//{
//	return 0;
//}
//
//int ManageMove::notify(const PackInfo & pack_info)
//{
//	return 0;
//}
//
//int ManageMove::request(const PackInfo & pack_info)
//{
//	return 0;
//}

void ManageMove::update()
{
	// do nothing
	return ;

	for (MovePlayerMap_t::iterator it = m_move_player_map.begin(); it != m_move_player_map.end(); )
	{
		if (!move(it->first, it->second))
		{
			m_move_player_map.erase(it++);
		}
		else
		{
			// is arrival
			if (it->second)
			{
				m_move_player_map.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}
}

int ManageMove::cmsg_player_move(const PackInfo & pack_info)
{
// 	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, pack_info.guid, player);
// 	if (NULL == player)
// 	{
// 		return 0;
// 	}
// 
// 	if (!playerCanMove(pack_info.guid))
// 	{
// 		return 0;
// 	}
// 
// 	typed::protocol::cmsg_player_move * role_move = (typed::protocol::cmsg_player_move *)pack_info.msg;
// 
// 	player->set_src_x(player->dst_x());
// 	player->set_src_y(player->dst_y());
// 	player->set_dst_x(role_move->dst_x());
// 	player->set_dst_y(role_move->dst_y());
// 	m_plugin_cfg.manage_grid->move(player);
// 
// 	m_move_player_map[pack_info.guid] = false;

	return 0;
}

int ManageMove::nmsg_player_is_offline(const PackInfo & pack_info)
{
	MovePlayerMap_t::iterator it = m_move_player_map.find(pack_info.guid);
	if (it != m_move_player_map.end())
	{
		m_move_player_map.erase(it);
	}
	return 0;
}

int ManageMove::nmsg_player_is_online(const PackInfo & pack_info)
{
	m_in_stage_player.erase(pack_info.guid);
	return 0;
}

int ManageMove::nmsg_player_jump_map(const PackInfo & pack_info)
{
// 	typed::protocol::nmsg_player_jump_map * jump_map = (typed::protocol::nmsg_player_jump_map *)pack_info.msg;
// 	MOVE_CHECK_NULL_POINT_WITH_LOG_RETURN(jump_map, SE_EPLUGIN_CODE);
// 
// 	if (jump_map->target_map_id() > 99999)
// 	{
// 		m_in_stage_player.insert(pack_info.guid);
// 	}
// 	else
// 	{
// 		m_in_stage_player.erase(pack_info.guid);
// 	}

	return 0;
}

int ManageMove::IPlugin_initing()
{
	//m_update_timer_id = PLUGIN_SCHEME_TIMER(m_plugin_cfg.scene, UPDATE_CHECK_TIME, boost::bind(&ManageMove::update, this));
	return 0;
}

bool ManageMove::move(uint64 player_guid, bool & is_arrival)
{
// 	GET_PLAYER_FROM_POOL(m_plugin_cfg.pool, player_guid, role);
// 	if (NULL == role)
// 	{
// 		return false;
// 	}
// 
// 	//float speed_value = 10;
// 
// 	float x = role->dst_x() - role->src_x();
// 	float y = role->dst_y() - role->src_y();
// 
// 	//float speed_value = 6000.0 * 0.001f * 0.7f;
// 	float speed_value = 300;
// 
// 
// 	float run_len = speed_value * UPDATE_CHECK_TIME / 1000;
// 
// 	if (run_len < 0.1)
// 	{
// 		return true;
// 	}
// 
// 	float diff_len = sqrt(x * x + y * y);
// 
// 	if (diff_len > run_len)
// 	{
// 		x = x * run_len / diff_len;
// 		y = y * run_len / diff_len;
// 		role->set_src_x(role->src_x() + x);
// 		role->set_src_y(role->src_y() + y);
// 	}
// 	else
// 	{
// 		role->set_src_x(role->dst_x());
// 		role->set_src_y(role->dst_y());
// 		is_arrival = true;
// 	}

	return true;
}

bool ManageMove::playerCanMove(uint64 player_guid)
{
	Uint64Set_t::iterator it = m_in_stage_player.find(player_guid);
	return it != m_in_stage_player.end() ? false : true;
}