
#ifndef DATABASE_POOL_HPP
#define DATABASE_POOL_HPP

#include <list>
#include <ace/Thread_Mutex.h>
#include <mysql++.h>

using namespace std;

struct SQLCfg
{
	SQLCfg()
		: port(3306)
		, pool_number(10)
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

class DatabasePool
{
public:
	DatabasePool();
	~DatabasePool();
public:
	typedef std::list<mysqlpp::Connection *>	ConnectionPoolList;

	bool init(const SQLCfg sql_cfg);

	mysqlpp::Connection * getConnection();

	void returnConnection(mysqlpp::Connection * connection);

	bool pingMYSQL();

private:
	SQLCfg m_sql_cfg;

	ACE_Thread_Mutex m_connction_mutex;

	ConnectionPoolList	m_connection_pool_list;
};

#endif