
#include "PluginDepotImp.h"
#include "opcode.h"
#include "share_fun.h"

#define PLUGIN_EXPORT_NAME "make_plugin"

#define MK_PDEPOT "[MK_PDEPOT] "

#define PDEPOT_LOG_DEBUG(log_info, ...)		m_plugin_depot_cfg.logger->log(LL_DEBUG, "%s" MK_PDEPOT log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define PDEPOT_LOG_INFO(log_info, ...)		m_plugin_depot_cfg.logger->log(LL_INFO, "%s" MK_PDEPOT log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define PDEPOT_LOG_WARNING(log_info, ...)	m_plugin_depot_cfg.logger->log(LL_WARNING, "%s" MK_PDEPOT log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define PDEPOT_LOG_TRACE(log_info, ...)		m_plugin_depot_cfg.logger->log(LL_TRACE, "%s" MK_PDEPOT log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define PDEPOT_LOG_ERROR(log_info, ...)		m_plugin_depot_cfg.logger->log(LL_ERROR, "%s" MK_PDEPOT log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define PDEPOT_LOG_FATAL(log_info, ...)		m_plugin_depot_cfg.logger->log(LL_FATAL, "%s" MK_PDEPOT log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)

PluginDepotImp::~PluginDepotImp()
{
	cleanVectorContent(m_plugin_class_vec);
}

int PluginDepotImp::IPluginDepot_input(Packet * packet)
{
	return -1;
}

int PluginDepotImp::IPluginDepot_input(const PackInfo & pack_info)
{
//	PDEPOT_LOG_DEBUG("enter PluginDepotImp::input, opcode is <%d>", pack_info.opcode);

	int result = -1;
	ACE_Time_Value start_time = ACE_OS::gettimeofday();

	Input_Msg_Type_Map::iterator it = m_input_msg_map.find(pack_info.opcode);
	if (it != m_input_msg_map.end())
	{
		result = (it->second.plugin_object->*it->second.msg_fun)(pack_info);
	}
	else
	{
		/// error
		PDEPOT_LOG_ERROR("do not handle message, player is <%llu>, opcode is <%d>", pack_info.guid, pack_info.opcode);
	}

	ACE_Time_Value time_diff = ACE_OS::gettimeofday() - start_time;
	uint64 spend_time = 0;
	time_diff.to_usec(spend_time);
	if (result >= 0)
	{
		PDEPOT_LOG_DEBUG("***enter PluginDepotImp::input, player is <%llu>, opcode is <%d>, spend time is <%llu>", 
			pack_info.guid, pack_info.opcode, spend_time);
	}
	else
	{
		PDEPOT_LOG_ERROR("***enter PluginDepotImp::input, player is <%llu>, opcode is <%d>, spend time is <%llu>, input msg error code is <%d>",
			pack_info.guid, pack_info.opcode, spend_time, result);
	}
	return result;
}

int PluginDepotImp::IPluginDepot_notify(const PackInfo & pack_info)
{
	Notify_Msg_Type_Map::iterator it = m_notify_msg_map.find(pack_info.opcode);
	if (it != m_notify_msg_map.end())
	{
		vector<NotifyMsgInfo> & notify_msg = it->second;
		for (vector<NotifyMsgInfo>::iterator sub_it = notify_msg.begin(); sub_it != notify_msg.end(); ++sub_it)
		{
			ACE_Time_Value start_time = ACE_OS::gettimeofday();

			((*sub_it).plugin_object->*(*sub_it).msg_fun)(pack_info);

			ACE_Time_Value time_diff = ACE_OS::gettimeofday() - start_time;
			uint64 spend_time = 0;
			time_diff.to_usec(spend_time);
			PDEPOT_LOG_DEBUG("*** enter PluginDepotImp::notify, player is <%llu>, plugin is <%d>, opcode is <%d>, spend time is <%llu>",
				pack_info.guid, (*sub_it).plugin_object->getType(), pack_info.opcode, spend_time);
		}
	}
	else
	{
		// error
	}
	return 0;
}

