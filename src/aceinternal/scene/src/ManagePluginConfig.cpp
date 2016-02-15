
#include "ManagePluginConfig.h"
#include "Logger.h"

ManagePluginConfig::ManagePluginConfig()
{

}

ManagePluginConfig::~ManagePluginConfig()
{

}

int ManagePluginConfig::init()
{
	if (loadConfig("plugin_config.xml"))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

bool ManagePluginConfig::checkPlugin(int scene_type, int scene_id, int plugin_id, bool & is_valid)
{
	PluginDefineCfgMap_t::iterator it = m_avoid_plugin_define_cfg_map.find(plugin_id);
	if (it != m_avoid_plugin_define_cfg_map.end())
	{
		PluginDefineCfg * plugin_def = it->second;
		is_valid = plugin_def->is_valid(scene_type,scene_id);
		if (is_valid)
		{
			is_valid = false;
			return true;
		}
	}

	it = m_valid_plugin_define_cfg_map.find(plugin_id);
	if (it != m_valid_plugin_define_cfg_map.end())
	{
		PluginDefineCfg * plugin_def = it->second;
		is_valid = plugin_def->is_valid(scene_type,scene_id);
		return true;
	}

	return false;
}

bool ManagePluginConfig::loadConfig(const string & config_name)
{
	bool result = false;
	std::auto_ptr<Document> xml_doc(XMLFactory::create_document());
	try
	{
		if (xml_doc->load_file(config_name))
		{
			Element * root_ele = xml_doc->get_root();

			result = true;
			result = loadValidPluginCfg(root_ele) && result;
			result = loadAvoidPluginCfg(root_ele) && result;
		}
		else
		{
			DEF_LOG_ERROR("failed to load config by xml document : %s\n", config_name.c_str());
		}
	}
	catch (...)
	{
		DEF_LOG_ERROR("unknown exception in ManageConfig::loadConfig\n");
		result = false;
	}

	return result;
}

bool ManagePluginConfig::loadValidPluginCfg(Element * root_element)
{
	Element * valid_ele = root_element->get_element("valid_for_scene");
	if (NULL == valid_ele)
	{
		return true;
	}

	Elements plugin_eles = valid_ele->get_elements("plugin");

	for (Elements::iterator it = plugin_eles.begin(); it != plugin_eles.end(); ++it)
	{
		Element * ele = *it;
		if (!loadPluginSceneCfg(ele, m_valid_plugin_define_cfg_map))
		{
			return false;
		}
	}

	return true;
}

bool ManagePluginConfig::loadAvoidPluginCfg(Element * root_element)
{
	Element * valid_ele = root_element->get_element("avoid_for_scene");
	if (NULL == valid_ele)
	{
		return true;
	}

	Elements plugin_eles = valid_ele->get_elements("plugin");

	for (Elements::iterator it = plugin_eles.begin(); it != plugin_eles.end(); ++it)
	{
		Element * ele = *it;
		if (!loadPluginSceneCfg(ele, m_avoid_plugin_define_cfg_map))
		{
			return false;
		}
	}

	return true;
}

bool ManagePluginConfig::loadPluginSceneCfg(Element * element, PluginDefineCfgMap_t & plugin_define_cfg_map)
{
	uint32 plugin_id = 0;
	if (!getAttrValue(element, "id", plugin_id))
	{
		DEF_LOG_ERROR("failed to get id of plugin\n");
		return false;
	}

	PluginDefineCfg * plugin_def_cfg = new PluginDefineCfg();
	plugin_def_cfg->plugin_id = plugin_id;

	Elements eles = element->get_elements("scene");
	for (Elements::iterator it = eles.begin(); it != eles.end(); ++it)
	{
		Element * ele = *it;
		uint32 scene_type = 0;
		Uint32Vec_t scene_id;
		if (!getAttrValue(ele, "type", scene_type))
		{
			DEF_LOG_ERROR("failed to get type of scene\n");
			return false;
		}
		if (!getAttrValue(ele, "id", scene_id))
		{
			DEF_LOG_ERROR("failed to get id of scene\n");
			return false;
		}

		Uint32Set_t scene_id_set;
		for (int i = 0; i < scene_id.size(); ++i)
		{
			scene_id_set.insert(scene_id[i]);
		}

		plugin_def_cfg->scene_type_map.insert(std::make_pair(scene_type, scene_id_set));
	}

	plugin_define_cfg_map.insert(std::make_pair(plugin_def_cfg->plugin_id, plugin_def_cfg));

	return true;
}