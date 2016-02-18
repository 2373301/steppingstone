
#include <iostream>
#include "ManageConfig.h"
#include "Logger.h"

ManageConfig::ManageConfig()
{

}

ManageConfig::~ManageConfig()
{

}

int ManageConfig::init(const string & config_file)
{
	if (loadConfig(config_file))
	{
		reportParam();
		return 0;
	}
	else
	{
		DEF_LOG_ERROR("failed to load config file : %s\n", config_file.c_str()) ;
		return -1;
	}
}

int ManageConfig::stop()
{
	return 0;
}

int ManageConfig::finit()
{
	return 0;
}

const SceneDepotCfg & ManageConfig::getScenesCfg()
{
	m_plugin_depot_cfg.gate_type = m_gate_cfg.type;
	m_plugin_depot_cfg.resource_cfg = m_resource_cfg;
	m_plugin_depot_cfg.guid_svr_cfg = m_guid_svr_cfg;
	//m_plugin_depot_cfg.log_dir = m_log_cfg.log_dir;
	return m_plugin_depot_cfg;
}

const LocalAddrCfg & ManageConfig::getLocalAddr()
{
	return m_local_addr;
}

const GateCfg & ManageConfig::getGateCfg()
{
	return m_gate_cfg;
}

const CacheCfg & ManageConfig::getCacheCfg()
{
	return m_cache_cfg;
}

const LogCfg & ManageConfig::getLogCfg()
{
	return m_log_cfg;
}

void ManageConfig::setFirstLunchFalse()
{
	std::auto_ptr<Document> xml_doc(XMLFactory::create_document());
	try
	{
		if (xml_doc->load_file(m_config_file))
		{
			Element * root_ele = xml_doc->get_root();
			Element * scenes_ele = root_ele->get_element("scenes");
			if (NULL == scenes_ele)
			{
				DEF_LOG_ERROR("failed to get scenes note in config xml\n");
				return ;
			}

			Attributes attrs = scenes_ele->get_attributes();
			for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
			{
				Attribute * attr = *it;
				if (attr->get_name() == "is_first_launch")
				{
					attr->set_int(0);
					break;
				}
			}
			
			xml_doc->save_file(m_config_file);
		}
	}
	catch(...)
	{

	}
}

bool ManageConfig::loadConfig(const string & config_file)
{
	m_config_file = config_file;

	bool result = false;
	std::auto_ptr<Document> xml_doc(XMLFactory::create_document());
	try
	{
		if (xml_doc->load_file(config_file))
		{
			Element * root_ele = xml_doc->get_root();

			result = true;
			result = loadScenesCfg(root_ele) && result;
			result = loadLocalAddr(root_ele) && result;
			result = loadGateCfg(root_ele) && result;
			result = loadCacheCfg(root_ele) && result;
			result = loadResourceCfg(root_ele) && result;
			result = loadGuidSvrCfg(root_ele) && result;
			result = loadExchangeCfg(root_ele) && result;
			result = loadRecordServerCfg(root_ele) && result;
			result = loadGmServerCfg(root_ele) && result;
			result = loadServerCfg(root_ele) && result;
			result = loadCrossServerCfg(root_ele) && result;
			result = loadReportServerCfg(root_ele) && result;
			result = loadVipInitCfg(root_ele) && result;
			result = loadRobotSecurityInfo(root_ele) && result;
			//result = loadLogCfg(root_ele) && result;
		}
		else
		{
			DEF_LOG_ERROR("failed to load config by xml document : %s\n", config_file.c_str());
		}
	}
	catch (...)
	{
		DEF_LOG_ERROR("unknown exception in ManageConfig::loadConfig\n");
		result = false;
	}

	return result;
}

void ManageConfig::reportParam()
{

}

