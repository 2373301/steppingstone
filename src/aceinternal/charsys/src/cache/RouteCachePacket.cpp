
#include <boost/lexical_cast.hpp>
#include <ace/OS.h>
#include "RouteCachePacket.h"
#include "ManageConfig.h"
#include "cachesys/inc/cachesys_opcode.hpp"
#include "opcode.h"
#include "coredef.h"
#include "RouteGatePacket.h"
#include "ManageGuid.h"
#include "ManagePlayerInit.h"
#include "ManageJobInit.h"
#include "ManageRoleRes.h"
#include "protocol/msg_cache.pb.h"

RouteCachePacket::RouteCachePacket()
: m_stop(false)
, m_rand_engineer(ACE_OS::gettimeofday().sec())
, m_rand_distribution(20, 500)
, m_rand_generator(m_rand_engineer, m_rand_distribution)
{

}

RouteCachePacket::~RouteCachePacket()
{

}

int RouteCachePacket::init()
{
	if (m_manage_cache_session.init(ManageConfig::instance()->getCacheConfig()) == -1)
	{
		return -1;
	}

	m_manage_cache_session.setHandleInput(this);

	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

int RouteCachePacket::stop()
{
	return 0;
}

int RouteCachePacket::finit()
{
	return 0;
}

void RouteCachePacket::input(Packet * packet)
{
	RouteGatePacket::instance()->handleCachePacket(packet);
}

int RouteCachePacket::svc(void)
{
	ACE_Time_Value start_time;
	ACE_Time_Value time_diff;
	uint64 time_interval = 0;

	ACE_Time_Value pack_start_time;
	ACE_Time_Value pack_time_diff;
	uint64 pack_time = 0;

	PacketList_t packet_stream_list;
	while (!m_stop)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_input_mutex, -1);
			std::copy(m_input_queue.begin(), m_input_queue.end(), std::back_inserter(packet_stream_list));
			m_input_queue.clear();
		}

		if (packet_stream_list.size() == 0)
		{
			ACE_OS::sleep(ACE_Time_Value(0, 1000));
			continue;
		}

		start_time = ACE_OS::gettimeofday();
		for (PacketList_t::iterator it = packet_stream_list.begin(); it != packet_stream_list.end(); ++it)
		{
			pack_start_time = ACE_OS::gettimeofday();

			Packet * packet = *it;

			doHandlePacket(packet);

			pack_time_diff = ACE_OS::gettimeofday() - pack_start_time;
			pack_time_diff.to_usec(pack_time);
			if (pack_time > 1000)
			{
				DEF_LOG_ERROR("cache pack process time error, opcode is <%d>, guid is <%llu>, spend time is <%llu>\n", packet->opcode(), packet->guid(), pack_time);
			}

			delete packet;
		}

		if (packet_stream_list.size() > 100)
		{
			time_diff = ACE_OS::gettimeofday() - start_time;
			time_diff.to_usec(time_interval);
			DEF_LOG_INFO("process cache packet, size is <%d>, spend time is <%llu>\n", packet_stream_list.size(), time_interval);
		}

		packet_stream_list.clear();
	}
	return 0;
}

int RouteCachePacket::handleClientPacket(Packet * packet_stream)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_input_mutex, -1);
	m_input_queue.push_back(packet_stream);
	return 0;
}

int RouteCachePacket::doHandlePacket(Packet * packet_stream)
{
	int op_code = packet_stream->opcode();
	int handle_result = 0;
	if (CMSG_PLAYER_MAKE == op_code)
	{
		handle_result = handleRoleMake(packet_stream);
	}
	else if (CMSG_PLAYER_DELETE == op_code)
	{
		handle_result = handleRoleDelete(packet_stream);
	}
	else if (DCMSG_LOAD_ENTITY == op_code)
	{
		handle_result = handleLoadEntity(packet_stream);
	}
	else
	{
		DEF_LOG_ERROR("get unknown opcode, guid is <%llu>, opcode is <%d>\n", packet_stream->guid(), packet_stream->opcode());
	}

	return handle_result;
}


