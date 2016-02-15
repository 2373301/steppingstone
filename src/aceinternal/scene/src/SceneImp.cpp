
#include <fstream>
#include <ace/OS.h>
#include <boost/filesystem.hpp>
#include "SceneImp.h"
#include "SceneFactory.h"
#include "opcode.h"
#include "LoggerFactory.h"

#define PLUGIN_SAVE_CONFIG_DIR	"plugin_data"

#define MK_SCENE "[MK_SCENE] "

#define SCENE_LOG_DEBUG(log_info, ...)		m_scene_cfg.logger->log(LL_DEBUG, "%s" MK_SCENE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define SCENE_LOG_INFO(log_info, ...)		m_scene_cfg.logger->log(LL_INFO, "%s" MK_SCENE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define SCENE_LOG_WARNING(log_info, ...)	m_scene_cfg.logger->log(LL_WARNING, "%s" MK_SCENE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define SCENE_LOG_TRACE(log_info, ...)		m_scene_cfg.logger->log(LL_TRACE, "%s" MK_SCENE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define SCENE_LOG_ERROR(log_info, ...)		m_scene_cfg.logger->log(LL_ERROR, "%s" MK_SCENE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define SCENE_LOG_FATAL(log_info, ...)		m_scene_cfg.logger->log(LL_FATAL, "%s" MK_SCENE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)

#define CONVERT_RANK(attr_value)	attr_value * (int32)m_system_res_info->rank_param / (int32)player_level

using namespace std;

ManageResourceImp SceneImp::m_manage_resource;

SceneImp::SceneImp()
: m_pool(NULL)
, m_plugin_depot(NULL)
, m_war_init_res_info(NULL)
, m_system_res_info(NULL)
, m_property_res_info(NULL)
, m_save_packet_index(0)
, m_manage_timer(this)
, m_packet_convert(this)
, m_all_players_flushed_after_lost_gate_connection(false)
, m_is_startup_success(false)
, m_is_stop(false)
, m_is_shutdown_success(false)
{

}

SceneImp::~SceneImp()
{
	delete m_pool;

	delete m_plugin_depot;

	cleanPackInfoVec(m_input_packet_vec);

	for (CachePackInfoVec_t::iterator it = m_cache_input_packet_vec.begin(); it != m_cache_input_packet_vec.end(); ++it)
	{
		CachePackInfo & cpi = *it;
		delete cpi.packet;
	}

	delete m_scene_cfg.logger;
}


int SceneImp::init(const SceneCfg & scene_cfg)
{
	m_scene_cfg = scene_cfg;

	m_pool = SceneFactory::createPool();
	if (NULL == m_pool)
	{
		DEF_LOG_ERROR("failed to create pool, get NULL pointer\n");
		return -1;
	}

	if (m_make_guid.init(m_scene_cfg.guid_svr_cfg.guid_addr, 10000) == -1)
	{
		DEF_LOG_ERROR("failed to init guid server, guid addr is <%s>\n", m_scene_cfg.guid_svr_cfg.guid_addr.c_str());
		return -1;
	}

	static bool first_time = true;
	if (first_time)
	{
		if (m_manage_resource.init(&m_scene_cfg) == -1)
		{
			DEF_LOG_ERROR("failed to init manage resource\n");
			return -1;
		}

		first_time = false;
	}

	//if (m_manage_battle_report.init(m_scene_cfg.original_map_id, m_scene_cfg.packet_dir) == -1)
	//{
	//	DEF_LOG_ERROR("failed to init m_manage_battle_report\n");
	//	return -1;
	//}

	m_war_init_res_info = (WarInitResInfo *)&this->getManageResource()->getWarInitResInfo();
	m_system_res_info = (SystemResInfo *)&this->getManageResource()->getSystemResInfo();
	m_property_res_info = (PropertyResInfoMap_t *)&this->getManageResource()->getPropertyResInfo();

	if (scene_cfg.plugin_dll_vec.size() != scene_cfg.plugin_param_vec->size())
	{
		DEF_LOG_ERROR("failed to init scene, the plugin dll vec size is not equal plugin param config size\n");
		return -1;
	}

	m_scene_cfg = scene_cfg;

	string scene_log_str = m_scene_cfg.log_dir + "/line_" + boost::lexical_cast<string>(m_scene_cfg.line_no) + "_" + boost::lexical_cast<string>(m_scene_cfg.original_map_id);
	m_scene_cfg.logger = LoggerFactory::createFileLogger(scene_log_str);
	if (NULL == m_scene_cfg.logger)
	{
		DEF_LOG_ERROR("failed to create scene logger\n");
		return -1;
	}

	PoolCfg pool_cfg;
	pool_cfg.map_id = m_scene_cfg.map_id;
	pool_cfg.line_id = m_scene_cfg.line_no;
	pool_cfg.handle_output = m_scene_cfg.cache_handle_output;
	pool_cfg.logger = m_scene_cfg.logger;
	pool_cfg.scene = this;
	if (m_pool->init(pool_cfg) == -1)
	{
		DEF_LOG_ERROR("failed to init pool\n");
		return -1;
	}

	m_plugin_depot = SceneFactory::createPluginDepot();
	if (NULL == m_plugin_depot)
	{
		DEF_LOG_ERROR("failed to create plugin depot in SceneImp::Init\n");
		return -1;
	}

	PluginDepotCfg plugin_depot_cfg;
	plugin_depot_cfg.plugin_dll_vec = m_scene_cfg.plugin_dll_vec;
	plugin_depot_cfg.plugin_param_vec = m_scene_cfg.plugin_param_vec;
	plugin_depot_cfg.scene = this;
	plugin_depot_cfg.pool = m_pool;
	plugin_depot_cfg.handle_output = m_scene_cfg.manage_terminal;
	plugin_depot_cfg.manage_grid = &m_manage_grid;
	plugin_depot_cfg.cache_type = m_scene_cfg.cache_type;
	plugin_depot_cfg.line_no = m_scene_cfg.line_no;
	plugin_depot_cfg.template_id = m_scene_cfg.original_map_id;
	plugin_depot_cfg.logger = m_scene_cfg.logger;
	plugin_depot_cfg.line_scene = m_scene_cfg.line_scene;
	plugin_depot_cfg.scene_request = m_scene_cfg.scene_request;
	plugin_depot_cfg.push_client_error_msg = m_scene_cfg.push_client_error_msg;
	plugin_depot_cfg.is_first_launch = m_scene_cfg.is_first_launch;
	plugin_depot_cfg.enable_gm = m_scene_cfg.enable_gm;
	plugin_depot_cfg.data_record = m_scene_cfg.data_record;
	plugin_depot_cfg.server_cfg = m_scene_cfg.server_cfg;
	plugin_depot_cfg.cross_server_cfg = m_scene_cfg.cross_server_cfg;
	plugin_depot_cfg.cross_server = m_scene_cfg.cross_server;

	if (m_plugin_depot->init(plugin_depot_cfg) == -1)
	{
		DEF_LOG_ERROR("failed to init plugin depot in SceneImp::init, scene id is <%d>\n", m_scene_cfg.scene_id);
		return -1;
	}

	std::vector<int> maps;
	m_manage_grid.init(&m_scene_cfg, this);
	m_manage_grid.setMaxScreenNumber(m_scene_cfg.max_sceen_number);

	m_scene_cfg.manage_terminal->registerScene(m_scene_cfg.map_id, 1);
	//for (Uint64Vec_t::iterator it = m_scene_cfg.map_id_vec.begin(); it != m_scene_cfg.map_id_vec.end(); ++it)
	//{
	//	m_scene_cfg.manage_terminal->registerScene(*it, 1);
	//}

	if (m_scene_cfg.save_packet)
	{
		boost::filesystem::path path(m_scene_cfg.packet_dir);
		if (!boost::filesystem::exists(path))
		{
			return boost::filesystem::create_directory(path);
		}
	}

	boost::filesystem::path plugin_data(PLUGIN_SAVE_CONFIG_DIR);
	if (!boost::filesystem::exists(plugin_data))
	{
		return boost::filesystem::create_directory(plugin_data);
	}

	//m_scene_cfg.

	return 0;
}