bool ManageConfig::loadScenesCfg(Element * root_element)
{
	Element * scenes_ele = root_element->get_element("scenes");
	if (NULL == scenes_ele)
	{
		DEF_LOG_ERROR("failed to get scenes note in config xml\n");
		return false;
	}

	Attributes attrs = scenes_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "plugin_dir")
		{
			m_plugin_depot_cfg.plugin_dir = attr->get_value();
		}
		else if (attr->get_name() == "save_packet")
		{
			m_plugin_depot_cfg.save_packet = attr->as_int() == 1;
		}
		else if (attr->get_name() == "push_client_error_msg")
		{
			m_plugin_depot_cfg.push_client_error_msg = attr->as_int() == 1;
		}
		else if (attr->get_name() == "is_first_launch")
		{
			m_plugin_depot_cfg.is_first_launch = attr->as_int() == 1;
		}
		else if (attr->get_name() == "startup_notify_port")
		{
			m_plugin_depot_cfg.startup_notify_port = attr->as_int();
		}
		else if (attr->get_name() == "max_sceen_number")
		{
			m_plugin_depot_cfg.max_sceen_number = attr->as_int();
		}
		else if (attr->get_name() == "enable_gm")
		{
			m_plugin_depot_cfg.enable_gm = attr->as_int() == 1;
		}
	}

	Elements line_eles = scenes_ele->get_elements();
	for (Elements::iterator it = line_eles.begin(); it != line_eles.end(); ++it)
	{
		Element * line_ele = *it;
		Elements items = line_ele->get_elements();
		Attribute * line_attr = line_ele->get_attribute("id");
		if (NULL == line_attr)
		{
			// error
			break;
		}

		SceneCfg scene_cfg;
		int line_no = line_attr->as_int(0);
		scene_cfg.line_no = line_no;

		for (Elements::iterator it = items.begin(); it != items.end(); ++it)
		{
			Element * scene_ele = *it;

			attrs = scene_ele->get_attributes();
			for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
			{
				Attribute * attr = *it;
				if (attr->get_name() == "id")
				{
					//scene_cfg.scene_id = attr->as_int();
					scene_cfg.map_id_vec.push_back(attr->as_int());
				}
			}
			//scene_cfg.line_no = line_no;
			//m_plugin_depot_cfg.scenes_cfg.push_back(scene_cfg);
		}

		m_plugin_depot_cfg.scenes_cfg.push_back(scene_cfg);
	}

	return true;
}

bool ManageConfig::loadLocalAddr(Element * root_element)
{
	Element * local_ele = root_element->get_element("local");
	if (NULL == local_ele)
	{
		DEF_LOG_ERROR("failed to get local note in config xml\n");
		return false;
	}

	Attributes attrs = local_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "ip")
		{
			m_local_addr.local_ip = attr->get_value();
		}
		else if (attr->get_name() == "port")
		{
			m_local_addr.local_port = attr->as_int();
		}
	}

	m_plugin_depot_cfg.local_addr = m_local_addr;

	return true;
}

bool ManageConfig::loadGateCfg(Element * root_element)
{
	Element * gates_ele = root_element->get_element("gates");
	if (NULL == gates_ele)
	{
		std::cout << "failed to get gates note in config xml" << std::endl;
		return false;
	}

	Attributes attrs = gates_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "type")
		{
			m_gate_cfg.type = attr->as_int();
		}
		else if (attr->get_name() == "networkThreadNo")
		{
			m_gate_cfg.network_thread_no = attr->as_int(1);
		}
	}

	Elements items = gates_ele->get_elements();
	for (Elements::iterator it = items.begin(); it != items.end(); ++it)
	{
		Element * gate_ele = *it;

		attrs = gate_ele->get_attributes();
		for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
		{
			Attribute * attr = *it;
			if (attr->get_name() == "addr")
			{
				m_gate_cfg.gate_addrs.push_back(attr->get_value());
			}
		}
	}

	return true;
}

