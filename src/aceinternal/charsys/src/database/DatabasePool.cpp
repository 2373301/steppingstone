
#include <ace/OS.h>
#include <ace/Global_Macros.h>
#include <ace/Guard_T.h>
#include "DatabasePool.h"
#include "Logger.h"

DatabasePool::DatabasePool()
{

}

DatabasePool::~DatabasePool()
{

}

bool DatabasePool::init(const SQLCfg sql_cfg)
{
	bool result = false;

	m_sql_cfg = sql_cfg;

	for (int i = 0; i < m_sql_cfg.pool_number; ++i)
	{
		mysqlpp::Connection * con = new mysqlpp::Connection(false);
		con->set_option(new mysqlpp::SetCharsetNameOption("utf8"));

		if (!con->connect(m_sql_cfg.database_name.c_str(), m_sql_cfg.server_ip.c_str(), m_sql_cfg.user_name.c_str(), m_sql_cfg.password.c_str(), m_sql_cfg.port))
		{
			DEF_LOG_ERROR("Failed to connect the database, error is <%s>\n", con->error());
			result = false;
			delete con;
			break;
		}
		else
		{
			m_connection_pool_list.push_back(con);
			result = true;
		}
	}
	return result;
}

mysqlpp::Connection * DatabasePool::getConnection()
{
	mysqlpp::Connection * result = NULL;
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_connction_mutex, NULL);
		if (m_connection_pool_list.size() > 0)
		{
			result = *m_connection_pool_list.begin();
			m_connection_pool_list.pop_front();
			return result;
		}
	}

	// not find
	mysqlpp::Connection * con = new mysqlpp::Connection(false);
	con->set_option(new mysqlpp::SetCharsetNameOption("utf8"));

	if (!con->connect(m_sql_cfg.database_name.c_str(), m_sql_cfg.server_ip.c_str(), m_sql_cfg.user_name.c_str(), m_sql_cfg.password.c_str(), m_sql_cfg.port))
	{
		DEF_LOG_ERROR("Failed to connect the database, error is <%s>\n", con->error());
		result = NULL;
		delete con;
	}
	else
	{
		result = con;
	}

	return result;
}

void DatabasePool::returnConnection(mysqlpp::Connection * connection)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_connction_mutex, );
	if (m_connection_pool_list.size() < m_sql_cfg.pool_number)
	{
		m_connection_pool_list.push_back(connection);
	}
	else
	{
		connection->disconnect();
		delete connection;
	}
}

bool DatabasePool::pingMYSQL()
{
	mysqlpp::Connection * mysql_connection = NULL;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_connction_mutex, false);
	for (ConnectionPoolList::iterator it = m_connection_pool_list.begin(); it != m_connection_pool_list.end(); ++it)
	{
		mysql_connection = *it;
		mysqlpp::Query query = mysql_connection->query();
		query << "select count(*) from ping";
		mysqlpp::StoreQueryResult res = query.store();
		if (!res)
		{
			DEF_LOG_ERROR("Failed to ping mysql database\n");
			return false;
		}
	}

	return true;
}
