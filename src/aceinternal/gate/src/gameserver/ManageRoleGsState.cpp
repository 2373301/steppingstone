
#include "opcode.h"
#include "PacketStream.h"
#include "GateLogger.h"
#include "ManageRoleGsState.h"
#include "RouteClientPackage.h"
#include "RouteGSPacket.h"

#define GET_ROLE_GS_STATE_INFO(result)		\
	RoleGsStateInfo * role_gs_state = NULL;		\
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_role_gs_state_map_mutex, result)		\
	RoleGsStateInfoMap_t::iterator it = m_role_gs_state_map.find(guid);		\
	if (it != m_role_gs_state_map.end())		\
	{	\
		role_gs_state = it->second;		\
	}

ManageRoleGsState::ManageRoleGsState()
{

}

ManageRoleGsState::~ManageRoleGsState()
{

}

#define MAKE_MAP_GUID(line_no, repduplicate_index, template_id)			MAKE_GUID(0xC, 15, (uint64)0 << 40 | repduplicate_index << 24 | template_id)

void ManageRoleGsState::enterGame(uint64 guid, uint64 map_id)
{
	GET_ROLE_GS_STATE_INFO( );

	if (NULL == role_gs_state)
	{
		RoleGsStateInfo * role_gsi = new RoleGsStateInfo();
		role_gsi->role_gs_state = RGS_ENTER_GAME;
		role_gsi->role_guid = guid;
		role_gsi->map_id = map_id;
		m_role_gs_state_map.insert(std::make_pair(guid, role_gsi));
	}
	else
	{
		GATE_LOG_INFO("player enter game, but it can not enter, guid is <%llu>, state is <%d>, map id is <%d>\n", guid, role_gs_state->role_gs_state, role_gs_state->map_id);
		role_gs_state->role_offline = true;
		doLogoff(role_gs_state);
	}
}

bool ManageRoleGsState::canEnterGame(uint64 guid)
{
	GET_ROLE_GS_STATE_INFO(false);

	if (NULL == role_gs_state)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ManageRoleGsState::enterGameSuccess(uint64 guid)
{
	bool role_should_logoff = false;
	uint64 map_id = 0;

	{
		GET_ROLE_GS_STATE_INFO();

		if (NULL != role_gs_state)
		{
			map_id = role_gs_state->map_id;

			if (RGS_ENTER_GAME == role_gs_state->role_gs_state)
			{
				role_gs_state->role_gs_state = RGS_ENTER_GAME_SUCCESS;
				if (role_gs_state->role_offline)
				{
					role_should_logoff = true;
				}
			}
			else
			{
				GATE_LOG_ERROR("role enter game success, wrong state, guid is <%llu>, state is <%d>\n", guid, role_gs_state->role_gs_state);
			}
		}
	}

	if (role_should_logoff)
	{
		sendLogoffMsg(guid, map_id);
	}
}

void ManageRoleGsState::leaveGame(uint64 guid, uint64 map_id)
{
	GET_ROLE_GS_STATE_INFO();

	if (NULL != role_gs_state)
	{
		//role_gs_state->map_id = map_id;

		switch (role_gs_state->role_gs_state)
		{
		case RGS_ENTER_GAME:
			role_gs_state->role_offline = true;
			break;
		case RGS_ENTER_GAME_SUCCESS:
			role_gs_state->role_gs_state = RGS_LEAVE_GAME;
			break;
		case RGS_LEAVE_GAME:
			break;
		case RGS_LEAVE_GAME_SUCCESS:
			break;
		}
	}
	else
	{
		
	}
}

bool ManageRoleGsState::canLeaveGame(uint64 guid)
{
	bool result = false;

	GET_ROLE_GS_STATE_INFO(false);

	if (NULL != role_gs_state)
	{
		switch (role_gs_state->role_gs_state)
		{
		case RGS_ENTER_GAME:
		case RGS_ENTER_GAME_SUCCESS:
			result = true;
			break;
		case RGS_LEAVE_GAME:
			break;
		case RGS_LEAVE_GAME_SUCCESS:
			break;
		}
	}
	else
	{

	}

	return result;
}

void ManageRoleGsState::leaveGameSuccess(uint64 guid)
{
	{
		GET_ROLE_GS_STATE_INFO();
		if (NULL != role_gs_state)
		{
			delete it->second;
			m_role_gs_state_map.erase(it);
		}
	}

	RouteClientPackage::instance()->roleLogoffSuccess(guid);
}

void ManageRoleGsState::jumpMapSuccess(uint64 guid, uint64 map_id)
{
	GET_ROLE_GS_STATE_INFO();

	if (NULL != role_gs_state)
	{
		role_gs_state->map_id = map_id;
	}
}

void ManageRoleGsState::roleLogoff(uint64 guid)
{
	GET_ROLE_GS_STATE_INFO();

	if (NULL != role_gs_state)
	{
		role_gs_state->role_offline = true;
	}
}

void ManageRoleGsState::cleanUnnormalPlayer(uint64 guid)
{
	GET_ROLE_GS_STATE_INFO();

	if (it != m_role_gs_state_map.end())
	{
		delete it->second;
		m_role_gs_state_map.erase(it);
	}
}

void ManageRoleGsState::cleanAllPlayerStatus()
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_role_gs_state_map_mutex, );
	for (RoleGsStateInfoMap_t::iterator it = m_role_gs_state_map.begin(); it != m_role_gs_state_map.end(); ++it)
	{
		delete it->second;
	}

	m_role_gs_state_map.clear();
}

void ManageRoleGsState::sendLogoffMsg(uint64 guid, uint64 map_id)
{
	PacketStream * ps = new PacketStream(CMSG_PLAYER_LEAVE_MAP, guid, "");
	RouteGSPacket::instance()->handleClientPackage(ps, map_id);
}

void ManageRoleGsState::doLogoff(RoleGsStateInfo * role_gs_state)
{
	if (!role_gs_state->role_offline)
	{
		return ;
	}

	switch (role_gs_state->role_gs_state)
	{
	case RGS_ENTER_GAME:
		// do nothing
		break;
	case RGS_ENTER_GAME_SUCCESS:
		// logoff
		sendLogoffMsg(role_gs_state->role_guid, role_gs_state->map_id);
		break;
	case RGS_LEAVE_GAME:
		// do nothing
		break;
	case RGS_LEAVE_GAME_SUCCESS:
		// do nothing
		break;
	}
}