bool ManageConfig::loadCacheCfg(Element * root_element)
{
	Element * caches_ele = root_element->get_element("caches");
	if (NULL == caches_ele)
	{
		std::cout << "failed to get caches note in config xml" << std::endl;
		return false;
	}

	Attributes attrs = caches_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "type")
		{
			m_cache_cfg.remote_cfg.type = attr->as_int();
		}
		else if (attr->get_name() == "local_path")
		{
			m_cache_cfg.local_path = attr->as_cstr();
		}
		else if (attr->get_name() == "networkThreadNo")
		{
			m_cache_cfg.network_thread_no = attr->as_int();
		}
	}

	Elements items = caches_ele->get_elements();
	for (Elements::iterator it = items.begin(); it != items.end(); ++it)
	{
		Element * cache_ele = *it;

		attrs = cache_ele->get_attributes();
		for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
		{
			Attribute * attr = *it;
			if (attr->get_name() == "addr")
			{
				m_cache_cfg.remote_cfg.cache_addrs.push_back(attr->get_value());
			}
		}
	}

	return true;
}

bool ManageConfig::loadResourceCfg(Element * root_element)
{
	Element * resource_ele = root_element->get_element("resource");
	if (NULL == resource_ele)
	{
		DEF_LOG_ERROR("failed to get resource_ele note in config xml\n");
		return false;
	}

	Attribute * attr = resource_ele->get_attribute("dir");
	if (NULL == attr)
	{
		DEF_LOG_ERROR("failed to get dir attr of resource element\n");
		return false;
	}

	m_resource_cfg.resource_dir = attr->as_cstr();
	return true;
}

bool ManageConfig::loadGuidSvrCfg(Element * root_element)
{
	Element * guid_ele = root_element->get_element("guid");
	if (NULL == guid_ele)
	{
		DEF_LOG_ERROR("failed to get guid_ele note in config xml\n");
		return false;
	}

	Attribute * attr = guid_ele->get_attribute("addr");
	if (NULL == attr)
	{
		DEF_LOG_ERROR("failed to get addr attr of guid element\n");
		return false;
	}

	m_guid_svr_cfg.guid_addr = attr->as_cstr();
	return true;
}

bool ManageConfig::loadLogCfg(Element * root_element)
{
	Element * log_ele = root_element->get_element("log");
	if (NULL == log_ele)
	{
		DEF_LOG_ERROR("failed to get log_ele note in config xml\n");
		return false;
	}

	Attributes attrs = log_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "level")
		{
			if (attr->get_value() == "LL_DEBUG")
			{
				m_log_cfg.log_level = LL_DEBUG;
			}
			else if (attr->get_value() == "LL_INFO")
			{
				m_log_cfg.log_level = LL_INFO;
			}
			else if (attr->get_value() == "LL_WARNING")
			{
				m_log_cfg.log_level = LL_WARNING;
			}
			else if (attr->get_value() == "LL_ERROR")
			{
				m_log_cfg.log_level = LL_ERROR;
			}
			else if (attr->get_value() == "LL_FATAL")
			{
				m_log_cfg.log_level = LL_FATAL;
			}
		}
		else if (attr->get_name() == "dir")
		{
			m_log_cfg.log_dir = attr->as_cstr();
		}
		else if (attr->get_name() == "print")
		{
			m_log_cfg.is_printing = attr->get_value() == "true";
		}
		else if (attr->get_name() == "enable_remote")
		{
			m_log_cfg.enable_remote = attr->get_value() == "true";
		}
	}

	return true;
}

bool ManageConfig::loadExchangeCfg(Element * root_element)
{
	Element * local_ele = root_element->get_element("exchange");
	if (NULL == local_ele)
	{
		DEF_LOG_ERROR("failed to get exchange note in config xml\n");
		return false;
	}

	Attributes attrs = local_ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "addr")
		{
			m_plugin_depot_cfg.exchange_cfg.exchange_addr = attr->get_value();
		}
		else if (attr->get_name() == "security_key")
		{
			m_plugin_depot_cfg.exchange_cfg.security_key = attr->get_value();
		}
	}

	return true;
}

bool ManageConfig::loadRecordServerCfg(Element * root_element)
{
	Element * ele = root_element->get_element("datarecord");
	if (NULL == ele)
	{
		DEF_LOG_ERROR("failed to get datarecord note in config xml\n");
		return false;
	}

	Attribute * attr = ele->get_attribute("addr");
	if (NULL == attr)
	{
		DEF_LOG_ERROR("failed to get addr of datarecord note in config xml\n");
		return false;
	}

	m_plugin_depot_cfg.record_server_addr = attr->as_cstr();

	return true;
}

