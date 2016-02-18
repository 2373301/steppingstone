
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


SceneImp::SceneImp()
:m_save_packet_index(0)
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
	//delete m_pool;

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


	static bool first_time = true;
	if (first_time)
	{
// 		if (m_manage_resource.init(&m_scene_cfg) == -1)
// 		{
// 			DEF_LOG_ERROR("failed to init manage resource\n");
// 			return -1;
// 		}

		first_time = false;
	}

	m_scene_cfg = scene_cfg;

	string scene_log_str = m_scene_cfg.log_dir + "/line_" + boost::lexical_cast<string>(m_scene_cfg.line_no) + "_" + boost::lexical_cast<string>(m_scene_cfg.original_map_id);
	m_scene_cfg.logger = LoggerFactory::createFileLogger(scene_log_str);
	if (NULL == m_scene_cfg.logger)
	{
		DEF_LOG_ERROR("failed to create scene logger\n");
		return -1;
	}

// 	PoolCfg pool_cfg;
// 	pool_cfg.map_id = m_scene_cfg.map_id;
// 	pool_cfg.line_id = m_scene_cfg.line_no;
// 	pool_cfg.handle_output = m_scene_cfg.cache_handle_output;
// 	pool_cfg.logger = m_scene_cfg.logger;
// 	pool_cfg.scene = this;
// 	if (m_pool->init(pool_cfg) == -1)
// 	{
// 		DEF_LOG_ERROR("failed to init pool\n");
// 		return -1;
// 	}
// 
// 	m_plugin_depot = SceneFactory::createPluginDepot();
// 	if (NULL == m_plugin_depot)
// 	{
// 		DEF_LOG_ERROR("failed to create plugin depot in SceneImp::Init\n");
// 		return -1;
// 	}
// 
// 	PluginDepotCfg plugin_depot_cfg;
// 	plugin_depot_cfg.plugin_dll_vec = m_scene_cfg.plugin_dll_vec;
// 	plugin_depot_cfg.plugin_param_vec = m_scene_cfg.plugin_param_vec;
// 	plugin_depot_cfg.scene = this;
// 	plugin_depot_cfg.pool = m_pool;
// 	plugin_depot_cfg.handle_output = m_scene_cfg.manage_terminal;
// 	plugin_depot_cfg.manage_grid = &m_manage_grid;
// 	plugin_depot_cfg.cache_type = m_scene_cfg.cache_type;
// 	plugin_depot_cfg.line_no = m_scene_cfg.line_no;
// 	plugin_depot_cfg.template_id = m_scene_cfg.original_map_id;
// 	plugin_depot_cfg.logger = m_scene_cfg.logger;
// 	plugin_depot_cfg.line_scene = m_scene_cfg.line_scene;
// 	plugin_depot_cfg.scene_request = m_scene_cfg.scene_request;
// 	plugin_depot_cfg.push_client_error_msg = m_scene_cfg.push_client_error_msg;
// 	plugin_depot_cfg.is_first_launch = m_scene_cfg.is_first_launch;
// 	plugin_depot_cfg.enable_gm = m_scene_cfg.enable_gm;
// 	plugin_depot_cfg.data_record = m_scene_cfg.data_record;
// 	plugin_depot_cfg.server_cfg = m_scene_cfg.server_cfg;
// 	plugin_depot_cfg.cross_server_cfg = m_scene_cfg.cross_server_cfg;
// 	plugin_depot_cfg.cross_server = m_scene_cfg.cross_server;
// 
// 	if (m_plugin_depot->init(plugin_depot_cfg) == -1)
// 	{
// 		DEF_LOG_ERROR("failed to init plugin depot in SceneImp::init, scene id is <%d>\n", m_scene_cfg.scene_id);
// 		return -1;
// 	}

	std::vector<int> maps;
	m_manage_grid.init(&m_scene_cfg, this);
	m_manage_grid.setMaxScreenNumber(m_scene_cfg.max_sceen_number);

	m_scene_cfg.manage_terminal->registerScene(m_scene_cfg.map_id, 1);
	//for (Uint64Vec_t::iterator it = m_scene_cfg.map_id_vec.begin(); it != m_scene_cfg.map_id_vec.end(); ++it)
	//{
	//	m_scene_cfg.manage_terminal->registerScene(*it, 1);
	//}

// 	if (m_scene_cfg.save_packet)
// 	{
// 		boost::filesystem::path path(m_scene_cfg.packet_dir);
// 		if (!boost::filesystem::exists(path))
// 		{
// 			return boost::filesystem::create_directory(path);
// 		}
// 	}

// 	boost::filesystem::path plugin_data(PLUGIN_SAVE_CONFIG_DIR);
// 	if (!boost::filesystem::exists(plugin_data))
// 	{
// 		return boost::filesystem::create_directory(plugin_data);
// 	}


	return 0;
}

int SceneImp::startup()
{
	if (this->activate() == -1)
	{
		SCENE_LOG_ERROR("failed to call active of SceneImp, last error is <%d>", ACE_OS::last_error());
		return -1;
	}

// 	if (m_packet_convert.activate() == -1)
// 	{
// 		SCENE_LOG_ERROR("failed to call active of PacketConvert, last error is <%d>", ACE_OS::last_error());
// 		return -1;
// 	}
// 
// 	if (m_manage_timer.init() == -1)
// 	{
// 		SCENE_LOG_ERROR("failed to init manage timer in scene init");
// 		return -1;
// 	}

	return 0;
}

int SceneImp::stop()
{
	m_is_stop = true;
// 	m_manage_timer.stop();
// 	m_make_guid.stop();
// 	m_manage_grid.stop();
// 	m_packet_convert.stop();
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
			}
			

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
			//m_pool->cacheInput(it->packet, it->map_id, it->request_id);
		}

		cache_input_packet_vec.clear();
	}

	return 0;
}

Message * SceneImp::getMessage()
{
	return this;
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