
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "ManageCrossPlayerData.h"
#include "protocol/msg_cross.pb.h"
#include "entity/player.pb.h"

#define CROSS_PLAYER_PATH "./cross_player_data/"

void searchDir(const string & search_dir, vector<string> & searched_file)
{
	boost::filesystem::path parent_path(search_dir);
	if (!boost::filesystem::is_directory(parent_path))
	{
		return ;
	}

	boost::filesystem::directory_iterator end_it;
	for (boost::filesystem::directory_iterator it(parent_path); it != end_it; ++it)
	{
		boost::filesystem::path seached_path = *it;
		if (boost::filesystem::is_directory(seached_path))
		{
			string file_dir = seached_path.leaf().string();
			if ((file_dir != ".") && (file_dir != ".."))
			{
				searched_file.push_back(it->path().filename().string());
			}
		}
	}
}

ManageCrossPlayerData::ManageCrossPlayerData()
{

}

ManageCrossPlayerData::~ManageCrossPlayerData()
{

}

int ManageCrossPlayerData::init(const SceneCfg * scene_cfg)
{
	m_scene_cfg = (SceneCfg *)scene_cfg;
	initCrossPlayerData();
	return 0;
}

void ManageCrossPlayerData::collectCrossPlayerData(const string & line_id, const string & platform, uint64 player_guid, uint64 entity_guid, const string & entity_stream)
{
	string key = getKey(line_id, platform, player_guid);
	CrossPlayerDataInfoMap_t::iterator it = m_cross_player_data_info_map.find(key);
	CrossPlayerDataInfo * cross_pd_info = NULL;
	if (m_cross_player_data_info_map.end() != it)
	{
		cross_pd_info = it->second;
	}
	else
	{
		cross_pd_info = new CrossPlayerDataInfo();
		cross_pd_info->platform = platform;
		cross_pd_info->line_id = line_id;
		cross_pd_info->player_guid = player_guid;
		cross_pd_info->key = key;
		m_cross_player_data_info_map[key] = cross_pd_info;
	}
	MSG_TYPE * entity_msg = NULL;
	if (extractEntity(entity_guid, entity_stream, &entity_msg))
	{
		cross_pd_info->msg_map[entity_guid] = entity_msg;
	}
	else
	{
		DEF_LOG_ERROR("failed to extract entity, player is <%llu>, guid is <%llu>\n", player_guid, entity_guid);
	}
}

void ManageCrossPlayerData::collectCrossPlayerDataFinish(const string & line_id, const string & platform, uint64 player_guid)
{
	string key = getKey(line_id, platform, player_guid);

	CrossPlayerDataInfoMap_t::iterator it = m_cross_player_data_info_map.find(key);
	if (it != m_cross_player_data_info_map.end())
	{
		CrossPlayerDataInfo * cross_pd_info = it->second;
		m_cross_player_guid_data_info_map[player_guid] = cross_pd_info;
		saveData(cross_pd_info);
	}
}

void ManageCrossPlayerData::deliveryCrossPlayerDataToGS()
{
	for (CrossPlayerDataInfoMap_t::iterator it = m_cross_player_data_info_map.begin(); it != m_cross_player_data_info_map.end(); ++it)
	{
		CrossPlayerDataInfo * cross_pd_info = it->second;
		deliveryCrossPlayerDataToGS(cross_pd_info);
	}
}

void ManageCrossPlayerData::deliveryCrossPlayerDataToGS(const string & platform, const string & line_id)
{
	for (CrossPlayerDataInfoMap_t::iterator it = m_cross_player_data_info_map.begin(); it != m_cross_player_data_info_map.end(); ++it)
	{
		CrossPlayerDataInfo * cross_pd_info = it->second;
		deliveryCrossPlayerDataToGS(platform, line_id, cross_pd_info);
	}	
}