bool ManageConfig::loadGmServerCfg(Element * root_element)
{
	Element * ele = root_element->get_element("gmserver");
	if (NULL == ele)
	{
		DEF_LOG_ERROR("failed to get gmserver note in config xml\n");
		return false;
	}

	Attribute * attr = ele->get_attribute("addr");
	if (NULL == attr)
	{
		DEF_LOG_ERROR("failed to get addr of datarecord note in config xml\n");
		return false;
	}

	m_plugin_depot_cfg.gm_server_addr = attr->as_cstr();

	return true;
}

bool ManageConfig::loadServerCfg(Element * root_element)
{
	Element * ele = root_element->get_element("server");
	if (NULL == ele)
	{
		DEF_LOG_ERROR("failed to get server note in config xml\n");
		return false;
	}
	
	Attributes attrs = ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "platform_id")
		{
			m_plugin_depot_cfg.server_cfg.platform_id = attr->as_int();
		}
		else if (attr->get_name() == "id")
		{
			m_plugin_depot_cfg.server_cfg.server_id = attr->get_value();
		}
		else if (attr->get_name() == "platform")
		{
			m_plugin_depot_cfg.server_cfg.platform = attr->get_value();
		}
		else if (attr->get_name() == "version")
		{
			m_plugin_depot_cfg.server_cfg.curr_version = attr->get_value();
		}
		else if (attr->get_name() == "report_id")
		{
			m_plugin_depot_cfg.server_cfg.report_id = attr->as_int();
		}
	}

	return true;
}

bool ManageConfig::loadCrossServerCfg(Element * root_element)
{
	Element * ele = root_element->get_element("crossserver");
	if (NULL == ele)
	{
		DEF_LOG_ERROR("failed to get crossserver note in config xml\n");
		return false;
	}

	Attributes attrs = ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "type")
		{
			m_plugin_depot_cfg.cross_server_cfg.server_type = attr->as_int();
		}
		else if (attr->get_name() == "addr")
		{
			m_plugin_depot_cfg.cross_server_cfg.cross_server_addr = attr->get_value();
		}
		else if (attr->get_name() == "security_key")
		{
			m_plugin_depot_cfg.cross_server_cfg.security_key = attr->get_value();
		}
	}

	return true;
}

bool ManageConfig::loadReportServerCfg(Element * root_element)
{
	Element * ele = root_element->get_element("battlereport");
	if (NULL == ele)
	{
		DEF_LOG_ERROR("failed to get battlereport note in config xml\n");
		return false;
	}

	Attributes attrs = ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "addr")
		{
			m_plugin_depot_cfg.battle_report_cfg.addr = attr->get_value();
		}
		else if (attr->get_name() == "security_key")
		{
			m_plugin_depot_cfg.battle_report_cfg.security_key = attr->get_value();
		}
	}

	return true;
}
bool ManageConfig::loadVipInitCfg(Element * root_element)
{
	Element * ele = root_element->get_element("vipinit");
	if (NULL == ele)
	{
		DEF_LOG_ERROR("failed to get vpinit note in config xml\n");
		return true;
	}

	Attributes attrs = ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "security_key")
		{
			m_plugin_depot_cfg.vip_init_security_key = attr->get_value();
		}
	}
	return true;
}

bool ManageConfig::loadRobotSecurityInfo(Element * root_element)
{
	Element * ele = root_element->get_element("robot");
	if (NULL == ele)
	{
		DEF_LOG_ERROR("failed to get robot note in config xml\n");
		return true;
	}

	Attributes attrs = ele->get_attributes();
	for (Attributes::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		Attribute * attr = *it;
		if (attr->get_name() == "security_key")
		{
			m_plugin_depot_cfg.robot_security_key = attr->get_value();
		}
	}
	return true;
}