//#include "ManageGridImp.h"
//#include "entity_ins.h"
//#include "protocol_ins.h"
//#include "opcode.h"
//
//ManageGridImp::ManageGridImp()
//: m_scene_cfg(NULL)
//, m_scene(NULL)
//, m_pool(NULL)
//{	
//}
//
//int ManageGridImp::init(SceneCfg * scene_cfg, Scene * scene, Pool * pool, vector<int> & map_id_vec)
//{
//	m_scene_cfg = scene_cfg;
//	m_scene = scene;
//	m_pool = pool;
//
//	return 0;
//}
//
//void ManageGridImp::init(vector<std::pair<int, int> > & map_ids)
//{
//	MapEntity map_e;
//	for (vector<std::pair<int, int> >::iterator it = map_ids.begin(); it != map_ids.end(); ++it)
//	{
//		map_e.map_type = it->second;
//		m_map_entity.insert(std::make_pair(it->first, map_e));
//	}
//}
//
//void ManageGridImp::add(typed::entity::Player * player)
//{
//	if (NULL == player)
//	{
//		return ;
//	}
//
//	MapEntityMap_t::iterator it = m_map_entity.find(player->map_id());
//	if (it != m_map_entity.end())
//	{
//		MapEntity & map_e = it->second;
//		if (!isNotifyOther(map_e.map_type))
//		{
//			map_e.players.insert(player->guid());
//			return ;
//		}
//
//		// notify other
//		if (IS_PLAYER_GUID(player->guid()))
//		{
//			GuidSet_t guid_players;
//			search(ET_PLAYER, map_e, guid_players);
//			typed::protocol::smsg_create_other other_create;
//			// nottify other
//			for (GuidSet_t::iterator it = guid_players.begin(); it != guid_players.end(); ++it)
//			{
//
//				if (fillCreateOther(player, &other_create))
//				{
//					MAKE_NEW_PACKET(ps, SMSG_CREATE_OTHER, *it, other_create.SerializeAsString());
//					SEND_PLAYER_MESSAGE(m_scene, ps);
//					//m_scene_cfg->manage_terminal->output(ps);
//				}
//			}
//
//			// notify this player
//			typed::entity::Player * per;
//			for (GuidSet_t::iterator it = guid_players.begin(); it != guid_players.end(); ++it)
//			{
//				//per = (typed::entity::Player *)m_pool->get(*it);
//				GET_PLAYER_FROM_POOL(m_pool, *it, per);
//				if (NULL == per)
//				{
//					DEF_LOG_ERROR("error to get player form pool, guid is <%s>\n", boost::lexical_cast<string>(*it).c_str());
//					continue;
//				}
//				//CHECK_NULL_POINT_WITH_LOG_RETURN(per, );
//
//				if (fillCreateOther(per, &other_create))
//				{
//					//GET_ROLE_FROM_POOL(m_pool, per->roles(0), per_role);
//					//other_create.set_guid(per->guid());
//					//other_create.set_name(per->name());
//					//other_create.set_level(per->level());
//					//other_create.set_src_x(per->src_x());
//					//other_create.set_src_y(per->src_y());
//					//other_create.set_dst_x(per->dst_x());
//					//other_create.set_dst_y(per->dst_y());
//					//other_create.set_role_id(per_role->role_id());
//
//					MAKE_NEW_PACKET(ps, SMSG_CREATE_OTHER, player->guid(), other_create.SerializeAsString());
//					SEND_PLAYER_MESSAGE(m_scene, ps);
//					//m_scene_cfg->manage_terminal->output(ps);
//				}
//			}
//			map_e.players.insert(player->guid());
//		}
//		else if (IS_MONSTOR_GUID(player->guid()))
//		{
//			map_e.monsters.insert(player->guid());
//		}
//	}
//	else
//	{
//		// error , should never happen
//	}
//}
//
//void ManageGridImp::readd(typed::entity::Player * player)
//{
//	if (NULL == player)
//	{
//		return ;
//	}
//
//	MapEntityMap_t::iterator map_it = m_map_entity.find(player->map_id());
//	if (m_map_entity.end() == map_it)
//	{
//		// warning
//		return ;
//	}
//
//	MapEntity & map_e = map_it->second;
//
//	if (map_e.players.find(player->guid()) == map_e.players.end())
//	{
//		this->add(player);
//		return ;
//	}
//
//	// notify other
//	if (IS_PLAYER_GUID(player->guid()))
//	{
//		GuidSet_t guid_players;
//		search(ET_PLAYER, map_e, guid_players);
//		typed::protocol::smsg_create_other other_create;
//		// notify this player
//		typed::entity::Player * per;
//		for (GuidSet_t::iterator it = guid_players.begin(); it != guid_players.end(); ++it)
//		{
//			if (player->guid() == *it)
//			{
//				continue;
//			}
//
//			GET_PLAYER_FROM_POOL(m_pool, *it, per);
//			if (NULL == per)
//			{
//				DEF_LOG_ERROR("error to get player form pool, guid is <%s>\n", boost::lexical_cast<string>(*it).c_str());
//				continue;
//			}
//			//CHECK_NULL_POINT_WITH_LOG_RETURN(per, );
//
//			if (fillCreateOther(per, &other_create))
//			{
//				//GET_ROLE_FROM_POOL(m_pool, per->roles(0), per_role);
//				//other_create.set_guid(per->guid());
//				//other_create.set_name(per->name());
//				//other_create.set_level(per->level());
//				//other_create.set_src_x(per->src_x());
//				//other_create.set_src_y(per->src_y());
//				//other_create.set_dst_x(per->dst_x());
//				//other_create.set_dst_y(per->dst_y());
//				//other_create.set_role_id(per_role->role_id());
//
//				MAKE_NEW_PACKET(ps, SMSG_CREATE_OTHER, player->guid(), other_create.SerializeAsString());
//				SEND_PLAYER_MESSAGE(m_scene, ps);
//				//m_scene_cfg->manage_terminal->output(ps);
//			}
//		}
//	}
//}
//
//void ManageGridImp::remove(typed::entity::Player * player)
//{
//	if (NULL == player)
//	{
//		return ;
//	}
//
//	MapEntityMap_t::iterator it = m_map_entity.find(player->map_id());
//	if (it != m_map_entity.end())
//	{
//		MapEntity & map_e = it->second;
//
//		if (!isNotifyOther(map_e.map_type))
//		{
//			map_e.players.erase(player->guid());
//			return ;
//		}
//
//		if (IS_PLAYER_GUID(player->guid()))
//		{
//			map_e.players.erase(player->guid());
//
//			GuidSet_t guid_players;
//			search(ET_PLAYER, map_e, guid_players);
//			for (GuidSet_t::iterator it = guid_players.begin(); it != guid_players.end(); ++it)
//			{
//				typed::protocol::smsg_player_remove remove_other;
//				remove_other.set_guid(player->guid());
//
//				MAKE_NEW_PACKET(ps, SMSG_PLAYER_REMOVE, *it, remove_other.SerializeAsString());
//				SEND_PLAYER_MESSAGE(m_scene, ps);
//				//m_scene_cfg->manage_terminal->output(ps);
//			}
//		}
//		else if (IS_MONSTOR_GUID(player->guid()))
//		{
//			map_e.monsters.insert(player->guid());
//		}
//	}
//	else
//	{
//		// error , should never happen
//	}
//}
//
//void ManageGridImp::search(EntityType entity_type, MapEntity & map_entity, GuidSet_t & guids)
//{
//	if (ET_PLAYER == entity_type)
//	{
//		for (GuidSet_t::iterator it = map_entity.players.begin(); it != map_entity.players.end(); ++it)
//		{
//			guids.insert(*it);
//		}
//	}
//}
//
//void ManageGridImp::search(EntityType entity_type, int map_id, GuidSet_t & guids)
//{
//	MapEntityMap_t::iterator it = m_map_entity.find(map_id);
//	if (it != m_map_entity.end())
//	{
//		if (isNotifyOther(it->second.map_type))
//		{
//			search(entity_type, it->second, guids);
//		}
//	}
//}
//
//bool ManageGridImp::isNotifyOther(int map_type)
//{
//	if (1 == map_type)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//bool ManageGridImp::fillCreateOther(typed::entity::Player * player, typed::protocol::smsg_create_other * other_create)
//{
//	GET_ROLE_FROM_POOL(m_pool, player->roles(0), role_ins);
//	if (NULL == role_ins)
//	{
//		// error 
//		return false;
//	}
//
//	other_create->set_guid (player->guid());
//	other_create->set_role_id(role_ins->role_id());
//	other_create->set_name (player->name ());
//	other_create->set_level(player->level());
//	//other_create->set_image(player->image());
//	other_create->set_src_x(player->src_x());
//	other_create->set_src_y(player->src_y());
//	other_create->set_dst_x(player->dst_x());
//	other_create->set_dst_y(player->dst_y());
//	other_create->set_first_role(player->roles(0));
//
//	return true;
//}