//void ManageCrossPlayerData::lookupCrossPlayerInfo(uint64 observer, uint64 player_guid)
//{
//	CrossPlayerGuidDataInfoMap_t::iterator it = m_cross_player_guid_data_info_map.find(player_guid);
//	if (it != m_cross_player_guid_data_info_map.end())
//	{
//		CrossPlayerDataInfo * cross_pd_info = it->second;
//		PoolImp pool;
//		for (CrossPlayerDataInfo::MsgMap_t::iterator it = cross_pd_info->msg_map.begin(); it != cross_pd_info->msg_map.end(); ++it)
//		{
//			pool.add(it->first, it->second, false, cross_pd_info->player_guid);
//		}
//		typed::entity::Player * player = (typed::entity::Player *)pool.get(cross_pd_info->player_guid);
//		
//		sendLookupPlayer(observer, player);
//		sendLookupRole(observer, player);
//		sendLookupEquipment(observer, player);
//		sendLookupRunes(observer, player);
//		sendLookupGuidName(observer, player);
//		sendLookupDodge(observer, player);
//	}
//}

void ManageCrossPlayerData::getCrossPlayerDataInfo(uint64 player_guid, map<uint64, MSG_TYPE *> & msg_map)
{
	CrossPlayerGuidDataInfoMap_t::iterator it = m_cross_player_guid_data_info_map.find(player_guid);
	if (it != m_cross_player_guid_data_info_map.end())
	{
		CrossPlayerDataInfo * cross_pd_info = it->second;
		for (CrossPlayerDataInfo::MsgMap_t::iterator it = cross_pd_info->msg_map.begin(); it != cross_pd_info->msg_map.end(); ++it)
		{
			msg_map[it->first] = it->second;
		}
	}
}

void ManageCrossPlayerData::initCrossPlayerData()
{
	boost::filesystem::path path(CROSS_PLAYER_PATH);

	if (boost::filesystem::exists(path))
	{
		loadCrossPlayerData();
	}
	else
	{
		boost::filesystem::create_directory(path);
	}
}

void ManageCrossPlayerData::loadCrossPlayerData()
{
	boost::filesystem::path path(CROSS_PLAYER_PATH);

	vector<string> platform_vec;
	vector<string> line_id_vec;
	vector<string> player_vec;
	
	searchDir(CROSS_PLAYER_PATH, platform_vec);
	for (vector<string>::iterator it = platform_vec.begin(); it != platform_vec.end(); ++it)
	{
		string platform_path = CROSS_PLAYER_PATH;
		platform_path += *it;
		line_id_vec.clear();
		searchDir(platform_path, line_id_vec);
		string line_path = platform_path + "/";
		for (vector<string>::iterator sub_it = line_id_vec.begin(); sub_it != line_id_vec.end(); ++sub_it)
		{
			line_path += *sub_it + "/";
			player_vec.clear();
			searchDir(line_path, player_vec);
			for (vector<string>::iterator player_it = player_vec.begin(); player_it != player_vec.end(); ++player_it)
			{
				string player_path = line_path + *player_it;
				loadCrossPlayerData(*it, *sub_it, player_path);
			}
		}
	}
}

