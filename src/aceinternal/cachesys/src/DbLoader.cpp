
#include "DbLoader.h"
#include "EntityHander.h"
#include "config.h"

#define MYSQL_PING_INTERVAL_SEC 60 * 15

namespace cached {
namespace service {

DbLoader::DbLoader()
{
}

DbLoader::~DbLoader()
{
}

int DbLoader::init()
{
	if (this->activate(THR_NEW_LWP, 6) == -1)
	{
		return -1;
	}

	return 0;
}

int DbLoader::stop()
{
	m_stop = true;
	return 0;
}

int DbLoader::finit()
{
	while(m_wait)
	{
		DEF_LOG_INFO("waiting for DbLoader\n");
		ACE_OS::sleep(1);
	}

	ACE_OS::sleep(2);

	return 0;
}

int DbLoader::svc()
{
	DEF_LOG_INFO("MYSQL task is running......\n");

	::mysqlpp::Connection conn;
	conn.set_option(new mysqlpp::SetCharsetNameOption("utf8"));

	if (!this->connect_to_database(conn))
	{
		DEF_LOG_ERROR("connect the database failed\n");
		return -1;
	}

	DEF_LOG_INFO("sql client version is <%s>\n", conn.client_version().c_str());

	/// 
	ACE_Time_Value sleep_time(0, 1 * 1000);

	ACE_Time_Value last_ping_mysql_time = ACE_OS::gettimeofday();

	DataRequestInfoVec_t data_req_info_vec;

	while (!m_stop)
	{
		data_req_info_vec.clear();
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_data_req_info_vec_mutex, -1);
			std::copy(m_data_req_info_vec.begin(), m_data_req_info_vec.end(), std::back_inserter(data_req_info_vec));
			m_data_req_info_vec.clear();
		}

		if (data_req_info_vec.size() == 0)
		{
			ACE_Time_Value time_diff = ACE_OS::gettimeofday() - last_ping_mysql_time;
			if (time_diff.sec() >= MYSQL_PING_INTERVAL_SEC)
			{
				this->ping_db(conn);
				last_ping_mysql_time = ACE_OS::gettimeofday();
			}
			else
			{
				ACE_OS::sleep(sleep_time);
			}
			continue;
		}

		for (DataRequestInfoVec_t::iterator it = data_req_info_vec.begin(); it != data_req_info_vec.end(); ++it)
		{
			handleRequest(conn, *it);
		}
	}
	m_wait = false;
	return 0;
}

void DbLoader::process(DataRequestInfo * data_request_info)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_data_req_info_vec_mutex, );
	m_data_req_info_vec.push_back(data_request_info);
}

void DbLoader::handleRequest(::mysqlpp::Connection & conn, DataRequestInfo * data_request_info)
{
	DEF_LOG_DEBUG("start to process requst in DbLoader::handleRequest, opcode is <%d>, guid is <%llu>\n", data_request_info->opcode, data_request_info->guid);
	int opcode = data_request_info->opcode;
	if ((opcode == DCMSG_LOAD_ENTITY)
		|| (opcode == DCMSG_LOAD_ENTITY_ONCE)
		)
	{
		handleLoadRequest(conn, data_request_info);
	}
	else
	{
		DEF_LOG_ERROR("failed to handle request, get unknown optype <%d>, guid <%llu>\n", data_request_info->opcode, data_request_info->guid);
	}
}

void DbLoader::handleLoadRequest(::mysqlpp::Connection & conn, DataRequestInfo * data_request_info)
{
	//if (!conn.connected())
	//{
	//	DEF_LOG_ERROR("the connection is disconnected, error is <%d>\n", conn.errnum());
	//	data_request_info->db_load_success = false;
	//	return ;
	//}

	data_request_info->db_load_success = false;
	dbass::CacheAssistant * cache_ass = AssFactory::instance()->create(data_request_info->guid, data_request_info->data_request->entity_name());
	data_request_info->entity_msg = cache_ass;
	if (NULL != cache_ass)
	{
		mysqlpp::Query q = conn.query();
		if (cache_ass->load(q)) 
		{
			data_request_info->db_load_success = true;
		}
		else
		{
			DEF_LOG_ERROR("failed to load entity from db, guid is <%llu>, error is <%d>, err msg is <%s>\n", data_request_info->guid, cache_ass->last_error(), cache_ass->what().c_str());
		}
	}
	else
	{
		DEF_LOG_ERROR("failed to create CacheAssistant object in DbLoader::handleLoadRequest, guid is <%llu>\n", data_request_info->guid);
	}

	EntityHander::instance()->handleDbLoadReplay(data_request_info);
}

bool DbLoader::connect_to_database(::mysqlpp::Connection& conn)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_sql_connection_mutex, false);

	return this->connect_to_database(conn, CacheConfiguration::instance()->lookup(DATABASE_IP), 
		::boost::lexical_cast< uint16 >(CacheConfiguration::instance()->lookup(DATABASE_PORT)),
		CacheConfiguration::instance()->lookup(DATABASE_USER),
		CacheConfiguration::instance()->lookup(DATABASE_PASSWORD),
		CacheConfiguration::instance()->lookup(DATABASE_DB_NAME));
}

bool DbLoader::connect_to_database(::mysqlpp::Connection& conn, const ::std::string& host, uint16 port, const ::std::string& user, const ::std::string& password, const ::std::string& database_name)
{
	try {
		if (host.size() == 0 || user.size() == 0 || password.size() == 0)
			return false;
		if (conn.connected())
			return true;
		conn.connect(database_name.c_str(), 
			host.c_str(), 
			user.c_str(), 
			password.c_str(), 
			port);
		DEF_LOG_INFO("Connect to db `%s@%s`\n", database_name.c_str(), host.c_str());
		return true;
	}
	catch (const ::std::exception& er) {
		DEF_LOG_ERROR("Connect mysql db failed: %s\n", er.what());
	}

	return false;
}

void DbLoader::ping_db(::mysqlpp::Connection& conn)
{
	DEF_LOG_INFO("starto to ping mysql\n");
	try {
		::mysqlpp::Query q = conn.query("select * from ping");

		q.store();

		return ;
	}
	catch (const ::mysqlpp::BadQuery& er) {
		DEF_LOG_ERROR("Ping mysql failed: %s\n", er.what());
		return ;
	}
	catch (const ::mysqlpp::Exception& er) {
		DEF_LOG_ERROR("Ping mysql failed: %s\n", er.what());
		return ;
	}
}

};
};
