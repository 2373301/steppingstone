#include "PluginDepot.h"

IPluginDepot::IPluginDepot()
{
}

IPluginDepot::~IPluginDepot()
{

}

int IPluginDepot::IPluginDepot_init(const PluginDepotCfg & plugin_depot_cfg)
{
	if ((NULL == plugin_depot_cfg.scene) ||
		(plugin_depot_cfg.plugin_dll_vec.size() != plugin_depot_cfg.plugin_param_vec->size()))
	{
		DEF_LOG_ERROR("failed to init plugin depot, the scene is <%x>, plugin dll size is <%d>, plugin param vec is <%d>\n", 
			plugin_depot_cfg.scene, plugin_depot_cfg.plugin_dll_vec.size(), plugin_depot_cfg.plugin_param_vec->size());
		return -1;
	}

	m_plugin_depot_cfg = plugin_depot_cfg;
	return IPluginDepot_initing();
}