int RouteCachePacket::handleRoleMake(Packet * packet_stream)
{
	typed::protocol::cmsg_player_make role_make;
	//if (packet_stream->parse_protocol(role_make))
	if (PARSE_PACKET(packet_stream, &role_make))
	{
		typed::entity::Role role;
		if (!initRole(packet_stream->guid(), role_make, role))
		{
			DEF_LOG_ERROR("failed to inti role by guid <%llu>\n", packet_stream->guid());
			return -1;
		}

		protocol::db_data_request db_request;
		db_request.set_transaction_index(0);
		db_request.set_transaction_num(0);
		db_request.set_transaction_id(0);
		db_request.set_line_id(0);
		db_request.set_data_stream(role.SerializeAsString());

		MAKE_NEW_PACKET(ps, DCMSG_NEW_ENEITY, role.guid(), db_request.SerializeAsString());
		m_manage_cache_session.output(ps, packet_stream->guid());

		typed::entity::Player player;
		player.set_role_id(role.role_id());
		if (!initPlayer(packet_stream->guid(), role_make, player, role.guid()))
		{
			DEF_LOG_ERROR("failed to init player by guid <%llu>\n", packet_stream->guid());
			return -1;
		}

		db_request.set_data_stream(player.SerializeAsString());

		MAKE_NEW_PACKET_NO_DEC(ps, DCMSG_NEW_ENEITY, player.guid(), db_request.SerializeAsString());

		m_manage_cache_session.output(ps, packet_stream->guid());

	}
	return 0;
}

int RouteCachePacket::handleRoleDelete(Packet * packet_stream)
{
	// do nothing currently
	return 0;

	//protocol::db_data_request db_request;
	//db_request.set_transaction_index(0);
	//db_request.set_transaction_num(0);
	//db_request.set_transaction_id(0);
	//db_request.set_line_id(0);
	//db_request.set_data_stream("");

	//MAKE_NEW_PACKET(ps, DCMSG_REMOVE_ENTITY, packet_stream->guid(), db_request.SerializeAsString());

	//m_manage_cache_session.output(ps);

	//return 0;
}


int RouteCachePacket::handleLoadEntity(Packet * packet_stream)
{
	protocol::db_data_request db_request;
	db_request.set_transaction_index(0);
	db_request.set_transaction_num(0);
	db_request.set_transaction_id(0);
	db_request.set_data_stream("");
	db_request.set_line_id(0);

	MAKE_NEW_PACKET(ps, DCMSG_LOAD_ENTITY, packet_stream->guid(), db_request.SerializeAsString());

	m_manage_cache_session.output(ps, packet_stream->guid());

	return 0;
}

bool RouteCachePacket::initPlayer(uint64 guid, const typed::protocol::cmsg_player_make & role_make, typed::entity::Player & player, uint64 first_role_guid)
{
	player.set_guid(guid);
	player.set_name(role_make.name());
	player.set_profession(role_make.profession());
	// todo
	player.set_line_no(ManageConfig::instance()->getLineId());

	InitializationCfg & init_cfg = ManagePlayerInit::instance()->getPlayerInitCfg();
	player.set_level(init_cfg.level);
	player.set_exp(init_cfg.exp);
	player.set_money(init_cfg.money);
	player.set_coin(init_cfg.coin);
	player.set_src_x(init_cfg.src_x);
	player.set_src_y(init_cfg.src_y);
	player.set_dst_x(init_cfg.src_x);
	player.set_dst_y(init_cfg.src_y);
	player.set_seniority(init_cfg.seniority);
	//player.set_vip_level(init_cfg.)
	player.set_vip_level(0);
	player.set_sex(role_make.sex());
	player.set_account(role_make.account());

	player.set_coin_exp(init_cfg.coin_exp);
	player.set_prestige(init_cfg.frame);
	player.set_energy(init_cfg.energy);
	//player.set_power(init_cfg.)
	player.set_power(0);

	player.set_training_slot(init_cfg.training_slot);
	player.set_member_slot(init_cfg.member_slot);
//	player.set_curr_battle_array(init_cfg.curr_battle_array);
	player.set_map_id(init_cfg.map_id);
	//player.set_stage(init_cfg.)	
	player.set_stage(0);
	//player.set_direction(init_cfg.)
	player.set_direction(0);
	player.add_roles(first_role_guid);
	player.set_first_role(first_role_guid);
	player.set_validate_str(role_make.validatestr());
	player.set_vip_level_init(role_make.param1());
	player.set_year_card(role_make.param2());
	player.set_time_stamp(role_make.param3());
	player.set_robot_validate(role_make.param4());
	player.set_robot_id(role_make.param5());
	player.set_open_id_referrer(role_make.iopenid());
	player.set_open_id_qq(role_make.account());
	for (int i = 0; i < MAX_BAG_NUMBER; ++i)
	{
		if (i < INIT_BAG_NUMBER)
		{
			player.add_bag(1);
		}
		else
		{
			player.add_bag(0);
		}
	}

	for (int i = 0; i < MAX_STOREHOUSE_NUMBER; ++i)
	{
		if (i < INIT_STOREHOUSE_NUMBER)
		{
			player.add_storehouse(1);
		}
		else
		{
			player.add_storehouse(0);
		}
	}


	//  init repeated field

	return true;
}

