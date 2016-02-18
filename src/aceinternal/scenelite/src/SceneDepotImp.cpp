
#include <iostream>
#include <ace/OS.h>
#include <ace/SOCK_Connector.h>
#include <ace/Svc_Handler.h>
#include <boost/filesystem.hpp>
#include "SceneDepotImp.h"
#include "SceneFactory.h"
#include "protocol/msg_player.pb.h"
#include "protocol/msg_gate.pb.h"
#include "opcode.h"
#include "coredef.h"
#include "LineScene.h"

namespace fs=boost::filesystem;

SceneDepotImp::SceneDepotImp()
: m_lost_gate_connection(false)
, m_success_startup_notify(false)
, m_success_flush_data_notify(false)
, m_recv_sigusr1(false)
, m_notify_all_scenes_to_flush_player_data(false)
, m_success_to_init(false)
, m_is_stop(false)
{

}

SceneDepotImp::~SceneDepotImp()
{
	for (SceneMap_t::iterator it = m_scene_map.begin(); it != m_scene_map.end(); ++it)
	{
		delete it->second;
	}

	cleanPacketVec(m_input_packet_vec);

	for (CacheInputPacketVec_t::iterator it = m_cache_input_packet_vec.begin(); it != m_cache_input_packet_vec.end(); ++it)
	{
		CachePackInfo & cache_pack_info = *it;
		delete cache_pack_info.packet;
	}
}

int SceneDepotImp::init(const SceneDepotCfg & scene_depot_cfg)
{
	m_scene_deopt_cfg = scene_depot_cfg;

	if (this->activate() == -1)
	{
		DEF_LOG_ERROR("failed to call active of SceneDepotImp, last error is <%d>\n", ACE_OS::last_error());
		return -1;
	}

	if (!initScenes())
	{
		return -1;
	}

	m_success_to_init = true;

	return 0;
}

int SceneDepotImp::stop()
{
	m_is_stop = true;
	for (SceneMap_t::iterator it = m_scene_map.begin(); it != m_scene_map.end(); ++it)
	{
		Scene * scene = it->second;
		scene->stop();
	}
	return 0;
}

int SceneDepotImp::finit()
{
	return 0;
}

void SceneDepotImp::input(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_input_packet_vec_mutex, );
	m_input_packet_vec.push_back(packet);
}

void SceneDepotImp::cacheInput(Packet * packet, uint64 map_id, uint64 request_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cache_input_packet_vec_mutex, );
	m_cache_input_packet_vec.push_back(CachePackInfo(request_id, map_id, packet));
}

int SceneDepotImp::svc (void)
{
	REPORT_THREAD_INFO();

	// process the packet
	PacketVec_t	input_packet_vec;
	Packet * packet = NULL;
	CacheInputPacketVec_t cache_input_packet_vec;

	bool terminal_is_empty = false;
	ACE_Time_Value idle_sleep_time(0, 1000);

	int op_code = 0;
	uint64 remove_route_guid = 0;

	while (!m_is_stop)
	{
		if (!m_success_startup_notify)
		{
			if (m_success_to_init)
			{
				checkSceneStateStartupSuccess();
			}
		}

		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_cache_input_packet_vec_mutex, -1);
			std::copy(m_cache_input_packet_vec.begin(), m_cache_input_packet_vec.end(), back_inserter(cache_input_packet_vec));
			m_cache_input_packet_vec.clear();
		}

		for (CacheInputPacketVec_t::iterator it = cache_input_packet_vec.begin(); it != cache_input_packet_vec.end(); ++it)
		{
			dispatchCachePacket(*it);
		}
		cache_input_packet_vec.clear();

		if (!m_success_startup_notify)
		{
			ACE_OS::sleep(3);
			DEF_LOG_INFO("wait for starting successfully\n");
			continue;
		}

		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_input_packet_vec_mutex, -1);
			std::copy(m_input_packet_vec.begin(), m_input_packet_vec.end(), back_inserter(input_packet_vec));
			m_input_packet_vec.clear();
		}

		terminal_is_empty = input_packet_vec.empty();

		for (PacketVec_t::iterator it = input_packet_vec.begin(); it != input_packet_vec.end(); ++it)
		{
			packet = *it;
			op_code = packet->opcode();
			if (CMSG_PLAYER_ENTER_MAP == op_code)
			{
				typed::protocol::cmsg_player_enter_map player_enter_map;
				if (PARSE_PACKET(packet, &player_enter_map))
				{
					addRoute(packet->guid(), player_enter_map.map_id(), player_enter_map.line_id());
				}
				else
				{
					// error 
				}
			}
			else if (GMSG_PLAYER_JUMP_MAP == op_code)
			{
				typed::protocol::gmsg_player_jump_map gmsg_player_jump;
				if (PARSE_PACKET(packet, &gmsg_player_jump))
				{
					modifyRoute(packet->guid(), gmsg_player_jump.map_id(), gmsg_player_jump.line_no());
				}
				else
				{
					// error
				}
			}
			else if (CMSG_PLAYER_LEAVE_MAP == op_code)
			{
				remove_route_guid = packet->guid();
			}
			else if (SMSG_LOST_GATE_CONNECTION == op_code)
			{
				DEF_LOG_DEBUG("recv gate lost connection in SceneDepot");
				lostGateConnection();
			}
			else if (SMSG_RECEIVE_SIGUSR_1 == op_code)
			{
				recvSingleUsr1();
			}

			dispatchTerminalPacket(packet);
			
			if (0 != remove_route_guid)
			{
				removeRoute(remove_route_guid);
				remove_route_guid = 0;
			}
		}
		input_packet_vec.clear();

		if (terminal_is_empty && cache_input_packet_vec.empty())
		{
			ACE_OS::sleep(idle_sleep_time);
		}

		if (m_lost_gate_connection)
		{
			ACE_OS::sleep(1);
			checkSceneStateAfterLostGateConnection();
			continue;
		}
	}
	return 0;
}

