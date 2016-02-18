
#ifndef MANAGE_PLUGIN_CONFIG_HPP
#define MANAGE_PLUGIN_CONFIG_HPP

#include "typedef.h"
#include "SimpleXML.h"

struct PluginDefineCfg 
{
	PluginDefineCfg()
	: plugin_id(0)
	{}

	bool is_valid(int st, int si)
	{
		SceneTypeMapt_t::iterator it = scene_type_map.find(st);
		if (it != scene_type_map.end())
		{
			Uint32Set_t & scene_set = it->second;
			if (scene_set.find(si) != scene_set.end())
			{
				return true;
			}
		}

		return false;
	}

	int plugin_id;

	typedef map<int, Uint32Set_t> SceneTypeMapt_t;

	SceneTypeMapt_t scene_type_map;
};

class ManagePluginConfig
{
public:
	ManagePluginConfig();
	~ManagePluginConfig();
public:
	int init();

	// return true if define
	bool checkPlugin(int scene_type, int scene_id, int plugin_id, bool & is_valid);
protected:
	typedef map<int, PluginDefineCfg *> PluginDefineCfgMap_t;

	bool loadConfig(const string & config_name);

	bool loadValidPluginCfg(Element * root_element);

	bool loadAvoidPluginCfg(Element * root_element);

	bool loadPluginSceneCfg(Element * element, PluginDefineCfgMap_t & plugin_define_cfg_map);
private:

	PluginDefineCfgMap_t m_valid_plugin_define_cfg_map;
	PluginDefineCfgMap_t m_avoid_plugin_define_cfg_map;
};
#endif