void ManageCrossPlayerData::loadCrossPlayerData(const string & platform, const string & line_id, const string & line_path)
{
	vector<string> file_vec;
	searchFileByExt(line_path, ".data", file_vec);
	CrossPlayerDataInfo * cross_pd_info = new CrossPlayerDataInfo();
	cross_pd_info->platform = platform;
	cross_pd_info->line_id = line_id;
	for (vector<string>::iterator it = file_vec.begin(); it != file_vec.end(); ++it)
	{
		string msg_stream;
		loadStream(*it, msg_stream);
		boost::filesystem::path entity_path(*it);
		uint64 entity_guid = extractGuid(entity_path.filename().string());
		MSG_TYPE * entity_msg = NULL;
		if (extractEntity(entity_guid, msg_stream, &entity_msg))
		{
			if (IS_PLAYER_GUID(entity_guid))
			{
				cross_pd_info->player_guid = entity_guid;

				if (m_scene_cfg->line_scene->isCrossServer())
				{
					typed::entity::Player & player = *(typed::entity::Player *)entity_msg;

					uint64 short_guid = SWITCH_GUID_TYPE(ET_SHORT, entity_guid);
					typed::entity::Short * short_info = new typed::entity::Short();

					short_info->set_guid(short_guid);
					short_info->set_name(player.name());
					short_info->set_level(player.level());
					short_info->set_sex(player.sex());
					short_info->set_job(player.profession());
					short_info->set_role_id(player.role_id());
					short_info->set_player(entity_guid);
					short_info->set_curr_map_id(player.map_id());
					short_info->set_line_id(player.line_no());
					short_info->set_platform(player.platform());
					short_info->set_fight_power(player.power());

					for (int i = 0; i < 3; ++i)
					{
						short_info->add_boss_id(0);
						short_info->add_boss_ranking(0);
					}

					m_scene_cfg->line_scene->updatePlayerShortInfo(entity_guid, short_info);
				}
			}

			cross_pd_info->msg_map[entity_guid] = entity_msg;
		}
	}

	string key = getKey(cross_pd_info->platform, cross_pd_info->line_id, cross_pd_info->player_guid);

	m_cross_player_data_info_map[key] = cross_pd_info;
	m_cross_player_guid_data_info_map[cross_pd_info->player_guid] = cross_pd_info;
}

string ManageCrossPlayerData::getKey(const string & line_id, const string & platform, uint64 player_guid)
{
	string player_guid_str = boost::lexical_cast<string>(player_guid);
	string key = platform + line_id + player_guid_str;
	return key;
}

void ManageCrossPlayerData::saveData(CrossPlayerDataInfo * cross_player_data_info)
{
	createDir(CROSS_PLAYER_PATH);
	createDir(CROSS_PLAYER_PATH + cross_player_data_info->platform);
	string line_dir = CROSS_PLAYER_PATH + cross_player_data_info->platform + "/" + cross_player_data_info->line_id + "/";
	createDir(line_dir);
	string player_dir = line_dir + boost::lexical_cast<string>(cross_player_data_info->player_guid) + "/";
	createDir(player_dir);

	for (CrossPlayerDataInfo::MsgMap_t::iterator it = cross_player_data_info->msg_map.begin(); it != cross_player_data_info->msg_map.end(); ++it)
	{
		string file_name = player_dir + boost::lexical_cast<string>(it->first);
		saveFile(file_name, it->second->SerializeAsString());
	}
}

void ManageCrossPlayerData::saveFile(const string & file_name, const string & file_stream)
{
	string file_str = file_name + ".data";
	fstream fs;
	fs.open(file_str.c_str(), ios_base::trunc | ios_base::out | ios_base::binary);
	fs.rdbuf()->sputn(file_stream.c_str(), file_stream.size());

	fs.flush();
	fs.close();
}

void ManageCrossPlayerData::createDir(const string & file_dir)
{
	boost::filesystem::path path(file_dir);
	if (!boost::filesystem::exists(path))
	{
		boost::filesystem::create_directory(path);
	}
}

void ManageCrossPlayerData::loadStream(const string & file_name, string & msg_stream)
{
	fstream file_stream;
	file_stream.open(file_name.c_str(), ios_base::in | ios_base::binary);
	file_stream.seekg(0, ios::end);
	int buffer_size = file_stream.tellg();
	file_stream.seekg(0, ios::beg);
	char * buffer_stream = (char *)malloc(buffer_size);
	file_stream.read(buffer_stream, buffer_size);

	msg_stream = string(buffer_stream, buffer_size);

	file_stream.flush();
	file_stream.close();
}

uint64 ManageCrossPlayerData::extractGuid(const string & file_name)
{
	uint64 guid = 0;
	char * str = (char *)file_name.c_str();
	char * pos = strstr(str, ".");
	if (pos != NULL)
	{
		pos[0] = '\0';
		guid = boost::lexical_cast<uint64>(str);
		pos[0] = '.';
	}

	return guid;
}