bool SceneDepotImp::loadPlugin()
{
	return m_load_dll.loadByDir(m_scene_deopt_cfg.plugin_dir);
}

bool SceneDepotImp::loadPluginCfg()
{
	DllInfoVec_t & dll_info_vec = m_load_dll.getDllInfoVec();

	ParamConfig param_config;
	for (DllInfoVec_t::iterator it = dll_info_vec.begin(); it != dll_info_vec.end(); ++it)
	{
		DEF_LOG_INFO("****** start to load plugin config, plugin is <%s>\n", (*it)->dll_path.c_str());
		fs::path xml_path((*it)->dll_path);
		xml_path.replace_extension(".xml");
		string plugin_config_path = xml_path.string();

		param_config.clear();
		if (!loadPluginCfg(plugin_config_path, param_config))
		{
			DEF_LOG_ERROR("failed to load the plugin config, path is <%s>\n", xml_path.string().c_str());
			return false;
		}
		m_plugin_config_vec.push_back(param_config);
	}

	return true;
}

bool SceneDepotImp::loadPluginCfg(const string & config_path, ParamConfig & param_config)
{
	bool result = false;

	std::auto_ptr<Document> xml_doc(XMLFactory::create_document());
	try
	{
		if (xml_doc->load_file(config_path))
		{
			Element * root_ele = xml_doc->get_root();

			result = true;
			result = loadPluginCfg(root_ele, param_config) && result;
		}
		else
		{
			DEF_LOG_ERROR("failed to load config by xml document : <%s>\n", config_path.c_str());
		}
	}
	catch (...)
	{
		DEF_LOG_ERROR("unknown exception in SceneDepotImp::loadPluginCfg\n");
		result = false;
	}

	return result;
}

bool SceneDepotImp::loadPluginCfg(Element * root_element, ParamConfig & param_config)
{
	Elements eles = root_element->get_elements();
	for (Elements::iterator it = eles.begin(); it != eles.end(); ++it)
	{
		Attribute * attr_name = (*it)->get_attribute("name");
		if (NULL == attr_name)
		{
			DEF_LOG_ERROR("failed to get name attribute\n");
			return false;
		}

		Attribute * attr_value = (*it)->get_attribute("value");
		if (NULL == attr_value)
		{
			DEF_LOG_ERROR("failed to get value attribute\n");
			return false;
		}

		param_config.append(attr_name->get_value(), attr_value->get_value());
	}

	return true;
}

