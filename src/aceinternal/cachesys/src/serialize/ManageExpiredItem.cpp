
#include "ManageExpiredItem.h"

ManageExpiredItem::ManageExpiredItem()
{

}

ManageExpiredItem::~ManageExpiredItem()
{
	for (ExpiredItemMap_t::iterator it = m_lru_map.begin(); it != m_lru_map.end(); ++it)
	{
		ExpiredItem & expired_item = it->second;
		delete expired_item.item;
	}
}

void ManageExpiredItem::update(CacheCGUID guid, CacheAssistantx * item, uint64 last_modify_time)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_lru_map_mutex, );
	ExpiredItemMap_t::iterator it = m_lru_map.find(guid);
	if (it != m_lru_map.end())
	{
		m_lru_map.erase(it);
	}

	m_lru_map.insert(ExpiredItemMap_t::value_type(guid, ExpiredItem(last_modify_time, item, false)));
}

void ManageExpiredItem::updateToDelete(CacheCGUID guid)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_lru_map_mutex, );
	ExpiredItemMap_t::iterator it = m_lru_map.find(guid);
	if (it != m_lru_map.end())
	{
		it->second.in_use = false;
		it->second.is_delete = true;
		it->second.item = NULL;
	}
}

bool ManageExpiredItem::query(CacheCGUID guid, CacheAssistantx * * item, bool delete_after_query)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_lru_map_mutex, false);
	ExpiredItemMap_t::iterator it = m_lru_map.find(guid);
	if (it != m_lru_map.end())
	{
		if (!it->second.is_delete)
		{
			*item = it->second.item;
			if (delete_after_query)
			{
				it->second.in_use = true;
			}

			return true;		
		}
	}

	return false;
}

bool ManageExpiredItem::query(CacheCGUID guid, ::google::protobuf::Message * * entity_msg)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_lru_map_mutex, false);
	ExpiredItemMap_t::iterator it = m_lru_map.find(guid);
	if (it != m_lru_map.end())
	{
		if (!it->second.is_delete)
		{
			*entity_msg = it->second.item->data()->New();
			(*entity_msg)->CopyFrom(*it->second.item->data());

			return true;		
		}
	}

	return false;
}

bool ManageExpiredItem::remove(CacheCGUID guid, uint64 last_modify_time)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_lru_map_mutex, false);
	ExpiredItemMap_t::iterator it = m_lru_map.find(guid);
	if (it != m_lru_map.end())
	{
		if (it->second.in_use)
		{
			m_lru_map.erase(it);
			return true;
		}
		else if (last_modify_time == it->second.last_motify_time)
		{
			delete it->second.item;
			m_lru_map.erase(it);
			DEF_LOG_DEBUG("sucess to remove entity from cache for getting convert replay, guid is <%llu>\n", guid);
			return true;
		}
		else
		{
			DEF_LOG_DEBUG("do not remove entity from cache for get convert replay, as last motify is not equal, cache last modify time is <%llu>, convert last modify time is <%llu>, guid is <%llu>\n", it->second.last_motify_time, last_modify_time, guid);
			return true;
		}
	}
	else
	{
		DEF_LOG_ERROR("start to remove entity from cache for get convert replay, but did not find the entity, guid is <%llu>\n", guid);
	}

	return false;
}