int PluginDepotImp::IPluginDepot_request(const PackInfo & pack_info)
{
	int result = -1;
	ACE_Time_Value start_time = ACE_OS::gettimeofday();

	Request_Msg_Type_Map::iterator it = m_request_msg_map.find(pack_info.opcode);
	if (it != m_request_msg_map.end())
	{
		result = (it->second.plugin_object->*it->second.msg_fun)(pack_info);
	}
	else
	{
		PDEPOT_LOG_ERROR("do not handle request msg, opcode is <%d>, player is <%llu>", pack_info.opcode, pack_info.guid);
	}

	ACE_Time_Value time_diff = ACE_OS::gettimeofday() - start_time;
	uint64 spend_time = 0;
	time_diff.to_usec(spend_time);

	if (result >= 0)
	{
		if (spend_time < 1000)
		{
			PDEPOT_LOG_DEBUG("*** enter PluginDepotImp::request, player is <%llu>, opcode is <%d>, spend time is <%llu>",
				pack_info.guid, pack_info.opcode, spend_time);
		}
		else
		{
			PDEPOT_LOG_INFO("*** enter PluginDepotImp::request, player is <%llu>, opcode is <%d>, spend time is <%llu>",
				pack_info.guid, pack_info.opcode, spend_time);
		}
	}
	else
	{
		PDEPOT_LOG_ERROR("*** enter PluginDepotImp::request, player is <%llu>, opcode is <%d>, spend time is <%llu>",
			pack_info.guid, pack_info.opcode, spend_time);
	}

	return result;
}

int PluginDepotImp::IPluginDepot_gmcmd(const string & gm_name, const vector<string> & gm_param, uint64 target_guid)
{
	Gmcmd_Msg_Type_Map::iterator it = m_gmcmd_msg_map.find(gm_name);
	if (it != m_gmcmd_msg_map.end())
	{
		return (it->second.plugin_object->*it->second.msg_fun)(gm_param, target_guid);
	}
	else
	{
		PDEPOT_LOG_ERROR("do not handle gmcmd msg, opcode is <%s>", gm_name.c_str());
	}
	return -1;
}

bool PluginDepotImp::IPluginDepot_isStartupSuccess()
{
	bool is_startup_success = true;
	for (PluginMap_t::iterator it = m_plugin_map.begin(); it != m_plugin_map.end(); ++it)
	{
		IPlugin * plugin = it->second;
		if (!plugin->IPlugin_isStartupSuccess())
		{
			PDEPOT_LOG_INFO("wait for plugin while startup : <%d>", plugin->getType());
			is_startup_success = false;
			break;
		}
	}
	
	return is_startup_success;
}

bool PluginDepotImp::IPluginDepot_isShutdownSuccess()
{
	bool is_shutdown_success = true;

	for (PluginMap_t::iterator it = m_plugin_map.begin(); it != m_plugin_map.end(); ++it)
	{
		IPlugin * plugin = it->second;
		if (!plugin->IPlugin_isShutdownSuccess())
		{
			PDEPOT_LOG_INFO("wait for plugin while shutdown : <%d>", plugin->getType());
			is_shutdown_success = false;
		}
	}

	return is_shutdown_success;
}