bool SceneDepotImp::initScenes()
{
	SceneCfg scene_cfg;
	for (SceneDepotCfg::SceneCfgVec_t::iterator it = m_scene_deopt_cfg.scenes_cfg.begin(); it != m_scene_deopt_cfg.scenes_cfg.end(); ++it)
	{
		scene_cfg.plugin_dll_vec = m_load_dll.getDllInfoVec();
		//scene_cfg.scene_id = it->scene_id;
		scene_cfg.scene_id = 0;
		scene_cfg.line_no = it->line_no;
		scene_cfg.plugin_param_vec = &m_plugin_config_vec;
		//scene_cfg.map_id = MAKE_MAP_GUID(it->line_no, 0, it->scene_id);
		scene_cfg.cache_type = m_scene_deopt_cfg.cache_type;
		scene_cfg.cache_handle_output = m_scene_deopt_cfg.cache_handle_output;
		scene_cfg.manage_terminal = m_scene_deopt_cfg.manage_terminal;
		scene_cfg.resource_cfg = m_scene_deopt_cfg.resource_cfg;
		scene_cfg.guid_svr_cfg = m_scene_deopt_cfg.guid_svr_cfg;
		scene_cfg.save_packet = m_scene_deopt_cfg.save_packet;
		scene_cfg.log_dir = m_scene_deopt_cfg.log_dir;
		scene_cfg.enable_gm = m_scene_deopt_cfg.enable_gm;

		//scene_cfg.map_id = MAKE_MAP_GUID(it->line_no, 0, 0);
		scene_cfg.map_id = MAKE_MAP_GUID(0, 0, 0);
		scene_cfg.original_map_id = 0;
		scene_cfg.map_id_vec = it->map_id_vec;
		LineScene * line_scene = new LineScene();
		scene_cfg.line_scene = line_scene;
		scene_cfg.scene_request = line_scene->getSceneRequest();
		scene_cfg.push_client_error_msg = m_scene_deopt_cfg.push_client_error_msg;
		scene_cfg.is_first_launch = m_scene_deopt_cfg.is_first_launch;
		scene_cfg.max_sceen_number = m_scene_deopt_cfg.max_sceen_number;
		scene_cfg.exchange_cfg = m_scene_deopt_cfg.exchange_cfg;
		scene_cfg.data_record = line_scene->getDataRecord();
		scene_cfg.record_server_addr = m_scene_deopt_cfg.record_server_addr;
		scene_cfg.gm_server_addr = m_scene_deopt_cfg.gm_server_addr;
		scene_cfg.server_cfg = m_scene_deopt_cfg.server_cfg;
		scene_cfg.cross_server_cfg = m_scene_deopt_cfg.cross_server_cfg;
		scene_cfg.battle_report_cfg = m_scene_deopt_cfg.battle_report_cfg;
		scene_cfg.server_cfg = m_scene_deopt_cfg.server_cfg;
		scene_cfg.cross_server = line_scene->getCrossServer();
		scene_cfg.vip_init_security_key = m_scene_deopt_cfg.vip_init_security_key;
		scene_cfg.robot_security_key = m_scene_deopt_cfg.robot_security_key;

		if (!initScene(scene_cfg, line_scene))
		{
			DEF_LOG_ERROR("failed to init line scene : %d\n", scene_cfg.original_map_id);
			return false;
		}
		m_scene_map[scene_cfg.map_id] = line_scene;

		Scene * scene = NULL;
		for (Uint64Vec_t::iterator map_it = scene_cfg.map_id_vec.begin(); map_it != scene_cfg.map_id_vec.end(); ++map_it)
		{
			//scene_cfg.map_id = MAKE_MAP_GUID(it->line_no, 0, *map_it);
			scene_cfg.map_id = MAKE_MAP_GUID(0, 0, *map_it);
			scene_cfg.original_map_id = *map_it;
			scene_cfg.scene_id = *map_it;
			scene_cfg.scene_request = NULL;
			scene = SceneFactory::createScene();
			if (!initScene(scene_cfg, scene))
			{
				DEF_LOG_ERROR("failed to init scene : %d", scene_cfg.original_map_id);
				return false;
			}

			if (line_scene->addScene(*map_it, scene) == -1)
			{
				return false;
			}

			m_scene_map[scene_cfg.map_id] = scene;
		}
	}

	for (SceneMap_t::iterator it = m_scene_map.begin(); it != m_scene_map.end(); ++it)
	{
		Scene * scene = it->second;
		PackInfo * pack_info = new PackInfo(SMSG_SCENE_INITED, 0, NULL);
		scene->packInput(pack_info);
	}

	return true;
}

