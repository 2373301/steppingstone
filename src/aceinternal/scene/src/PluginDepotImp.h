
#ifndef PLUGIN_DEPOT_IMP_HPP
#define PLUGIN_DEPOT_IMP_HPP

#include "Plugin.h"
#include "PluginDepot.h"
#include "ManagePluginConfig.h"

class PluginDepotImp : public PluginDepot
{
public:
	~PluginDepotImp();

public:
	virtual int input(Packet * packet);

	virtual int input(const PackInfo & pack_info);

	virtual int notify(const PackInfo & pack_info);

	virtual int request(const PackInfo & pack_info);

	virtual int gmcmd(const string & gm_name, const vector<string> & gm_param, uint64 target_guid);

	virtual bool isStartupSuccess();

	virtual bool isShutdownSuccess();
protected:
	virtual int initing();

	bool buildMsgMapping();

	bool buildInputMsgMapping();

	bool buildRequestMsgMapping();

	bool buildNotifyMsgMapping();

	bool buildGmcmdMsgMapping();

	bool isAvailablePlugin(Plugin * plugin);
protected:
	Input_Msg_Type_Map	m_input_msg_map;

	Request_Msg_Type_Map	m_request_msg_map;

	Notify_Msg_Type_Map		m_notify_msg_map;

	Gmcmd_Msg_Type_Map		m_gmcmd_msg_map;

	vector<Plugin *>		m_plugin_class_vec;

	ManagePluginConfig		m_manage_lugin_config;
private:
};

#endif