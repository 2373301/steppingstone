
#include "PlayerFunction.h"
#include "ManageContainerImp.h"

ManageContainerImp::ManageContainerImp()
: m_container_container(NULL)
{
}

ManageContainerImp::~ManageContainerImp()
{

}

bool ManageContainerImp::isRegister(EntityType et_type)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, false);

	uint64 guid = MAKE_ENTITY_GUID(ET_CONTAINER, 1, et_type);

	for (int i = 0; i < m_container_container->con_guid_size(); ++i)
	{
		if (m_container_container->con_guid(i) == guid)
		{
			return true;
		}
	}

	return false;
}

bool ManageContainerImp::registerEntityType(EntityType et_type)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, false);

	uint64 guid = MAKE_ENTITY_GUID(ET_CONTAINER, 1, et_type);

	typed::entity::Container * con = new typed::entity::Container();
	con->set_guid(guid);
	m_container_container->add_con_guid(guid);

	m_added_msg_set.insert(con);
	m_updated_guid_set.insert(m_container_container->guid());

	m_container_map.insert(std::make_pair(et_type, con));

	return true;
}

bool ManageContainerImp::addGuid(EntityType et_type, uint64 entity_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, false);

	ContainerMap_t::iterator it = m_container_map.find(et_type);
	if (it != m_container_map.end())
	{
		typed::entity::Container * con = it->second;
		con->add_ent_guid(entity_guid);
		m_updated_guid_set.insert(con->guid());

		return true;
	}
	else
	{
		return false;
	}
}

bool ManageContainerImp::removeGuid(EntityType et_type, uint64 entity_guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, false);

	bool result = false;
	ContainerMap_t::iterator it = m_container_map.find(et_type);
	if (it != m_container_map.end())
	{

		typed::entity::Container * con = it->second;
		result = removeFirstInRepeatedField(entity_guid, *con->mutable_ent_guid());
		m_updated_guid_set.insert(con->guid());
	}

	return result;
}

// call this function only once when startup
ManageContainer::MsgSet_t ManageContainerImp::getEntityMessages(EntityType et_type)
{
	ManageContainer::MsgSet_t result;

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, result);

	ContainerEntityMap_t::iterator it = m_container_entity_map.find(et_type);
	if (it != m_container_entity_map.end())
	{
		vector<GOOGLE_MESSAGE_TYPE *> & con_entity_vec = it->second;
		for (int i = 0; i < con_entity_vec.size(); ++i)
		{
			result.insert(con_entity_vec[i]);
		}

		con_entity_vec.clear();
	}

	return result;
}

Uint64Vec_t ManageContainerImp::getContainerGuids(EntityType et_type)
{
	Uint64Vec_t result;

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, result);

	ContainerMap_t::iterator it = m_container_map.find(et_type);
	if (it != m_container_map.end())
	{
		typed::entity::Container * con = it->second;
		for (int i = 0; i < con->ent_guid_size(); ++i)
		{
			result.push_back(con->ent_guid(i));
		}
	}
	
	return result;
}
bool ManageContainerImp::isAddedGuid(EntityType et_type, uint64 guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, false);

	bool result = false;

	ContainerMap_t::iterator it = m_container_map.find(et_type);
	if (it != m_container_map.end())
	{
		typed::entity::Container * con = it->second;
		for (int i = 0; i < con->ent_guid_size(); ++i)
		{
			if (con->ent_guid(i) == guid)
			{
				result = true;
				break;
			}
		}
	}

	return result;
}

bool ManageContainerImp::addContainer(EntityType et_type, GOOGLE_MESSAGE_TYPE * ent_msg)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, false);

	if (ET_CONTAINER_CONTAINER == et_type)
	{
		m_container_container = (typed::entity::ContainerContainer *)ent_msg;
	}

	ContainerMap_t::iterator it = m_container_map.find(et_type);
	if (it != m_container_map.end())
	{
		// error
		return false;
	}
	
	m_container_map.insert(std::make_pair(et_type, (typed::entity::Container *)ent_msg));

	return true;
}

bool ManageContainerImp::addContainerEntity(EntityType et_type, GOOGLE_MESSAGE_TYPE * ent_msg)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, false);

	m_container_entity_map[et_type].push_back(ent_msg);

	return true;
}

Uint64Set_t ManageContainerImp::getUpdateGuids()
{
	Uint64Set_t result;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, result);

	result = m_updated_guid_set;
	m_updated_guid_set.clear();
	return result;
}

ManageContainer::MsgSet_t ManageContainerImp::getAddedContainer()
{
	ManageContainer::MsgSet_t result;

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_container_mutex, result);

	result = m_added_msg_set;	
	m_added_msg_set.clear();
	return result;
}
