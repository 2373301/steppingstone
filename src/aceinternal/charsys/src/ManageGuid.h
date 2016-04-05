
#ifndef MANAGE_GUID_HPP
#define MANAGE_GUID_HPP

#include <ace/Thread_Mutex.h>
#include "Singleton.h"
#include "SingletonInit.h"
#include "typedef.h"
#include "coredef.h"
#include "MakeGuid.h"

class ManageGuid : public SingletonInit
{
public:
	static ManageGuid * instance()
	{
		return Singleton<ManageGuid>::instance();
	}
public:
	virtual int init();

	bool get_guid(EntityType entity_type, uint64 & guid);

protected:
private:
	MakeGuid m_make_player_guid;

	MakeGuid m_make_role_guid;

	ACE_Thread_Mutex m_make_guid_mutex;
};
#endif