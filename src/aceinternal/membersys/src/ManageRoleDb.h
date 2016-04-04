
#ifndef MANAGE_ROLE_DB_HPP
#define MANAGE_ROLE_DB_HPP

#include "Singleton.h"
#include "MemberProtocol.h"
#include "ManageConfig.h"
#include <mysql++.h>
#include "msg_member.pb.h"
typedef vector<membersys::RoleInfo> RoleInfoVec_t;

class ManageRoleDb
{
public:
	ManageRoleDb();
	~ManageRoleDb();
public:
	static ManageRoleDb * instance()
	{
		return Singleton<ManageRoleDb>::instance();
	}
public:
	int init(const DbConfig & account_db, const DbConfig & game_db);

	bool queryRoleInfo(const string & account, RoleInfoVec_t & role_info_vec);
    bool queryRegisterUser(const string & date, Uint64Vec_t & guids);
    void queryPlayerRegisterInfo(uint64 player_guid, typed::protocol::smsg_query_register_user_info & msg);

	void pingDb();

	void pingDb(mysqlpp::Connection * conn);

protected:
	mysqlpp::Connection * connectDb(const DbConfig & db_config);

	void checkDbConnection(mysqlpp::Connection * * conn, const DbConfig & db_config);

	bool queryFromAccount(const string & account, RoleInfoVec_t & role_info_vec);

	void queryFromGame(const string & account, RoleInfoVec_t & role_info_vec);

private:
	mysqlpp::Connection * m_account_connection;

	mysqlpp::Connection * m_game_connection;

	DbConfig m_account_db_cfg;

	DbConfig m_game_db_cfg;
};

#endif
