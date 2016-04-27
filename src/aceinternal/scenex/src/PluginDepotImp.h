
#ifndef PLUGIN_DEPOT_IMP_HPP
#define PLUGIN_DEPOT_IMP_HPP

#include "Plugin.h"
#include "PluginDepot.h"
#include "ManagePluginConfig.h"

class PluginDepotImp 
	: public IPluginDepot
{
public:
	~PluginDepotImp();

public:
	virtual int IPluginDepot_input(Packet * packet);

	virtual int IPluginDepot_input(const PackInfo & pack_info);

	virtual int IPluginDepot_notify(const PackInfo & pack_info);

	virtual int IPluginDepot_request(const PackInfo & pack_info);

	virtual int IPluginDepot_gmcmd(const string & gm_name, const vector<string> & gm_param, uint64 target_guid);

	virtual bool IPluginDepot_isStartupSuccess();

	virtual bool IPluginDepot_isShutdownSuccess();
protected:
	virtual int IPluginDepot_initing();

	bool buildMsgMapping();

	bool buildInputMsgMapping();

	bool buildRequestMsgMapping();

	bool buildNotifyMsgMapping();

	bool buildGmcmdMsgMapping();

	bool isAvailablePlugin(IPlugin * plugin);
protected:
	Input_Msg_Type_Map	m_input_msg_map;

	Request_Msg_Type_Map	m_request_msg_map;

	Notify_Msg_Type_Map		m_notify_msg_map;

	Gmcmd_Msg_Type_Map		m_gmcmd_msg_map;

	vector<IPlugin *>		m_plugin_class_vec;

	ManagePluginConfig		m_manage_lugin_config;
private:
};

#endif