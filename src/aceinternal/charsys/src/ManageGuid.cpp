
#include "ManageGuid.h"
#include "ManageConfig.h"

int ManageGuid::init()
{
	int r = m_make_player_guid.init(ManageConfig::instance()->getGUIDAddr(), 1000);
	if (r != 0)
	{
		return r;
	}

	r = m_make_role_guid.init(ManageConfig::instance()->getGUIDAddr(), 1000);
	return r;
}

bool ManageGuid::get_guid(EntityType entity_type, uint64 & guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_make_guid_mutex, false);
	switch (entity_type)
	{
	case ET_PLAYER:
		return m_make_player_guid.get_guid(entity_type, guid);
		break;
	case ET_ROLE:
	default:
		return m_make_role_guid.get_guid(entity_type, guid);
		break;
	}

	return false;
}