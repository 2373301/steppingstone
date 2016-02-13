
#ifndef MANAGE_CONFIG_HPP
#define MANAGE_CONFIG_HPP

#include <string>
#include <ace/Null_Mutex.h>
#include "SimpleXML.h"
#include "Singleton.h"
#include "typedef.h"
#include "SingletonInit.h"

using std::string;

struct SQLCfg
{
	SQLCfg()
		: port(3306)
		, pool_number(3)
		, ping_MYSQL_interval(15 * 60)
	{}

	int    port;

	int pool_number;

	int ping_MYSQL_interval;

	string server_ip;

	string database_name;

	string user_name;

	string password;
};

struct SerializeCfg 
{
	SerializeCfg()
	{}

	string backup_directory;
};

struct CacheCfg
{
	CacheCfg()
	{}

	vector<string>	cache_vec;
};

struct LogCfg 
{
	LogCfg()
	: log_level(0)
	, is_print(0)
	, enable_remote(0)
	, remote_log_level(0)
	{}

	int log_level;
	int is_print;
	int enable_remote;
	int remote_log_level;
	string remote_addr;
};

/***
 * @class SerializeFactory
 *
 * @brief 配置文件加载类
 */
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
	virtual int init();

	virtual int stop();

	virtual int finit();
public:
	CacheCfg & getCacheCfg();

	SQLCfg & getSQLCfg();

	SerializeCfg & getSerializeCfg();

	LogCfg & getLogCfg();

protected:
	bool loadConfig(const string & config_file);

	bool loadSQLCfg(Element * root_element);

	bool loadSerializeCfg(Element * root_element);

	bool loadCacheCfg(Element * root_element);

	bool loadLogCfg(Element * root_element);

	bool loadRemoteLogCfg(Element * root_element);

	void reportParam();
private:
	SQLCfg m_sql_cfg;

	SerializeCfg m_serialize_cfg;

	CacheCfg m_cache_cfg;

	LogCfg m_log_cfg;
};

#define sMsgCfg ManageConfig::instance()

#endif