int SceneImp::startup()
{
	if (this->activate() == -1)
	{
		SCENE_LOG_ERROR("failed to call active of SceneImp, last error is <%d>", ACE_OS::last_error());
		return -1;
	}

	if (m_packet_convert.activate() == -1)
	{
		SCENE_LOG_ERROR("failed to call active of PacketConvert, last error is <%d>", ACE_OS::last_error());
		return -1;
	}

	if (m_manage_timer.init() == -1)
	{
		SCENE_LOG_ERROR("failed to init manage timer in scene init");
		return -1;
	}

	return 0;
}

int SceneImp::stop()
{
	m_is_stop = true;
	m_manage_timer.stop();
	m_make_guid.stop();
	m_manage_grid.stop();
	m_packet_convert.stop();
	return 0;
}

int SceneImp::finit()
{
	return 0;
}

//void SceneImp::allPlayersFlushedAfterLostGateConnection()
//{
//	m_all_players_flushed_after_lost_gate_connection = true;
//}

bool SceneImp::isShutdownSuccess()
{
	if (m_is_shutdown_success)
	{
		return m_is_shutdown_success;
	}

	m_is_shutdown_success = m_plugin_depot->isShutdownSuccess();

	return m_is_shutdown_success;
}

bool SceneImp::isStartupSuccess()
{
	if (m_is_startup_success)
	{
		return m_is_startup_success;
	}

	m_is_startup_success = m_plugin_depot->isStartupSuccess();

	if (m_is_startup_success)
	{
		m_manage_timer.trigger();
	}

	return m_is_startup_success;
}

bool SceneImp::isLineScene()
{
	return false;
}

void SceneImp::input(Packet * packet)
{
	if (m_scene_cfg.save_packet)
	{
		savePacket(packet);
	}
	m_packet_convert.processPacket(packet);
	//ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_input_packet_vec_mutex, );
	//m_input_packet_vec.push_back(packet);
}

void SceneImp::cacheInput(Packet * packet, uint64 map_id, uint64 request_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cache_input_packet_vec_mutex, );
	m_cache_input_packet_vec.push_back(CachePackInfo(request_id, map_id, packet));
}

void SceneImp::packInput(PackInfo * pack_info)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_input_packet_vec_mutex, );
	m_input_packet_vec.push_back(pack_info);
}