int PluginDepotImp::IPluginDepot_initing()
{
	if (m_manage_lugin_config.init() == -1)
	{
		DEF_LOG_ERROR("failed to init m_manage_lugin_config\n");
		return -1;
	}

	static bool is_first_time = true;
	MAKE_PLUGIN_INSTANCE make_plugin = NULL;
	DllInfo * dll_info = NULL;
	int init_plugin_result = 0;
	IPlugin * plugin_ins = NULL;
	PluginCfg plugin_cfg;

	DllInfoVec_t::iterator it_dll = m_plugin_depot_cfg.plugin_dll_vec.begin();
	PluginParamConfigVec_t::iterator it_param = m_plugin_depot_cfg.plugin_param_vec->begin();
	for (; it_dll != m_plugin_depot_cfg.plugin_dll_vec.end(); 
		++it_dll, ++it_param)
	{
		dll_info = *it_dll;
		make_plugin = (MAKE_PLUGIN_INSTANCE)dll_info->dll_handle->symbol(PLUGIN_EXPORT_NAME);
		if (NULL == make_plugin)
		{
			PDEPOT_LOG_ERROR("failed to get plugin export function, the plugin is <%s>", dll_info->dll_path.c_str());
			return -1;
		}

		plugin_ins = make_plugin();
		if (NULL == plugin_ins)
		{
			PDEPOT_LOG_ERROR("failed to get plugin instance, the plugin is <%s>", dll_info->dll_path.c_str());
			return -1;
		}

		m_plugin_class_vec.push_back(plugin_ins);

		if (!isAvailablePlugin(plugin_ins))
		{
			// todo
			//delete plugin_ins;
			plugin_ins = NULL;
			continue;
		}

		PDEPOT_LOG_INFO("start to init plugin : <%s>", dll_info->dll_path.c_str());

		//plugin_cfg.plugin_depot = this;
		//plugin_cfg.scene = m_plugin_depot_cfg.scene;
		//plugin_cfg.pool = m_plugin_depot_cfg.pool;
		//plugin_cfg.handle_output = m_plugin_depot_cfg.handle_output;
		plugin_cfg.plugin_config = *it_param;
		//plugin_cfg.cache_type = m_plugin_depot_cfg.cache_type;
		//plugin_cfg.line_no = m_plugin_depot_cfg.line_no;
		//plugin_cfg.manage_grid = m_plugin_depot_cfg.manage_grid;
		plugin_cfg.message = m_plugin_depot_cfg.scene;
		//plugin_cfg.template_id = m_plugin_depot_cfg.template_id;
		plugin_cfg.logger = m_plugin_depot_cfg.logger;
		//plugin_cfg.line_scene = m_plugin_depot_cfg.line_scene;
		//plugin_cfg.scene_request = m_plugin_depot_cfg.scene_request;
		plugin_cfg.is_first_launch = m_plugin_depot_cfg.is_first_launch;
		plugin_cfg.enable_gm = m_plugin_depot_cfg.enable_gm;
		//plugin_cfg.data_record = m_plugin_depot_cfg.data_record;
		//plugin_cfg.server_cfg = m_plugin_depot_cfg.server_cfg;
		//plugin_cfg.cross_server = m_plugin_depot_cfg.cross_server;
		//plugin_cfg.manage_container = m_plugin_depot_cfg.line_scene->getManageContainer();

		if (plugin_ins->IPlugin_init(plugin_cfg) == -1)
		{
			init_plugin_result = -1;
			PDEPOT_LOG_ERROR("failed to init the plugin, plugin path is <%s>", dll_info->dll_path.c_str());
			break;
		}
			
		if (is_first_time)
		{
			PDEPOT_LOG_INFO("success to init plugin : <%s>, type is <%d>", dll_info->dll_path.c_str(), plugin_ins->getType());
		}

		std::pair<PluginMap_t::iterator, bool> insert_result = m_plugin_map.insert(std::make_pair(plugin_ins->getType(), plugin_ins));
		if (!insert_result.second)
		{
			init_plugin_result = -1;
			PDEPOT_LOG_ERROR("getting reduplicate plugin dll, the second plugin path is <%s>, plugin type is <%d>", dll_info->dll_path.c_str(), plugin_ins->getType());
			break;
		}
	}

	if (-1 != init_plugin_result)
	{
		if (!buildMsgMapping())
		{
			init_plugin_result = -1;
		}
	}

	is_first_time = false;

	return init_plugin_result;
}

bool PluginDepotImp::buildMsgMapping()
{
	bool result = true;
	result = buildInputMsgMapping() && result;
	result = buildRequestMsgMapping() && result;
	result = buildNotifyMsgMapping() && result;
	result = buildGmcmdMsgMapping() && result;

	return result;
}

