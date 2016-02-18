
#ifndef MANAGE_CONFIG_HPP
#define MANAGE_CONFIG_HPP

#include "typedef.h"
#include "SimpleXML.h"
#include "SceneDepot.h"
#include "Singleton.h"
#include "Terminal.h"
#include "CacheInterface.h"

struct BroadcastCfg
{
	BroadcastCfg()
	{}

	string inline_addr;
	string outline_addr;
};

struct LogCfg 
{
	LogCfg()
	: log_level(LL_DEBUG)
	, is_printing(true)
	, enable_remote(false)
	{}

	int log_level;
	bool is_printing;
	bool enable_remote;
	string log_dir;
};

struct RemoteLogCfg 
{
	RemoteLogCfg()
	: log_level(LL_INFO)
	{}

	int log_level;
	string remote_addr;
};

class ManageConfig
{
public:
	ManageConfig();

	~ManageConfig();
public:
	static ManageConfig * instance()
	{
		return Singleton<ManageConfig>::instance();
	}
public:
	int init(const string & config_file);

	int stop();

	int finit();

	const SceneDepotCfg & getScenesCfg();

	const LocalAddrCfg & getLocalAddr();

	const GateCfg & getGateCfg();

	const CacheCfg & getCacheCfg();

	const LogCfg & getLogCfg();

	void setFirstLunchFalse();
protected:
	bool loadConfig(const string & config_file);

	void reportParam();
protected:
	bool loadScenesCfg(Element * root_element);

	bool loadLocalAddr(Element * root_element);

	bool loadGateCfg(Element * root_element);

	bool loadCacheCfg(Element * root_element);

	bool loadResourceCfg(Element * root_element);

	bool loadGuidSvrCfg(Element * root_element);

	bool loadLogCfg(Element * root_element);

	bool loadExchangeCfg(Element * root_element);

	bool loadRecordServerCfg(Element * root_element);

	bool loadGmServerCfg(Element * root_element);

	bool loadServerCfg(Element * root_element);

	bool loadCrossServerCfg(Element * root_element);

	bool loadReportServerCfg(Element * root_element);

	bool loadVipInitCfg(Element * root_element);
	
	bool loadRobotSecurityInfo(Element * root_element);

private:
	SceneDepotCfg	m_plugin_depot_cfg;

	LocalAddrCfg	m_local_addr;

	GateCfg			m_gate_cfg;

	CacheCfg		m_cache_cfg;

	ResourceCfg		m_resource_cfg;

	GuidSvrCfg		m_guid_svr_cfg;

	LogCfg			m_log_cfg;

	string			m_config_file;
};
#endif