bool SceneDepotImp::initScene(const SceneCfg & scene_cfg, Scene * scene)
{
	DEF_LOG_INFO("***** start to init scene, scene id is <%d>, line no is <%d>, map id is <%llu>\n", scene_cfg.scene_id, scene_cfg.line_no, scene_cfg.map_id);
	SceneMap_t::iterator map_it = m_scene_map.find(scene_cfg.map_id);
	if (m_scene_map.end() != map_it)
	{
		DEF_LOG_ERROR("error to create scene twice, scene id is <%d>, map id is <%s>\n", scene_cfg.scene_id, boost::lexical_cast<string>(scene_cfg.map_id).c_str());
		return false;
	}

	if (NULL == scene)
	{
		DEF_LOG_ERROR("failed to create scene, scene id is <%d>\n", scene_cfg.scene_id);
		return false;
	}

	if (scene->init(scene_cfg) == -1)
	{
		DEF_LOG_ERROR("failed to init scene, scene id is <%d>\n", scene_cfg.scene_id);
		return false;
	}

	DEF_LOG_INFO("****** start to startup scene, scene id is <%d>, line no is <%d>, map id is <%s>\n", scene_cfg.scene_id, scene_cfg.line_no, boost::lexical_cast<string>(scene_cfg.map_id).c_str());
	if (scene->startup() == -1)
	{
		DEF_LOG_ERROR("failed to startup scene, scene id is <%d>\n", scene_cfg.scene_id);
		return false;
	}

	DEF_LOG_INFO("##### finish to start scene, scene id is <%d>, line no is <%d>, map id is <%llu>\n", scene_cfg.scene_id, scene_cfg.line_no, scene_cfg.map_id);

	return true;
}

void SceneDepotImp::addRoute(uint64 player_guid, uint32 map_id, uint32 line_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_route_map_mutex, );
	//uint64 md = MAKE_MAP_GUID(line_id, 0, map_id);
	uint64 md = MAKE_MAP_GUID(0, 0, map_id);
	Scene * scene = getSceneByMap(md);
	if (NULL != scene)
	{
		/// todo
		/// deal with reduplicate building route
		m_player_route_map[player_guid] = scene;
	}
	else
	{
		// error , never to happy
	}
}

void SceneDepotImp::removeRoute(uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_route_map_mutex, );
	PlayerRouteMap_t::iterator it = m_player_route_map.find(player_guid);
	if (it != m_player_route_map.end())
	{
		m_player_route_map.erase(it);
	}
}

void SceneDepotImp::modifyRoute(uint64 player_guid, uint32 map_id, uint32 line_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_route_map_mutex, );
	//uint64 md = MAKE_MAP_GUID(line_id, 0, map_id);
	uint64 md = MAKE_MAP_GUID(0, 0, map_id);
	Scene * scene = getSceneByMap(md);
	if (NULL != scene)
	{
		/// todo
		/// deal with reduplicate building route
		m_player_route_map[player_guid] = scene;
	}
	else
	{
		// error , never to happy
	}
}

Scene * SceneDepotImp::getSceneByMap(uint64 map_id)
{
	/// todo
	///m_scene_map_mutex 这个锁以后优化掉，scene的动态添加和删除，通过消息来处理，
	///即把这两个事件转换成消息添加到input packet队列里处理，这样就可以不用这个锁了。
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_scene_map_mutex, NULL);
	if (m_scene_map.size() == 0)
	{
		return NULL;
	}

	SceneMap_t::iterator it = m_scene_map.find(map_id);
	if (it != m_scene_map.end())
	{
		return it->second;
	}
	else
	{
		//直连模式
		if (0 == m_scene_deopt_cfg.gate_type)
		{
			return m_scene_map.begin()->second;
		}
		else
		{
			return NULL;
		}
	}
}

Scene * SceneDepotImp::getSceneByPalyer(uint64 player_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_player_route_map_mutex, NULL);
	if (0 == player_guid)
	{
		// for the member charge, if player_guid is zero, the message is coming from member system
		if (m_scene_map.size() > 0)
		{
			return m_scene_map.begin()->second;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		PlayerRouteMap_t::iterator it = m_player_route_map.find(player_guid);
		if (m_player_route_map.end() != it)
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}
}

void SceneDepotImp::dispatchTerminalPacket(Packet * packet)
{
	Scene * scene = getSceneByPalyer(packet->guid());
	if (NULL != scene)
	{
		scene->input(packet);
	}
	else
	{
		/// todo
		delete packet;
	}
}