bool PluginDepotImp::buildInputMsgMapping()
{
	bool result = true;
	for (PluginMap_t::iterator it = m_plugin_map.begin(); it != m_plugin_map.end(); ++it)
	{
		IPlugin * plugin = it->second;
		const Input_Msg_Type_Map & input_msg_map = plugin->getInputMsgMap();
		for (Input_Msg_Type_Map::const_iterator msg_it = input_msg_map.begin(); msg_it != input_msg_map.end(); ++msg_it)
		{
			std::pair<Input_Msg_Type_Map::iterator, bool> insert_result = m_input_msg_map.insert(std::make_pair(msg_it->first, msg_it->second));
			if (!insert_result.second)
			{
				result = false;
				PDEPOT_LOG_ERROR("failed to build input msg map, getting reduplicate input msg type, msg id is <%d>", msg_it->first);
				break;
			}
		}

		if (!result)
		{
			break;
		}
	}

	return result;
}

bool PluginDepotImp::buildRequestMsgMapping()
{
	bool result = true;
	for (PluginMap_t::iterator it = m_plugin_map.begin(); it != m_plugin_map.end(); ++it)
	{
		IPlugin * plugin = it->second;
		const Request_Msg_Type_Map & request_msg_map = plugin->getRequestMsgMap();
		for (Request_Msg_Type_Map::const_iterator msg_it = request_msg_map.begin(); msg_it != request_msg_map.end(); ++msg_it)
		{
			std::pair<Request_Msg_Type_Map::iterator, bool> insert_result = m_request_msg_map.insert(std::make_pair(msg_it->first, msg_it->second));
			if (!insert_result.second)
			{
				result = false;
				PDEPOT_LOG_ERROR("failed to build request msg map, getting reduplicate request msg type, msg id is <%d>", msg_it->first);
				break;
			}
		}

		if (!result)
		{
			break;
		}
	}

	return result;
}

bool PluginDepotImp::buildNotifyMsgMapping()
{
	bool result = true;
	for (PluginMap_t::iterator it = m_plugin_map.begin(); it != m_plugin_map.end(); ++it)
	{
		IPlugin * plugin = it->second;
		const Notify_Msg_Type_Map & notify_msg_map = plugin->getNofityMsgMap();
		for (Notify_Msg_Type_Map::const_iterator msg_it = notify_msg_map.begin(); msg_it != notify_msg_map.end(); ++msg_it)
		{
			std::copy(msg_it->second.begin(), msg_it->second.end(), std::back_inserter(m_notify_msg_map[msg_it->first]));
		}

		if (!result)
		{
			break;
		}
	}

	return result;
}

bool PluginDepotImp::buildGmcmdMsgMapping()
{
	bool result = true;
	for (PluginMap_t::iterator it = m_plugin_map.begin(); it != m_plugin_map.end(); ++it)
	{
		IPlugin * plugin = it->second;
		const Gmcmd_Msg_Type_Map & gmcmd_msg_map = plugin->getGmcmdMsgMap();
		for (Gmcmd_Msg_Type_Map::const_iterator msg_it = gmcmd_msg_map.begin(); msg_it != gmcmd_msg_map.end(); ++msg_it)
		{
			std::pair<Gmcmd_Msg_Type_Map::iterator, bool> insert_result = m_gmcmd_msg_map.insert(std::make_pair(msg_it->first, msg_it->second));
			if (!insert_result.second)
			{
				result = false;
				PDEPOT_LOG_ERROR("failed to build gmcmd msg map, getting reduplicate request msg type, msg id is <%s>", msg_it->first.c_str());
				break;
			}
		}

		if (!result)
		{
			break;
		}
	}

	return result;
}

bool PluginDepotImp::isAvailablePlugin(IPlugin * plugin)
{
	bool is_valid = false;

// 	if (m_manage_lugin_config.checkPlugin(m_plugin_depot_cfg.cross_server_cfg.server_type, m_plugin_depot_cfg.template_id, plugin->getType(), is_valid))
// 	{
// 		return is_valid;
// 	}

	if (plugin->getSceneType() == PST_BOTH)
	{
		return true;
	}

	return false;
}

IPluginDepot*  createPluginDepot()
{
	return new PluginDepotImp();
}