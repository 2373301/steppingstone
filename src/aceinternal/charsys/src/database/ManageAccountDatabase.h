
#ifndef MANAGE_ACCOUNT_DATABASE_HPP
#define MANAGE_ACCOUNT_DATABASE_HPP

#include <ace/Task.h>
#include "Singleton.h"
#include "Logger.h"
#include "SingletonInit.h"
#include "ManageConfig.h"
#include "typedef.h"
#include "coredef.h"
#include "DatabasePool.h"
#include "player.pb.h"
#include "GateSession.h"
#include "MakeGuid.h"

struct AccountInfo 
{
	AccountInfo()
	: packet_guid(0)
	, gate_session(NULL)
	{}

	typedef map<uint64, typed::entity::Player *> RoleMap_t;
	string account;
	uint64 packet_guid;
	GateSession * gate_session;
	RoleMap_t role_map;
};

class ManageAccountDatabase : public ACE_Task<ACE_NULL_SYNCH>, public SingletonInit
{
public:
	ManageAccountDatabase();
	~ManageAccountDatabase();
public:
	static ManageAccountDatabase * instance()
	{
		return Singleton<ManageAccountDatabase>::instance();
	}
public:
	virtual int svc (void);

public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	/// init
	virtual int init();

	/// fini, stop
	virtual int stop();

	/// wait for finish
	virtual int finit();
public:
	bool  getRoleList(AccountInfo & account_info, int & error_number);

	//bool  makeRole(const typed::protocol::cmsg_player_make & role_make, const string & account_id, uint64 & new_guid);

	bool  deleteRole(uint64 role_guid);

	bool makeRoleDisable(uint64 role_guid);

	bool isExistRoleName(const std::string & role_name, int & error_number);

	bool insertRoleName(const string & account_id, const std::string & role_name, uint64 & role_guid, int & error_number);

	bool getAccountRoleNumber(const string & account_id, int & account_number, int & error_number);

	bool updateRoleStatus(uint64 guid, int status);
protected:
	bool increaseGUIDIndex();

private:
	bool m_stop;

	bool m_wait;

	int m_guid_ggen;

	uint64 m_guid_index;

	//MakeGuid m_make_guid;

	DatabasePool m_database_pool;

	ACE_Thread_Mutex m_guid_mutex;
};

#endif