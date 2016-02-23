
#ifndef MANAGE_ROLE_GS_STATE_HPP
#define MANAGE_ROLE_GS_STATE_HPP

#include <map>
#include <vector>
#include <ace/Task.h>
#include "typedef.h"
#include "Singleton.h"

enum RoleGsState
{
	RGS_ENTER_GAME				= 1,
	RGS_ENTER_GAME_SUCCESS		= 2,
	RGS_LEAVE_GAME				= 3,
	RGS_LEAVE_GAME_SUCCESS		= 4,
};

struct RoleGsStateInfo
{
	RoleGsStateInfo()
	: role_gs_state(RGS_ENTER_GAME)
	, map_id(0)
	, role_guid(0)
	, role_offline(false)
	{}

	RoleGsState role_gs_state;
	uint64 map_id;
	uint64 role_guid;
	bool role_offline;
};

class ManageRoleGsState
{
public:
	ManageRoleGsState();
	~ManageRoleGsState();
public:
	static ManageRoleGsState * instance()
	{
		return Singleton<ManageRoleGsState>::instance();
	}
public:
	typedef define_unordered_map<uint64, RoleGsStateInfo *>	RoleGsStateInfoMap_t;

	void enterGame(uint64 guid, uint64 map_id);

	bool canEnterGame(uint64 guid);

	void enterGameSuccess(uint64 guid);

	void leaveGame(uint64 guid, uint64 map_id);

	bool canLeaveGame(uint64 guid);

	void leaveGameSuccess(uint64 guid);

	void jumpMapSuccess(uint64 guid, uint64 map_id);

	void roleLogoff(uint64 guid);

	void cleanUnnormalPlayer(uint64 guid);

	void cleanAllPlayerStatus();

protected:
	void sendLogoffMsg(uint64 guid, uint64 map_id);

	void doLogoff(RoleGsStateInfo * role_gs_state);

private:

	RoleGsStateInfoMap_t m_role_gs_state_map;

	ACE_Thread_Mutex m_role_gs_state_map_mutex;
};

#endif