int SceneImp::svc (void)
{
	REPORT_THREAD_INFO();

	PackInfo * pack_info = NULL;
	vector<PackInfo *> input_packet_vec;
	CachePackInfoVec_t cache_input_packet_vec;
	GOOGLE_MESSAGE_TYPE * protobuf_msg = NULL;

	ACE_Time_Value start_time;
	ACE_Time_Value diff_time;

	bool terminal_is_empty = true;
	ACE_Time_Value idle_sleep_time(0, 1000);

	while (!m_is_stop)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_input_packet_vec_mutex, -1);
			std::copy(m_input_packet_vec.begin(), m_input_packet_vec.end(), back_inserter(input_packet_vec));
			m_input_packet_vec.clear();
		}

		terminal_is_empty = input_packet_vec.empty();

		if (input_packet_vec.size() > 2000)
		{
			SCENE_LOG_INFO("SceneImp::svc, packet size is <%d>", input_packet_vec.size());
		}

		start_time = ACE_OS::gettimeofday();

		for (vector<PackInfo *>::iterator it = input_packet_vec.begin(); it != input_packet_vec.end(); ++it)
		{
			m_pool->beginTransaction();
			pack_info = *it;
			if (SMSG_TIMER_OCCUR == pack_info->opcode)
			{
				TimerCallBackMap_t::iterator it = m_timer_callback_map.find((long)pack_info->guid);
				if (it != m_timer_callback_map.end())
				{
					ACE_Time_Value start_time = ACE_OS::gettimeofday();

					(it->second)();

					ACE_Time_Value time_diff = ACE_OS::gettimeofday() - start_time;
					uint64 spend_time = 0;
					time_diff.to_usec(spend_time);
					if (spend_time > 10000)
					{
						SCENE_LOG_INFO("call plugin timer, timer id is <%llu>, timer spend time is <%llu>", pack_info->guid, spend_time);
					}
				}
			}
			else
			{
				m_plugin_depot->input(*pack_info);
				//protobuf_msg = NULL;
				//if (extractPacket(packet, protobuf_msg))
				//{
				//	pack_info.guid = packet->guid();
				//	pack_info.opcode = packet->opcode();
				//	pack_info.msg = protobuf_msg;
				//	m_plugin_depot->input(pack_info);
				//	if (NULL != pack_info.msg)
				//	{
				//		delete pack_info.msg;
				//		pack_info.msg = NULL;
				//	}
				//}
				//else
				//{
				//	///todo
				//}
			}
			
			m_pool->endtransaction();

			delete pack_info;
		}

		if (input_packet_vec.size() > 2000)
		{
			diff_time = ACE_OS::gettimeofday() - start_time;
			uint64 spend_time = 0;
			diff_time.to_usec(spend_time);
			int st = spend_time / 1000;
			SCENE_LOG_INFO("SceneImp::svc, packet size is <%d>, spend time is <%d>", input_packet_vec.size(), st);
		}

		pack_info = NULL;

		input_packet_vec.clear();

		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cache_input_packet_vec_mutex, -1);
			std::copy(m_cache_input_packet_vec.begin(), m_cache_input_packet_vec.end(), back_inserter(cache_input_packet_vec));
			m_cache_input_packet_vec.clear();
		}

		if (terminal_is_empty && cache_input_packet_vec.empty())
		{
			ACE_OS::sleep(idle_sleep_time);
		}

		for (CachePackInfoVec_t::iterator it = cache_input_packet_vec.begin(); it != cache_input_packet_vec.end(); ++it)
		{
			m_pool->cacheInput(it->packet, it->map_id, it->request_id);
		}

		cache_input_packet_vec.clear();
	}

	return 0;
}

Pool * SceneImp::getPool()
{
	return m_pool;
}

Message * SceneImp::getMessage()
{
	return this;
}

ManageResource * SceneImp::getManageResource()
{
	return &m_manage_resource;
}

long SceneImp::schemeTimer(int interval_value, TimerCallBack timer_callback)
{
	long timer_id = m_manage_timer.schemeTimer(interval_value);
	if (-1 != timer_id)
	{
		m_timer_callback_map[timer_id] = timer_callback;
	}
	else
	{
		// error
	}

	return timer_id;
}

long SceneImp::cancelTimer(long timer_id)
{
	TimerCallBackMap_t::iterator it = m_timer_callback_map.find(timer_id);
	if (it != m_timer_callback_map.end())
	{
		m_timer_callback_map.erase(it);
	}

	return m_manage_timer.cancelTimer(timer_id);
}

bool SceneImp::get_guid(EntityType entity_type, uint64 & guid)
{
	bool result = m_make_guid.get_guid(entity_type, guid);
	if (!result)
	{
		SCENE_LOG_ERROR("failed to make new guid");
	}
	return result;
}

int SceneImp::get_random(int max_no, int min_no)
{
	return m_manage_random.get_random(max_no, min_no);
}

int SceneImp::getRoleAttr(typed::entity::Role * role, RoleAttrType role_attr)
{
	if (NULL == role)
	{
		SCENE_LOG_ERROR("role is NULL in SceneImp::getRoleAttr, role attr index is <%d>", role_attr);
		return -1;
	}

	if ((role_attr >= RAT_ENDING) || (role_attr <= RAT_BEGIN))
	{
		// error
		return -1;
	}

	if (role_attr >= role->attributes_size())
	{
		SCENE_LOG_ERROR("error role attr index, role attr index is <%d>, role attr size is <%d>", role_attr, role->attributes_size());
		return -1;
	}

	return role->attributes(role_attr);
}