bool RouteCachePacket::initRole(uint64 player_guid, const typed::protocol::cmsg_player_make & role_make, typed::entity::Role & role)
{
	uint64 role_guid = 0;
	RoleInfo * role_info = RouteGatePacket::instance()->getPlayerInfo(player_guid);
	if (NULL == role_info)
	{
		DEF_LOG_ERROR("failed to find role info by player guid <%llu>\n", player_guid);
		return false;
	}

	int role_id = ManageJobInit::instance()->getRoleId(role_make.profession(), role_make.sex());
	if (-1 == role_id)
	{
		DEF_LOG_ERROR("failed to get role id by job : <%d>, sex : <%d>\n", role_make.profession(), role_make.sex());
		return false;
	}

	RoleResInfo * role_res_info = ManageRoleRes::instance()->getRoleResInfo(role_id);
	if (NULL == role_res_info)
	{
		DEF_LOG_ERROR("error to get role res info by role id : <%d>\n", role_id);
		return false;
	}

	for (int i = 0; i < 8; ++i)
	{
		role.add_equipments(0);
	}

	role.set_guid(role_info->role_guid);
	role.set_name(role_make.name());
	role.set_role_id(role_res_info->role_id);
	role.set_avatar_id(role_res_info->role_id);

	role.set_player(player_guid);
	role.set_sex(role_make.sex());
	role.set_job(role_make.profession());
	role.set_faction(0);
	role.set_level(role_res_info->level);
	role.set_exp(0);
	role.set_postion(0);
	role.set_normal_skill(role_res_info->normal_skill);
	role.set_extremely_skill(role_res_info->extremely_skill);
	role.set_char_ability(role_res_info->char_ability);
	
	
	
	role.add_attributes(0);	// 0, nothing
	role.add_attributes(role_res_info->hp);
	role.add_attributes(role_res_info->hp);
	role.add_attributes(role_res_info->str);
	role.add_attributes(role_res_info->agi);
	role.add_attributes(role_res_info->int_value);
	role.add_attributes(role_res_info->phy_att);		// 外功攻击基础值
	role.add_attributes(0);		
	role.add_attributes(role_res_info->phy_att);		// 外功攻击值
	role.add_attributes(role_res_info->agi_att);		// 内功攻击基础值
	role.add_attributes(0);
	role.add_attributes(role_res_info->agi_att);		// 内功攻击
	role.add_attributes(role_res_info->int_att);		// 奇术攻击基础值
	role.add_attributes(0);
	role.add_attributes(role_res_info->agi_att);		// 奇术攻击
	role.add_attributes(role_res_info->phy_def);		// 外功防御基础值
	role.add_attributes(0);
	role.add_attributes(role_res_info->phy_def);		// 外功防御
	role.add_attributes(role_res_info->agi_def);		// 内功防御基础值
	role.add_attributes(0);
	role.add_attributes(role_res_info->agi_def);		// 内功防御
	role.add_attributes(role_res_info->int_def);		// 奇术防御基础值
	role.add_attributes(0);
	role.add_attributes(role_res_info->int_def);		// 奇术防御
	role.add_attributes(role_res_info->critical);		// 暴击
	role.add_attributes(role_res_info->critical_damage);		// 暴击倍数
	role.add_attributes(role_res_info->block);		// 格挡
	role.add_attributes(role_res_info->block_defence);		// 格挡减免伤害倍率
	role.add_attributes(role_res_info->block_damage);		// 格挡伤害倍数
	role.add_attributes(role_res_info->dodge);		// 闪避
	role.add_attributes(role_res_info->hit);		// 命中
	role.add_attributes(0);		// 武力值
	role.add_attributes(0);		// 气势
	role.add_attributes(0);		// 反击
	role.add_attributes(role_res_info->hp);
	role.add_attributes(0);

	return true;
}