void ManageCrossPlayerData::deliveryCrossPlayerDataToGS(CrossPlayerDataInfo * cross_pd_info)
{
	Packet * ps = NULL;

	typed::protocol::smsg_cross_deliver_player_data_start scd_start;
	scd_start.set_platform(cross_pd_info->platform);
	scd_start.set_line_id(cross_pd_info->line_id);
	scd_start.set_player_guid(cross_pd_info->player_guid);

	ps = new Packet(SMSG_DELIVERY_CROSS_PLAYER_DATA_START, cross_pd_info->player_guid, scd_start.SerializeAsString());
	m_scene_cfg->cross_server->broadLineServerMsg(ps);

	typed::protocol::smsg_cross_deliver_player_data scd_pd;

	for (CrossPlayerDataInfo::MsgMap_t::iterator it = cross_pd_info->msg_map.begin(); it != cross_pd_info->msg_map.end(); ++it)
	{
		if (it->second != NULL)
		{
			scd_pd.set_player_guid(cross_pd_info->player_guid);
			scd_pd.set_entity_guid(it->first);
			scd_pd.set_line_id(cross_pd_info->line_id);
			scd_pd.set_platform(cross_pd_info->platform);
			scd_pd.set_entity_stream(it->second->SerializeAsString());
			ps = new Packet(SMSG_DELIVERY_CROSS_PLAYER_DATA, cross_pd_info->player_guid, scd_pd.SerializeAsString());
			m_scene_cfg->cross_server->broadLineServerMsg(ps);
		}
	}

	typed::protocol::smsg_cross_deliver_player_data_end scd_end;
	scd_end.set_platform(cross_pd_info->platform);
	scd_end.set_line_id(cross_pd_info->line_id);
	scd_end.set_player_guid(cross_pd_info->player_guid);
	ps = new Packet(SMSG_DELIVERY_CROSS_PLAYER_DATA_END, cross_pd_info->player_guid, scd_end.SerializeAsString());

	m_scene_cfg->cross_server->broadLineServerMsg(ps);
}

void ManageCrossPlayerData::deliveryCrossPlayerDataToGS(const string & platform, const string & line_id, CrossPlayerDataInfo * cross_pd_info)
{
	Packet * ps = NULL;

	typed::protocol::smsg_cross_deliver_player_data_start scd_start;
	scd_start.set_platform(cross_pd_info->platform);
	scd_start.set_line_id(cross_pd_info->line_id);
	scd_start.set_player_guid(cross_pd_info->player_guid);

	ps = new Packet(SMSG_DELIVERY_CROSS_PLAYER_DATA_START, cross_pd_info->player_guid, scd_start.SerializeAsString());

	m_scene_cfg->cross_server->sendLineServerMsg(line_id, platform, ps);

	typed::protocol::smsg_cross_deliver_player_data scd_pd;

	for (CrossPlayerDataInfo::MsgMap_t::iterator it = cross_pd_info->msg_map.begin(); it != cross_pd_info->msg_map.end(); ++it)
	{
		if (it->second != NULL)
		{
			scd_pd.set_player_guid(cross_pd_info->player_guid);
			scd_pd.set_entity_guid(it->first);
			scd_pd.set_line_id(cross_pd_info->line_id);
			scd_pd.set_platform(cross_pd_info->platform);
			scd_pd.set_entity_stream(it->second->SerializeAsString());
			ps = new Packet(SMSG_DELIVERY_CROSS_PLAYER_DATA, cross_pd_info->player_guid, scd_pd.SerializeAsString());
			m_scene_cfg->cross_server->sendLineServerMsg(line_id, platform, ps);
		}
	}

	typed::protocol::smsg_cross_deliver_player_data_end scd_end;
	scd_end.set_platform(cross_pd_info->platform);
	scd_end.set_line_id(cross_pd_info->line_id);
	scd_end.set_player_guid(cross_pd_info->player_guid);
	ps = new Packet(SMSG_DELIVERY_CROSS_PLAYER_DATA_END, cross_pd_info->player_guid, scd_end.SerializeAsString());

	m_scene_cfg->cross_server->sendLineServerMsg(line_id, platform, ps);

}