bool SceneImp::modifyRoleAttr(typed::entity::Role * role, RoleAttrType role_attr, int attr_value, bool notify_client)
{
	if (NULL == role)
	{
		SCENE_LOG_ERROR("get null pointer of role in SceneImp::modifyRoleAttr, role attr index is <%d>, attr value is <%d>", role_attr, attr_value);
		return false;
	}

	if ((role_attr >= RAT_ENDING) || (role_attr <= RAT_BEGIN))
	{
		// error
		SCENE_LOG_ERROR("get error attr index of role in SceneImp::modifyRoleAttr, role attr index is <%d>, attr value is <%d>", role_attr, attr_value);
		return false;
	}

	if (0 == attr_value)
	{
		return true;
	}

	bool player_already_online = false;
	uint32 player_level = 1;

	OnlinePlayerInfoMap_t::iterator it = m_online_player.find(role->player());
	if (it == m_online_player.end())
	{
		PlayerBriefInfo * pb_info = m_scene_cfg.line_scene->getPlayerBriefInfo(role->player());
		if (NULL == pb_info)
		{
			SCENE_LOG_ERROR("failed to get player brief info <%llu>", role->player());
			return false;
		}
		else
		{
			player_already_online = false;
			player_level = pb_info->short_info->level();
		}
	}
	else
	{
		player_already_online = it->second->already_online;
		player_level = it->second->level;
	}

	if (player_already_online && notify_client)
	{
		m_need_update_power_player.insert(role->player());
	}

	int old_attr_value = this->getRoleAttr(role, role_attr);
	int new_attr_value = old_attr_value + attr_value;

	switch (role_attr)
	{
	case RAT_STR:
		modifyRoleAttr(role, RAT_STR_ATT_BASE, attr_value * m_war_init_res_info->str_att / TEN_THOUSAND_PERCENT_BASE, notify_client);
		modifyRoleAttr(role, RAT_STR_DEF_BASE, attr_value * m_war_init_res_info->str_def / TEN_THOUSAND_PERCENT_BASE, notify_client);
		modifyRoleAttr(role, RAT_INITIATIVE_EDUCATE, attr_value * m_system_res_info->sanwei_first_strike_1, notify_client);
		break;
	case RAT_AGI:
		modifyRoleAttr(role, RAT_AGI_ATT_BASE, attr_value * m_war_init_res_info->agi_att / TEN_THOUSAND_PERCENT_BASE, notify_client);
		modifyRoleAttr(role, RAT_AGI_DEF_BASE, attr_value * m_war_init_res_info->agi_def / TEN_THOUSAND_PERCENT_BASE, notify_client);
		modifyRoleAttr(role, RAT_INITIATIVE_EDUCATE, attr_value * m_system_res_info->sanwei_first_strike_2, notify_client);
		break;
	case RAT_INT:
		modifyRoleAttr(role, RAT_INT_ATT_BASE, attr_value * m_war_init_res_info->int_att / TEN_THOUSAND_PERCENT_BASE, notify_client);
		modifyRoleAttr(role, RAT_INT_DEF_BASE, attr_value * m_war_init_res_info->int_def / TEN_THOUSAND_PERCENT_BASE, notify_client);
		modifyRoleAttr(role, RAT_INITIATIVE_EDUCATE, attr_value * m_system_res_info->sanwei_first_strike_3, notify_client);
		break;

	case RAT_STR_ATT_BASE:
		role->set_attributes(RAT_STR_ATTACK, new_attr_value * (role->attributes(RAT_STR_ATT_PERCENT) / TEN_THOUSAND_PERCENT_BASE + 1));
		break;
	case RAT_STR_ATT_PERCENT:
		role->set_attributes(RAT_STR_ATTACK, (1 + new_attr_value / TEN_THOUSAND_PERCENT_BASE) * role->attributes(RAT_STR_ATT_BASE));
		break;
	case RAT_STR_DEF_BASE:
		role->set_attributes(RAT_STR_DEFENSE, new_attr_value * (role->attributes(RAT_STR_DEF_PERCENT) / TEN_THOUSAND_PERCENT_BASE + 1));
		break;
	case RAT_STR_DEF_PERCENT:
		role->set_attributes(RAT_STR_DEFENSE, (1 + new_attr_value / TEN_THOUSAND_PERCENT_BASE) * role->attributes(RAT_STR_DEF_BASE));
		break;

	case RAT_AGI_ATT_BASE:
		role->set_attributes(RAT_AGI_ATTACK, new_attr_value * (role->attributes(RAT_AGI_ATT_PERCENT) / TEN_THOUSAND_PERCENT_BASE + 1));
		break;
	case RAT_AGI_ATT_PERCENT:
		role->set_attributes(RAT_AGI_ATTACK, (1 + new_attr_value / TEN_THOUSAND_PERCENT_BASE) * role->attributes(RAT_AGI_ATT_BASE));
		break;
	case RAT_AGI_DEF_BASE:
		role->set_attributes(RAT_AGI_DEFENSE, new_attr_value * (role->attributes(RAT_AGI_DEF_PERCENT) / TEN_THOUSAND_PERCENT_BASE + 1));
		break;
	case RAT_AGI_DEF_PERCENT:
		role->set_attributes(RAT_AGI_DEFENSE, (1 + new_attr_value / TEN_THOUSAND_PERCENT_BASE) * role->attributes(RAT_AGI_DEF_BASE));
		break;

	case RAT_INT_ATT_BASE:
		role->set_attributes(RAT_INT_ATTACK, new_attr_value * (role->attributes(RAT_INT_ATT_PERCENT) / TEN_THOUSAND_PERCENT_BASE + 1));
		break;
	case RAT_INT_ATT_PERCENT:
		role->set_attributes(RAT_INT_ATTACK, (1 + new_attr_value / TEN_THOUSAND_PERCENT_BASE) * role->attributes(RAT_INT_ATT_BASE));
		break;
	case RAT_INT_DEF_BASE:
		role->set_attributes(RAT_INT_DEFENSE, new_attr_value * (role->attributes(RAT_INT_DEF_PERCENT) / TEN_THOUSAND_PERCENT_BASE + 1));
		break;
	case RAT_INT_DEF_PERCENT:
		role->set_attributes(RAT_INT_DEFENSE, (1 + new_attr_value / TEN_THOUSAND_PERCENT_BASE) * role->attributes(RAT_INT_DEF_BASE));
		break;
	case RAT_HP_BASE:
		role->set_attributes(RAT_HP, new_attr_value * (role->attributes(RAT_HP_PERCENT) / TEN_THOUSAND_PERCENT_BASE + 1));
		role->set_attributes(RAT_CURR_HP, new_attr_value * (role->attributes(RAT_HP_PERCENT) / TEN_THOUSAND_PERCENT_BASE + 1));
		break;
	case RAT_HP_PERCENT:
		role->set_attributes(RAT_HP, (new_attr_value / TEN_THOUSAND_PERCENT_BASE + 1) * role->attributes(RAT_HP_BASE));
		role->set_attributes(RAT_CURR_HP, (new_attr_value / TEN_THOUSAND_PERCENT_BASE + 1) * role->attributes(RAT_HP_BASE));
		break;
	case RAT_CRITICAL_RANK:
		modifyRoleAttr(role, RAT_CRITICAL, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_CRITICAL_MULTIPLE_RANK:
		modifyRoleAttr(role, RAT_CRITICAL_MULTIPLE, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_BLOCK_RANK:
		modifyRoleAttr(role, RAT_BLOCK, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_BLOCK_DEFENCE_MULTIPLE_RANK:
		modifyRoleAttr(role, RAT_BLOCK_DEFENCE_MULTIPLE, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_STRIKE_BACK_RANK:
		modifyRoleAttr(role, RAT_STRIKE_BACK, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_HIT_RANK:
		modifyRoleAttr(role, RAT_HIT, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_DODGE_RANK:
		modifyRoleAttr(role, RAT_DODGE, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_MULTI_HIT_RANK:
		modifyRoleAttr(role, RAT_MULTI_HIT, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_UNION_HIT_RANK:
		modifyRoleAttr(role, RAT_UNION_HIT, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_MULTI_HIT_ADDI_RANK:
		modifyRoleAttr(role, RAT_MULTI_HIT_ADDI, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_UNION_HIT_ADDI_RANK:
		modifyRoleAttr(role, RAT_UNION_HIT_ADDI, CONVERT_RANK(attr_value), notify_client);
		break;

	case RAT_INITIATIVE_STHEN:
		modifyRoleAttr(role, RAT_INITIATIVE_ALL, attr_value, notify_client);
		break;
	case RAT_INITIATIVE_COLOR:
		modifyRoleAttr(role, RAT_INITIATIVE_ALL, attr_value, notify_client);
		break;
	case RAT_INITIATIVE_LAW:
		modifyRoleAttr(role, RAT_INITIATIVE_ALL, attr_value, notify_client);
		break;
	case RAT_INITIATIVE_EDUCATE:
		modifyRoleAttr(role, RAT_INITIATIVE_ALL, attr_value, notify_client);
		break;
	case RAT_INITIATIVE_DODGE:
		modifyRoleAttr(role, RAT_INITIATIVE_ALL, attr_value, notify_client);
		break;
	case RAT_INITIATIVE_CHAKRE:
		modifyRoleAttr(role, RAT_INITIATIVE_ALL, attr_value, notify_client);
		break;
	case RAT_INITIATIVE_TRAINING:
		modifyRoleAttr(role, RAT_INITIATIVE_ALL, attr_value, notify_client);
		break;
	case RAT_INITIATIVE_LEVEL:
		modifyRoleAttr(role, RAT_INITIATIVE_ALL, attr_value, notify_client);
		break;
	case RAT_UNCRITICAL_RANK:
		modifyRoleAttr(role, RAT_UNCRITICAL, CONVERT_RANK(attr_value), notify_client);
		break;
	case RAT_UNBLOCK_RANK:
		modifyRoleAttr(role, RAT_UNBLOCK, CONVERT_RANK(attr_value), notify_client);
		break;

	default :
		break;
	}

	//new_attr_value = new_attr_value >= 0 ? new_attr_value : 0;
	role->set_attributes(role_attr, new_attr_value);

	//// after modify
	//switch (role_attr)
	//{
	//case RAT_HP:
	//	if (role->attributes(RAT_CURR_HP) > role->attributes(RAT_HP))
	//	{
	//		role->set_attributes(RAT_CURR_HP, role->attributes(RAT_HP));
	//	}
	//	break;
	//}

	bool update_attr = false;

	role->set_update_fight_power(0);

	typed::protocol::smsg_update_role_attr update_role_attr;

	//int fight_power = 0;

	//fight_power = this->calcRoleFightPower(role);
	//role->set_attributes(RAT_FIGHT_POWER, fight_power);

	//update_role_attr.add_attr_index(20);
	//update_role_attr.add_attr_value(fight_power);

	//update_attr = true;

	//switch (role_attr)
	//{
	//case RAT_HP:
	//case RAT_STR:
	//case RAT_AGI:
	//case RAT_INT:
	//case RAT_STR_ATTACK:
	//case RAT_AGI_ATTACK:
	//case RAT_INT_ATTACK:
	//case RAT_STR_DEFENSE:
	//case RAT_AGI_DEFENSE:
	//case RAT_INT_DEFENSE:
	//case RAT_CRITICAL:
	//case RAT_CRITICAL_MULTIPLE:
	//case RAT_BLOCK:
	//case RAT_BLOCK_DEFENCE_MULTIPLE:
	//case RAT_BLOCK_DAMAGE_MULTIPLE:
	//case RAT_DODGE:
	//case RAT_HIT:
	//case RAT_STRIKE_BACK:
	//	fight_power = this->calcRoleFightPower(role);
	//	role->set_attributes(RAT_FIGHT_POWER, fight_power);

	//	update_role_attr.add_attr_index(20);
	//	update_role_attr.add_attr_value(fight_power);

	//	update_attr = true;

	//	break;
	//}

	switch (role_attr)
	{
	case RAT_STR:
		update_role_attr.add_attr_index(3);
		update_role_attr.add_attr_value(role->attributes(RAT_STR));
		update_attr = true;
		break;
	case RAT_AGI:
		update_role_attr.add_attr_index(4);
		update_role_attr.add_attr_value(role->attributes(RAT_AGI));
		update_attr = true;
		break;
	case RAT_INT:
		update_role_attr.add_attr_index(5);
		update_role_attr.add_attr_value(role->attributes(RAT_INT));
		update_attr = true;
		break;
	case RAT_HP:
	case RAT_HP_BASE:
	case RAT_HP_PERCENT:
		update_role_attr.add_attr_index(2);
		update_role_attr.add_attr_value(role->attributes(RAT_HP));

		update_role_attr.add_attr_index(1);
		update_role_attr.add_attr_value(role->attributes(RAT_CURR_HP));

		update_attr = true;
		break;
	}

	if (notify_client && update_attr)
	{
		if (player_already_online)
		{
			// online
			update_role_attr.set_role_guid(role->guid());
			MAKE_NEW_PACKET(ps, SMSG_UPDATE_ROLE_ATTR, role->player(), update_role_attr.SerializeAsString());
			SEND_PLAYER_MESSAGE(this, ps);
		}
	}

	return true;
}

bool SceneImp::modifyRoleAttrBySchemeId(typed::entity::Role * role, int scheme_id, int attr_value, bool notify_client)
{
	if (NULL == role)
	{
		SCENE_LOG_ERROR("get null pointer of role in SceneImp::modifyRoleAttrBySchemeId, role attr index is <%d>, attr value is <%d>", scheme_id, attr_value);
		return false;
	}

	PropertyResInfo * property_res = getPropertyRes(scheme_id);
	if (NULL == property_res)
	{
		SCENE_LOG_ERROR("do not find the scheme property id <%d>", scheme_id);
		return false;
	}

	if (0 == property_res->value)
	{
		return false;
	}

	if ((property_res->program_id <= RAT_BEGIN) || (property_res->program_id >= RAT_ENDING))
	{
		return false;
	}

	int real_attr_v = attr_value / property_res->value;

	return MODIFY_ROLE_ATTR(this, role, (RoleAttrType)property_res->program_id, real_attr_v, notify_client);
}

bool SceneImp::modifyPlayerAttrBySchemeId(typed::entity::Player * player, typed::entity::Role * role, int scheme_id, int attr_value, bool notify_client)
{
	switch (scheme_id)
	{
	case 1:
		this->modifyRoleAttr(role, RAT_HP_BASE, attr_value, notify_client);
		break;
	case 2:
		// todo
		break;
	case 3:
		// todo
		break;
	case 4:
		// todo
		break;
	case 5:
		// todo
		break;
	case 6:
		// todo
		break;
	case 7:
		this->modifyRoleAttr(role, RAT_STR_ATT_BASE, attr_value, notify_client);
		break;
	case 8:
		this->modifyRoleAttr(role, RAT_STR_DEF_BASE, attr_value, notify_client);
		break;
	case 9:
		this->modifyRoleAttr(role, RAT_AGI_ATT_BASE, attr_value, notify_client);
		break;
	case 10:
		this->modifyRoleAttr(role, RAT_INT_DEF_BASE, attr_value, notify_client);
		break;
	case 11:
		this->modifyRoleAttr(role, RAT_INT_ATT_BASE, attr_value, notify_client);
		break;
	case 12:
		this->modifyRoleAttr(role, RAT_INT_DEF_BASE, attr_value, notify_client);
		break;
	case 13:
		this->modifyRoleAttr(role, RAT_STR, attr_value, notify_client);
		break;
	case 14:
		this->modifyRoleAttr(role, RAT_AGI, attr_value, notify_client);
		break;
	case 15:
		this->modifyRoleAttr(role, RAT_INT, attr_value, notify_client);
		break;
	case 16:
		this->modifyRoleAttr(role, RAT_STR, attr_value, notify_client);
		this->modifyRoleAttr(role, RAT_AGI, attr_value, notify_client);
		this->modifyRoleAttr(role, RAT_INT, attr_value, notify_client);
		break;
	case 17:
		this->modifyRoleAttr(role, RAT_STR_ATT_BASE, attr_value, notify_client);
		this->modifyRoleAttr(role, RAT_AGI_ATT_BASE, attr_value, notify_client);
		this->modifyRoleAttr(role, RAT_INT_ATT_BASE, attr_value, notify_client);
		break;
	case 18:
		this->modifyRoleAttr(role, RAT_STR_DEF_BASE, attr_value, notify_client);
		this->modifyRoleAttr(role, RAT_AGI_DEF_BASE, attr_value, notify_client);
		this->modifyRoleAttr(role, RAT_INT_DEF_BASE, attr_value, notify_client);
		break;
	case 25:
		this->modifyRoleAttr(role, RAT_STR_ATT_PERCENT, attr_value, notify_client);
		break;
	case 26:
		this->modifyRoleAttr(role, RAT_STR_DEF_PERCENT, attr_value, notify_client);
		break;
	case 27:
		this->modifyRoleAttr(role, RAT_AGI_ATT_PERCENT, attr_value, notify_client);
		break;
	case 28:
		this->modifyRoleAttr(role, RAT_AGI_DEF_PERCENT, attr_value, notify_client);
		break;
	case 29:
		this->modifyRoleAttr(role, RAT_INT_ATT_PERCENT, attr_value, notify_client);
		break;
	case 30:
		this->modifyRoleAttr(role, RAT_INT_DEF_PERCENT, attr_value, notify_client);
		break;
	case 31:
		this->modifyRoleAttr(role, RAT_CRITICAL, attr_value, notify_client);
		break;
	case 32:
		this->modifyRoleAttr(role, RAT_BLOCK, attr_value, notify_client);
		break;
	case 33:
		this->modifyRoleAttr(role, RAT_HIT, attr_value, notify_client);
		break;
	case 34:
		this->modifyRoleAttr(role, RAT_DODGE, attr_value, notify_client);
		break;
	case 35:
		this->modifyRoleAttr(role, RAT_STRIKE_BACK, attr_value, notify_client);
		break;
	case 36:
		this->modifyRoleAttr(role, RAT_BLOCK_DEFENCE_MULTIPLE, attr_value, notify_client);
		break;
	case 37:
		this->modifyRoleAttr(role, RAT_CRITICAL_MULTIPLE, attr_value, notify_client);
		break;
	case 38:
		this->modifyRoleAttr(role, RAT_CRITICAL_MULTIPLE, attr_value, notify_client);
		break;
	case 39:
		// todo
		break;
	case 40:
		// todo
		break;
	case 41:
		// todo
		break;
	case 42:
		this->modifyRoleAttr(role, RAT_STR_ATT_PERCENT, attr_value, notify_client);
		this->modifyRoleAttr(role, RAT_AGI_ATT_PERCENT, attr_value, notify_client);
		this->modifyRoleAttr(role, RAT_INT_ATT_PERCENT, attr_value, notify_client);
		break;
	case 43:
		this->modifyRoleAttr(role, RAT_STR_DEF_PERCENT, attr_value, notify_client);
		this->modifyRoleAttr(role, RAT_AGI_DEF_PERCENT, attr_value, notify_client);
		this->modifyRoleAttr(role, RAT_INT_DEF_PERCENT, attr_value, notify_client);
		break;
	default:
		break;
	}

	return true;
}

int SceneImp::calcRoleFightPower(typed::entity::Role * role)
{
	int fight_power = 0;
	fight_power += role->level() * 10 + (role->attributes(RAT_HP) / 5.0 + 0.9999);
	//SCENE_LOG_DEBUG("role is <%llu>, fight power is <%d>", role->guid(), fight_power);

	fight_power += (role->attributes(RAT_STR) + role->attributes(RAT_AGI) + role->attributes(RAT_INT)) * 5;

	fight_power += role->attributes(RAT_STR_ATTACK) / 1.0 + 0.9999;
	fight_power += role->attributes(RAT_AGI_ATTACK) / 2.0 + 0.9999;
	fight_power += role->attributes(RAT_INT_ATTACK) / 0.8 + 0.9999;
	fight_power += role->attributes(RAT_STR_DEFENSE) / 0.6 + 0.9999;
	fight_power += role->attributes(RAT_AGI_DEFENSE) / 1.4 + 0.9999;
	fight_power += role->attributes(RAT_INT_DEFENSE) / 0.3 + 0.9999;

	fight_power += (role->attributes(RAT_CRITICAL) 
					+ role->attributes(RAT_CRITICAL_MULTIPLE) 
					+ role->attributes(RAT_BLOCK) 
					+ role->attributes(RAT_BLOCK) 
					+ role->attributes(RAT_BLOCK_DEFENCE_MULTIPLE) 
					+ role->attributes(RAT_BLOCK_DAMAGE_MULTIPLE) 
					+ role->attributes(RAT_DODGE) + 
					role->attributes(RAT_HIT) + 
					role->attributes(RAT_STRIKE_BACK)) / 100.0;
	fight_power = fight_power < 0 ? 0 : fight_power;

	//SCENE_LOG_DEBUG("role is <%llu>, fight power is <%d>", role->guid(), fight_power);

	return fight_power;
}

bool SceneImp::savePluginConfig(PluginType plugin_type, int index, char * buffer_stream, int buffer_size)
{
	if ((NULL == buffer_stream) || (buffer_size <= 0))
	{
		return false;
	}

	char file_name[100] = {0};
	sprintf(file_name, "./%s/plugin_data_%d_%d.data", PLUGIN_SAVE_CONFIG_DIR, plugin_type, index);
	fstream file_stream;
	file_stream.open(file_name, ios_base::trunc | ios_base::out | ios_base::binary);
	file_stream.rdbuf()->sputn(buffer_stream, buffer_size);

	bool failed = file_stream.fail();

	file_stream.flush();
	file_stream.close();

	return !failed;
}

bool SceneImp::loadPluginConfig(PluginType plugin_type, int index, char * * buffer_stream, int & buffer_size)
{
	char file_name[100] = {0};
	sprintf(file_name, "./%s/plugin_data_%d_%d.data", PLUGIN_SAVE_CONFIG_DIR, plugin_type, index);

	boost::filesystem::path plugin_data(file_name);
	if (!boost::filesystem::exists(plugin_data))
	{
		return false;
	}

	fstream file_stream;
	file_stream.open(file_name, ios_base::in | ios_base::binary);
	file_stream.seekg(0, ios::end);
	buffer_size = file_stream.tellg();
	file_stream.seekg(0, ios::beg);
	*buffer_stream = (char *)malloc(buffer_size);
	file_stream.read(*buffer_stream, buffer_size);

	bool failed = file_stream.fail();

	file_stream.flush();
	file_stream.close();

	return !failed;
}

void SceneImp::collectPlayerLevel(uint64 player, uint32 level)
{
	OnlinePlayerInfoMap_t::iterator it = m_online_player.find(player);
	if (it != m_online_player.end())
	{
		it->second->level = level;
	}
	else
	{
		OnlinePlayerInfo * player_info = new OnlinePlayerInfo(false, level);
		m_online_player.insert(std::make_pair(player, player_info));
	}
}

void SceneImp::playerOnline(uint64 player)
{
	OnlinePlayerInfoMap_t::iterator it = m_online_player.find(player);
	if (it != m_online_player.end())
	{
		it->second->already_online = true;
	}
}

void SceneImp::playerOffline(uint64 player)
{
	OnlinePlayerInfoMap_t::iterator it = m_online_player.find(player);
	if (it != m_online_player.end())
	{
		delete it->second;
		m_online_player.erase(it);
	}
}

Uint64Set_t & SceneImp::getNeedUpdatePowerPlayer()
{
	return m_need_update_power_player;
}

void SceneImp::playerMsg(Packet * packet)
{
	//if (packet->opcode() == SMSG_BATTLE_PLAYBACK)
	//{
	//	Packet * ps = new Packet(packet->opcode(), packet->guid(), string(packet->ch_body(), packet->body_size()));
	//	m_manage_battle_report.handleBattleReport(ps);
	//}

	if (m_scene_cfg.save_packet)
	{
		savePacket(packet);
	}
	SCENE_LOG_DEBUG("send player msg, player is <%llu>, opcode is <%d>", packet->guid(), packet->opcode());
	m_scene_cfg.manage_terminal->output(packet);
}

void SceneImp::inlineBroadMsg(Packet * packet)
{
	// todo
}

void SceneImp::notifyMsgToPlugins(const PackInfo & pack_info)
{
	m_plugin_depot->notify(pack_info);
}

bool SceneImp::requestMsgToPlugins(const PackInfo & pack_info)
{
	int res = m_plugin_depot->request(pack_info);
	if (res < 0)
	{
		SCENE_LOG_ERROR("request plugin msg failed, opcode is <%d>, return code is <%d>", pack_info.opcode, res);
	}

	return res >= 0;
}

bool SceneImp::gmcmdMsgToPlugins(const string & gm_name, const vector<string> & gm_param, uint64 target_guid)
{
	int res = m_plugin_depot->gmcmd(gm_name, gm_param, target_guid);
	if (res < 0)
	{
		SCENE_LOG_ERROR("failed to handle gm cmd, gm name is <%s>, return code is <%d>", gm_name.c_str(), res);
	}

	return res >= 0;
}

PropertyResInfo * SceneImp::getPropertyRes(int scheme_id)
{
	PropertyResInfoMap_t::iterator it = m_property_res_info->find(scheme_id);
	if (it != m_property_res_info->end())
	{
		return it->second;
	}

	return NULL;
}

void SceneImp::savePacket(Packet * packet)
{
	if (packet->opcode() < 1000)
	{
		return ;
	}

	if ((packet->opcode() == SMSG_PLAYER_MOVE) || (packet->opcode() == CMSG_PLAYER_MOVE))
	{
		return ;
	}

	char file_name[100] = {0};
	string guid = boost::lexical_cast<string>(packet->guid());
	sprintf(file_name, "%s/%d_%d_%d_%d_%s.pks", m_scene_cfg.packet_dir.c_str(), m_scene_cfg.line_no, m_scene_cfg.original_map_id, m_save_packet_index, packet->opcode(), guid.c_str());
	fstream file_stream;
	file_stream.open(file_name, ios_base::out | ios_base::app | ios_base::binary);
	if (packet->body_size() > 0)
	{
		file_stream.rdbuf()->sputn(packet->ch_body(), packet->body_size());
	}
	file_stream.flush();
	file_stream.close();

	m_save_packet_index += 1;
}