void SceneDepotImp::dispatchCachePacket(const CachePackInfo & cache_pack_info)
{
	Scene * scene = getSceneByMap(cache_pack_info.map_id);
	if (NULL != scene)
	{
		scene->cacheInput(cache_pack_info.packet, cache_pack_info.map_id, cache_pack_info.request_id);
	}
	else
	{
		/// todo
		DEF_LOG_ERROR("error to get scene by map id in SceneDepotImp::dispatchCachePacket, map id is <%s>, guid is <%s>\n", boost::lexical_cast<string>(cache_pack_info.map_id).c_str(), boost::lexical_cast<string>(cache_pack_info.packet->guid()).c_str());
		delete cache_pack_info.packet;
	}
}

void SceneDepotImp::lostGateConnection()
{
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_scene_map_mutex, );
		if (m_lost_gate_connection)
		{
			return ;
		}

		m_lost_gate_connection = true;
	}

	notifyAllScenesToFlushPlayerData();
}

void SceneDepotImp::recvSingleUsr1()
{
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_scene_map_mutex, );
		if (m_recv_sigusr1)
		{
			return ;
		}

		m_recv_sigusr1 = true;
	}

	notifyAllScenesToFlushPlayerData();
}

void SceneDepotImp::notifyAllScenesToFlushPlayerData()
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_scene_map_mutex, );
	if (m_notify_all_scenes_to_flush_player_data)
	{
		return ;
	}

	m_notify_all_scenes_to_flush_player_data = true;
	for (SceneMap_t::iterator it = m_scene_map.begin(); it != m_scene_map.end(); ++it)
	{
		MAKE_NEW_PACKET(ps, SMSG_LOST_GATE_CONNECTION, 0, "");
		Scene * scene = it->second;
		scene->input(ps);
	}
}

void SceneDepotImp::checkSceneStateAfterLostGateConnection()
{
	bool finish_to_flush = true;
	for (SceneMap_t::iterator it = m_scene_map.begin(); it != m_scene_map.end(); ++it)
	{
		Scene * scene = it->second;
		//if (scene->isLineScene())
		//{
		//	continue;
		//}

		if (!scene->isShutdownSuccess())
		{
			finish_to_flush = false;
		}
	}

	if (finish_to_flush)
	{
		allScenesFinishToFlushPlayers();
	}
}

void SceneDepotImp::checkSceneStateStartupSuccess()
{
	bool success_to_init = true;
	for (SceneMap_t::iterator it = m_scene_map.begin(); it != m_scene_map.end(); ++it)
	{
		Scene * scene = it->second;
		//if (!scene->isLineScene())
		//{
		//	continue;
		//}

		if (!scene->isStartupSuccess())
		{
			success_to_init = false;
		}
	}

	if (success_to_init)
	{
		m_success_startup_notify = true;
		allScenesStartupSuccessNotify();
	}
}

void SceneDepotImp::allScenesFinishToFlushPlayers()
{
	if (m_success_flush_data_notify)
	{
		return ;
	}

	m_success_flush_data_notify = true;

	DEF_LOG_INFO("\n\nall scenes finish to flush players\n\n\n");

	ACE_OS::sleep(3);

	MAKE_NEW_PACKET(ps, IMSG_GS_SHUTDOWN_REPORT, 1, "");

	sendNotifyMsg(ps);
}

void SceneDepotImp::allScenesStartupSuccessNotify()
{
	DEF_LOG_INFO("\n\nall scenes startup success\n\n\n");

	MAKE_NEW_PACKET(ps, IMSG_GS_STARTUP_REPORT, 1, "");

	sendNotifyMsg(ps);
}

void SceneDepotImp::sendNotifyMsg(Packet * packet)
{
	std::auto_ptr<Packet> auto_pack(packet);

	ACE_SOCK_Connector connector;
	ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> svc_handler;
	ACE_INET_Addr addr(m_scene_deopt_cfg.startup_notify_port, m_scene_deopt_cfg.local_addr.local_ip.c_str());
	if (connector.connect(svc_handler.peer(), addr) == -1)
	{
		DEF_LOG_ERROR("error to connect the addr <%s>, port <%d>\n", m_scene_deopt_cfg.local_addr.local_ip.c_str(), m_scene_deopt_cfg.startup_notify_port);
	}

	int sn = svc_handler.peer().send_n(packet->stream(), packet->stream_size());
	ACE_OS::sleep(3);
	svc_handler.shutdown();
}
