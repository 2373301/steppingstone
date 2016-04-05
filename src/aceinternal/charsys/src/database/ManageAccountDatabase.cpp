
#include <fstream>
#include <ace/OS.h>
#include <ace/Date_Time.h>
#include "ManageAccountDatabase.h"
#include "ManageGuid.h"

#define MAKE_GUID(type, ggen, id) (((uint64(type) << 56) & 0xFF00000000000000) | ((uint64(ggen) << 48) & 0x00FF000000000000) | (uint64(id) & 0x0000FFFFFFFFFFFF))

ManageAccountDatabase::ManageAccountDatabase()
: m_stop(false)
, m_wait(false)
{

}

ManageAccountDatabase::~ManageAccountDatabase()
{

}

int ManageAccountDatabase::svc (void)
{
	while (!m_wait)
	{
		ACE_OS::sleep(sMsgCfg->getAccountSQLCfg().ping_MYSQL_interval);
		m_database_pool.pingMYSQL();
	}
	DEF_LOG_ERROR("Exit thread ManageAccountDatabase::svc, last error is <%d>\n", ACE_OS::last_error());
	return 0;
}

int ManageAccountDatabase::init()
{
	m_guid_ggen = sMsgCfg->getGUIDCfg().m_ggen_value;
	m_guid_index = sMsgCfg->getGUIDCfg().m_index_value;

	//m_make_guid.init(ManageConfig::instance()->getGUIDAddr(), 10000);

	if (!m_database_pool.init(sMsgCfg->getAccountSQLCfg()))
	{
		return -1;
	}

	if (this->activate() == -1)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int ManageAccountDatabase::stop()
{
	m_stop = true;
	return 0;
}

int ManageAccountDatabase::finit()
{
	m_wait = true;
	super::wait();
	return 0;
}

bool ManageAccountDatabase::getRoleList(AccountInfo & account_info, int & error_number)
{
	bool result = false;
	mysqlpp::Connection * con = m_database_pool.getConnection();
	if (NULL != con)
	{
		try
		{
			mysqlpp::Query query = con->query();
			char str_buf[50] = {0};
			query << "select guid from role where account='" << account_info.account.c_str() << "'\0";
			query << " and status=1";
			mysqlpp::StoreQueryResult res = query.store();
			if (res)
			{
				for (int i = 0; i < res.num_rows(); ++i)
				{
					account_info.role_map[boost::lexical_cast<uint64>(res[i][0].c_str())] = NULL;
				}
				result = true;
			}
			else
			{
				// system error
				DEF_LOG_ERROR("failed to get role list, account is <%s>\n", account_info.account.c_str());
				//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
			}
		}
		catch (...)
		{
			DEF_LOG_ERROR("Raise unknown exception in ManageAccountDatabase::getRoleList, last error is <%d>\n", ACE_OS::last_error());
			//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
		}
		m_database_pool.returnConnection(con);
	}
	else
	{
		DEF_LOG_ERROR("Failed to get connection, point is NULL.\n");
	}
	return result;
}

bool ManageAccountDatabase::deleteRole(uint64 role_guid)
{
	bool result = false;
	mysqlpp::Connection * con = m_database_pool.getConnection();
	if (NULL != con)
	{
		try
		{
			ACE_Date_Time curr_time;
			string str_time;
			str_time.resize(50, '\0');
			sprintf((char *)str_time.data(), "%.04d-%.02d-%.02dT%.02d:%.02d:%.02d", curr_time.year(), curr_time.month(), curr_time.day(), curr_time.hour(), curr_time.minute(), curr_time.second());

			mysqlpp::Query query = con->query();
			char str_buf[50] = {0};
			query << "update role set status=2, delete_time='" << str_time.c_str() << "' where guid=" 
				<< boost::lexical_cast<string>(role_guid).c_str();
			mysqlpp::SimpleResult res = query.execute();
			if (res)
			{
				result = true;
				DEF_LOG_INFO("delete role <%s>\n", boost::lexical_cast<string>(role_guid).c_str());
			}
			else
			{
				DEF_LOG_ERROR("Failed to delete role from database, role name is <%s>, info is <%s>\n", boost::lexical_cast<string>(role_guid).c_str(), res.info());
			}
		}
		catch (...)
		{
			DEF_LOG_ERROR("Raise unknown exception in ManageAccountDatabase::deleteRole, last error is <%d>\n", ACE_OS::last_error());
		}
		m_database_pool.returnConnection(con);
	}
	return result;
}

bool ManageAccountDatabase::makeRoleDisable(uint64 role_guid)
{
	return false;
}

bool ManageAccountDatabase::isExistRoleName(const std::string & role_name, int & error_number)
{
	bool result = true;
	mysqlpp::Connection * con = m_database_pool.getConnection();
	if (NULL != con)
	{
		try
		{
			mysqlpp::Query query = con->query();
			char str_buf[50] = {0};
			query << "select guid from role where name='" << role_name.c_str() << "'\0";
			mysqlpp::StoreQueryResult res = query.store();
			if (res && (res.num_rows() == 0))
			{
				result = false;
			}
			else
			{
				if (res)
				{
					DEF_LOG_ERROR("The role name exists in database, name is <%s>\n", role_name.c_str());
					//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_NAME_EXIST;
					result = true;
				}
				else
				{
					// system error
					DEF_LOG_ERROR("failed to request the role name in database, role name is <%s>\n", role_name.c_str());
					//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
					result = true;
				}
			}
		}
		catch (...)
		{
			DEF_LOG_ERROR("Raise unknown exception in ManageAccountDatabase::isExistRoleName, last error is <%d>\n", ACE_OS::last_error());
			//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
		}
		m_database_pool.returnConnection(con);
	}
	else
	{
		DEF_LOG_ERROR("Failed to get connection, point is NULL.\n");
		//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
	}
	return result;
}

bool ManageAccountDatabase::insertRoleName(const string & account_id, const std::string & role_name, uint64 & role_guid, int & error_number)
{
	if (!increaseGUIDIndex())
	{
		return false;
	}

	bool result = true;
	mysqlpp::Connection * con = m_database_pool.getConnection();
	if (NULL != con)
	{
		try
		{
			ACE_Date_Time curr_time;
			string str_time;
			str_time.resize(50, '\0');
			sprintf((char *)str_time.data(), "%.04d-%.02d-%.02dT%.02d:%.02d:%.02d", curr_time.year(), curr_time.month(), curr_time.day(), curr_time.hour(), curr_time.minute(), curr_time.second());
			mysqlpp::Query query = con->query();
			//role_guid = MAKE_GUID(et_player, m_guid_ggen, m_guid_index - 1);
			//m_make_guid.get_guid(ET_PLAYER, role_guid);

			if (!ManageGuid::instance()->get_guid(ET_PLAYER, role_guid))
			{
				// error 
				DEF_LOG_ERROR("failed to make guid while make player, player name is <%s>\n", role_name.c_str());
				return false;
			}

			char str_buf[50] = {0};
			query << "insert into role(guid, account, name, status, create_time) values(";
			query << boost::lexical_cast<string>(role_guid).c_str() << ",";
			query << "'" << account_id.c_str() << "',";
			query << "'" << role_name.c_str() << "',";
			query << "0, ";
			query << "'" << str_time << "'";
			query << ")\0";
			mysqlpp::SimpleResult res = query.execute();
			if (res)
			{
				result = true;
			}
			else
			{
				// system error
				DEF_LOG_ERROR("failed to insert the role name in database, role name is <%s>, info is <%s>\n", role_name.c_str(), res.info());
				//error_number = typed::protocol::smsg_error_ecode_EFAILED_MAKE_ROLE;
				result = false;
			}
		}
		catch (...)
		{
			DEF_LOG_ERROR("Raise unknown exception in ManageAccountDatabase::isExistRoleName, last error is <%d>\n", ACE_OS::last_error());
			//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
		}
		m_database_pool.returnConnection(con);
	}
	else
	{
		DEF_LOG_ERROR("Failed to get connection, point is NULL.\n");
		//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
	}
	return result;
}

bool ManageAccountDatabase::getAccountRoleNumber(const string & account_id, int & account_number, int & error_number)
{
	bool result = false;
	mysqlpp::Connection * con = m_database_pool.getConnection();
	if (NULL != con)
	{
		try
		{
			mysqlpp::Query query = con->query();
			char str_buf[50] = {0};
			query << "select count(*) as number from role where account='" << account_id << "'";
			query << " and status=1";
			mysqlpp::StoreQueryResult res = query.store();
			if (res)
			{
				if (res.num_rows() == 1)
				{
					account_number = atoi(res[0][0].c_str());
				}
				result = true;
			}
			else
			{
				DEF_LOG_ERROR("Failed to query account role info, the account id is <%s>, error is <%s>\n", account_id.c_str(), query.error());
				//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
			}
		}
		catch (...)
		{
			DEF_LOG_ERROR("Raise unknown exception in ManageDatabase::getRoleList, last error is <%d>\n", ACE_OS::last_error());
			//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
		}
		m_database_pool.returnConnection(con);
	}
	else
	{
		DEF_LOG_ERROR("Failed to get connection, point is NULL.\n");
		//error_number = typed::protocol::smsg_error_ecode_EFAILED_ROLE_INTERNAL_ERROR;
	}
	return result;
}

bool ManageAccountDatabase::updateRoleStatus(uint64 guid, int status)
{
	bool result = false;
	mysqlpp::Connection * con = m_database_pool.getConnection();
	if (NULL != con)
	{
		try
		{
			mysqlpp::Query query = con->query();
			char str_buf[50] = {0};
			query << "update role set status=1 where guid=" 
				<< boost::lexical_cast<string>(guid).c_str();
			mysqlpp::SimpleResult res = query.execute();
			if (res)
			{
				result = true;
				DEF_LOG_INFO("update role <%s> to 1\n", boost::lexical_cast<string>(guid).c_str());
			}
			else
			{
				DEF_LOG_ERROR("Failed to update role status to 1 from database, role name is <%s>, info is <%s>\n", boost::lexical_cast<string>(guid).c_str(), res.info());
			}
		}
		catch (...)
		{
			DEF_LOG_ERROR("Raise unknown exception in ManageAccountDatabase::updateRoleStatus, last error is <%d>\n", ACE_OS::last_error());
		}
		m_database_pool.returnConnection(con);
	}
	return result;
}

bool ManageAccountDatabase::increaseGUIDIndex()
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_guid_mutex, false);
	bool result = true;
	++m_guid_index;
	//std::fstream fs;
	//fs.open("guid_index.txt", ios_base::trunc | ios_base::out);
	//if (fs.is_open())
	//{
	//	string buf = boost::lexical_cast<std::string>(m_guid_index);
	//	fs << buf;
	//	fs.close();
	//}

	return result;
}