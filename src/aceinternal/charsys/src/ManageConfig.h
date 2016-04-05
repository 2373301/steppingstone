#ifndef _MANAGECONFIG_HPP
#define _MANAGECONFIG_HPP

#include <string>
#include <ace/Null_Mutex.h>
#include "SimpleXML.h"
#include "SingletonInit.h"
#include "Singleton.h"
#include "typedef.h"
#include "DatabasePool.h"

using std::string;

struct GateCfg 
{
	GateCfg()
	: port(0)
	{}

	int port;
	string ip;
};

struct CacheConfig 
{
	string ip;
};

struct GUIDCfg
{
	GUIDCfg()
		: m_ggen_value(0)
		, m_index_value(0)
	{}

	int8	m_ggen_value;

	uint64	m_index_value;
};

struct MapInfo
{
	MapInfo()
	: map_id(0)
	, born_x(0)
	, born_y(0)
	, faith(1)
	, press_test(false)
	{}

	uint64 map_id;  // map template
//	string map_instance;
	int born_x;
	int born_y;
	int faith;
	bool press_test;
};

struct LogsysCfg
{
	LogsysCfg()
	{}

	string port;
	string ip;
};

typedef std::vector<GateCfg> GateCfgVec_t;

typedef std::vector<CacheConfig> CacheConfigVec_t;


class ManageConfig : public SingletonInit
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
	/// AppNotifyInterface init
	virtual int init();

	/// AppNotifyInterface fini, stop
	virtual int stop();

	/// AppNotifyInterface wait for finish
	virtual int finit();
public:

	bool loadConfig(const string & config_file);

	const CacheConfigVec_t & getCacheConfig();

	const GateCfgVec_t & getGateCfg();

	const SQLCfg & getAccountSQLCfg();

	const GUIDCfg & getGUIDCfg() const;

	const string & getGUIDAddr() const;

	const string & getUnfawfulFile() const ;

	const LogsysCfg & getLogsysCfg() const;

	const string & getInitPlayerCfg() const;

	const string & getInitJobCfg() const;

	const string & getRoleCfg() const;

	const string & getRandomNameCfg() const ;

	int getLineId() const;

protected:
	bool loadAccountSQLCfg(Element * root_element);

	bool loadGateCfg(Element * root_element);

	bool loadCacheConfig(Element * root_element);

	//bool loadGenGUID(Element * root_element);

	bool loadUnlawfulInfo(Element * root_element);

	bool loadLogsysInfo(Element * root_element);

	bool loadGUIDAddr(Element * root_element);

	bool loadInitialization(Element * root_element);

	bool loadLineInfo(Element * root_element);

	void reportParam();
private:
	GUIDCfg m_guid_cfg;

	SQLCfg m_account_sql_cfg;

	GateCfgVec_t m_gate_cfg;

	CacheConfigVec_t m_cache_cfg;

	string m_unlawful_file;

	bool m_press_test;

	int m_line_id;

	string m_guid_addr;

	LogsysCfg m_logsys_cfg;

	string m_init_player_file;

	string m_init_job_file;
	
	string m_role_file;

	string m_random_name_file;
};

#define sMsgCfg ManageConfig::instance()

#endif