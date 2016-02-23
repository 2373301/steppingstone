#ifndef _MANAGECONFIG_HPP
#define _MANAGECONFIG_HPP

#include <string>
#include <ace/Null_Mutex.h>
#include "SimpleXML.h"
#include "AppNotifyInterface.h"
#include "Singleton.h"
#include "typedef.h"

using std::string;

struct SQLCfg
{
	SQLCfg()
	: port(3306)
	, pool_number(4)
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

struct MapInfo
{
	MapInfo()
	: map_id(0)
	, born_x(0)
	, born_y(0)
	{}

	int map_id;
	string map_instance;
	int born_x;
	int born_y;
};

struct ClientCfg
{
	ClientCfg()
	: listen_port(13100)
	, handle_input_stream_thread_pool_number(10)
	, handle_packet_thread_pool_number(10)
	, beat_interval(120)
	, lost_beat_time(3)
	, beat_check_interval(30)
	, client_connection_timeout(120)
	, max_socket(4096)
	, socket_buffer_length(202400)
	, recv_buffer_length(50000)
	, cooler_time_interval(60)
	, robot_press_test(false)
	, host("0.0.0.0")
	, stream_limit_spead(4096)
	, stream_stat_interval(20)
	, encry_type(3)
	{}

	int listen_port;

	int handle_input_stream_thread_pool_number;

	int handle_packet_thread_pool_number;

	int beat_interval;

	int lost_beat_time;

	int beat_check_interval;

	int client_queue_length;

	int client_connection_timeout;

	int max_socket;

	int socket_buffer_length;

	int recv_buffer_length;

	int cooler_time_interval;

	bool robot_press_test;

	string host;

	int stream_limit_spead;

	int stream_stat_interval;

	int encry_type;
};

struct GSCfg
{
	GSCfg()
	: listen_port(13200)
	, handle_stream_thread_pool_number(4)
	, handle_packet_thread_pool_number(4)
	, socket_buffer_length(1024000)
	, recv_window_size(65535)
	, send_window_size(65535)
	, host("0.0.0.0")
	{}

	int listen_port;

	int handle_stream_thread_pool_number;

	int handle_packet_thread_pool_number;

	int socket_buffer_length;

	int recv_window_size;

	int send_window_size;

	string host;

};

struct LSCfg 
{
	LSCfg()
	: listen_port(13300)
	, recv_port(4097)
	, report_interval(300)
	, host("0.0.0.0")
	, validation_interval(180)
	, check_client_auth(1)
	{}

	typedef list<string> LoginServerList;

	int listen_port;

	int recv_port;

	int report_interval;

	string host;

	int validation_interval;

	int check_client_auth;

	LoginServerList login_server_list;
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

struct StatCfg
{
	StatCfg()
	: report_interval(30)
	{}

	int32	report_interval;
};

struct RoleCfg 
{
	RoleCfg()
	: listen_port(13400)
	, host("0.0.0.0")
	, handle_packet_thread_pool_number(1)
	{}

	int listen_port;

	string host;

	int handle_packet_thread_pool_number;
};

struct LogCfg 
{
	LogCfg()
	: log_level(0)
	, is_print(true)
	, enable_remote(false)
	{}

	int log_level;
	bool is_print;
	bool enable_remote;
};

struct RemoteLogCfg 
{
	RemoteLogCfg()
		: log_level(0)
	{}

	int log_level;
	string addr;
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
	/// AppNotifyInterface init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// AppNotifyInterface fini, stop
	virtual int fini();

	/// AppNotifyInterface wait for finish
	virtual int wait();
public:
	const SQLCfg & getSQLCfg() const;

	const MapInfo & getMapInfo() const;

	const ClientCfg & getClientCfg() const;

	const GSCfg & getGSCfg() const;

	const LSCfg & getLSCfg() const;

	//const GUIDCfg & getGUIDCfg() const;

	const StatCfg & getStatCfg() const;

	const RoleCfg & getRoleCfg() const;

	const LogCfg & getLogCfg() const;

	const RemoteLogCfg & getRemoteLogCfg() const;

	const string & getQueryAddr() const;

	int64 getGateID();
protected:
	bool loadConfig(const string & config_file);

	bool loadSQLCfg(Element * root_element);

	bool loadMapInfo(Element * root_element);

	bool loadClientCfg(Element * root_element);

	bool loadGSCfg(Element * root_element);

	bool loadLSCfg(Element * root_element);

	//bool loadGUIDCfg(Element * root_element);

	bool loadStatCfg(Element * root_element);

	bool loadRoleCfg(Element * root_element);

	bool loadTranscriptConditionFile(Element * root_element);

	bool loadLogCfg(Element * root_element);

	bool loadRemoteLogCfg(Element * root_element);

	bool loadQueryCfg(Element * root_element);

	void reportParam();
private:
	int64 m_gate_id;

	string m_transcript_condition_file;

	SQLCfg m_sql_cfg;

	MapInfo m_map_info;

	ClientCfg m_client_cfg;

	GSCfg m_gs_cfg;

	LSCfg m_ls_cfg;

	//GUIDCfg m_guid_cfg;

	StatCfg m_stat_cfg;

	RoleCfg m_role_cfg;

	LogCfg m_log_cfg;

	RemoteLogCfg m_remote_log_cfg;

	string m_query_addr;
};

#define sMsgCfg ManageConfig::instance()

#endif
