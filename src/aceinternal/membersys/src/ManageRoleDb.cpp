
#include "Logger.h"
#include "ManageRoleDb.h"
#include <boost/typeof/typeof.hpp>

ManageRoleDb::ManageRoleDb()
: m_account_connection(NULL)
, m_game_connection(NULL)
{

}
ManageRoleDb::~ManageRoleDb()
{

}

int ManageRoleDb::init(const DbConfig & account_db, const DbConfig & game_db)
{
	m_account_db_cfg = account_db;
	m_game_db_cfg = game_db;

	m_account_connection = connectDb(account_db);

	if (NULL == m_account_connection)
	{
		DEF_LOG_ERROR("failed to connect the account db, ip is <%s>, port is <%d>\n", account_db.ip.c_str(), account_db.port);
		return -1;
	}

	m_game_connection = connectDb(game_db);

	if (NULL == m_game_connection)
	{
		DEF_LOG_ERROR("failed to connect the game db, ip is <%s>, port is <%d>\n", game_db.ip.c_str(), game_db.port);
		return -1;
	}

	return 0;
}

bool ManageRoleDb::queryRoleInfo(const string & account, RoleInfoVec_t & role_info_vec)
{
	role_info_vec.clear();
	if (queryFromAccount(account, role_info_vec))
	{
		queryFromGame(account, role_info_vec);

		return true;
	}

	return false;
}

void ManageRoleDb::queryPlayerRegisterInfo(uint64 player_guid,
                                           typed::protocol::smsg_query_register_user_info & msg)
{   

    checkDbConnection(&m_game_connection, m_game_db_cfg);
    if ((NULL == m_game_connection) || (!m_game_connection->connected()))
    {
        return ;
    }

    mysqlpp::Query query = m_game_connection->query();

    query << "select account, name, level, last_login_time  from Player where guid =" << player_guid;

    mysqlpp::StoreQueryResult res = query.store();

    if (res)
    {
        for (int i = 0; i < res.num_rows(); ++i)
        {   
            BOOST_AUTO(sub, msg.add_list_info());
            sub->set_user_id(res[i][0]);
            sub->set_nick_name(res[i][1]);
            sub->set_level(res[i][2]);
            sub->set_last_login_time(res[i][3]);
        }
    }
    else
    {
        DEF_LOG_ERROR("failed to query from game, last error is <%d>, db error is <%d>\n", ACE_OS::last_error(), query.errnum());
    }

}

bool ManageRoleDb::queryRegisterUser(const string & date, Uint64Vec_t & guids)
{
    string start_date = date + "T00:00:00";
    string end_date = date + "T23:59:59";
    
    checkDbConnection(&m_account_connection, m_account_db_cfg);
    if ((NULL == m_account_connection) || (!m_account_connection->connected()))
    {
        return false;
    }

    mysqlpp::Query query = m_account_connection->query();

    query << "select guid from role where create_time >='" << start_date.c_str() << "'";
    query << " and create_time <='" << end_date.c_str() <<"'";

    mysqlpp::StoreQueryResult res = query.store();

    membersys::RoleInfo role_info;
    if (res)
    {
        for (int i = 0; i < res.num_rows(); ++i)
        {
            guids.push_back( res[i][0]);
        }

        return true;
    }


    DEF_LOG_ERROR("failed to query from account, last error is <%d>, db error is <%d>\n", ACE_OS::last_error(), query.errnum());
    return false;
}


void ManageRoleDb::pingDb()
{
	pingDb(m_account_connection);
	pingDb(m_game_connection);
}

void ManageRoleDb::pingDb(mysqlpp::Connection * conn)
{
	mysqlpp::Query query = conn->query();

	query << "select * from ping";

	mysqlpp::StoreQueryResult res = query.store();
}

mysqlpp::Connection * ManageRoleDb::connectDb(const DbConfig & db_config)
{
	mysqlpp::Connection * con = new mysqlpp::Connection(false);
	con->set_option(new mysqlpp::SetCharsetNameOption("utf8"));

	if (con->connect(db_config.db_name.c_str(), db_config.ip.c_str(), db_config.user_name.c_str(), db_config.password.c_str(), db_config.port))
	{
		return con;
	}
	else
	{
		return NULL;
	}
}

bool ManageRoleDb::queryFromAccount(const string & account, RoleInfoVec_t & role_info_vec)
{
	role_info_vec.clear();

	checkDbConnection(&m_account_connection, m_account_db_cfg);
	if ((NULL == m_account_connection) || (!m_account_connection->connected()))
	{
		return false;
	}

	mysqlpp::Query query = m_account_connection->query();

	query << "select guid,name from role where account='" << account.c_str() << "'";
	query << " and status=1";

	mysqlpp::StoreQueryResult res = query.store();

	membersys::RoleInfo role_info;
	if (res)
	{
		for (int i = 0; i < res.num_rows(); ++i)
		{
			string role_guid = res[i][0].c_str();
			copyRoleGuid(role_info.role_id, role_guid.c_str());
			
			string role_name = res[i][1].c_str();
			copyRoleName(role_info.role_name, role_name.c_str());
			role_info.role_level = 1;
			role_info_vec.push_back(role_info);
		}

		return true;
	}
	else
	{
		DEF_LOG_ERROR("failed to query from account, last error is <%d>, db error is <%d>\n", ACE_OS::last_error(), query.errnum());
		return false;
	}
}

void ManageRoleDb::queryFromGame(const string & account, RoleInfoVec_t & role_info_vec)
{
	checkDbConnection(&m_game_connection, m_game_db_cfg);
	if ((NULL == m_game_connection) || (!m_game_connection->connected()))
	{
		return ;
	}

	mysqlpp::Query query = m_game_connection->query();

	query << "select guid,level,line_no from Player where account='" << account.c_str() << "';\0";

	mysqlpp::StoreQueryResult res = query.store();

	if (res)
	{
		for (int i = 0; i < res.num_rows(); ++i)
		{
			string role_id = res[i][0].c_str();
			int role_level = atoi(res[i][1].c_str());
			int line_id = atoi(res[i][2].c_str());
			for (RoleInfoVec_t::iterator it = role_info_vec.begin(); it != role_info_vec.end(); ++it)
			{
				membersys::RoleInfo & role_info = *it;
				if (strcmp(role_id.c_str(), role_info.role_id) == 0)
				{
					role_info.role_level = role_level;
					role_info.line_id = line_id;
				}
			}
		}
	}
	else
	{
		DEF_LOG_ERROR("failed to query from game, last error is <%d>, db error is <%d>\n", ACE_OS::last_error(), query.errnum());
	}

}

void ManageRoleDb::checkDbConnection(mysqlpp::Connection * * conn, const DbConfig & db_config)
{
	mysqlpp::Connection * connection = *conn;
	if ((NULL == connection) || (!connection->connected()))
	{
		if (NULL != connection)
		{
			connection->disconnect();
			delete connection;
		}

		*conn = connectDb(db_config);
	}
}

