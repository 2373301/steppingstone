
#ifndef MANAGE_EXPIRED_ITEM_HPP
#define MANAGE_EXPIRED_ITEM_HPP

#include <ace/Thread_Mutex.h>
//#include "serializer.h"
#include "cache_auto_assistant.h"
#include "Singleton.h"
#include "typedef.h"
#include "cached.h"

//using namespace cached::service;

struct ExpiredItem 
{
	ExpiredItem(uint64 lmt, dbass::CacheAssistant * im, bool in_u)
	: last_motify_time(lmt)
	, in_use(in_u)
	, is_delete(false)
	, item(im)
	{}

	uint64				last_motify_time;
	bool				in_use;
	bool				is_delete;
	dbass::CacheAssistant * item;
};

class ManageExpiredItem
{
public:
	ManageExpiredItem();
	~ManageExpiredItem();
public:
	static ManageExpiredItem * instance()
	{
		return Singleton<ManageExpiredItem>::instance();
	}
public:
	void update(CacheCGUID guid, dbass::CacheAssistant * item, uint64 last_modify_time);

	void updateToDelete(CacheCGUID guid);

	bool query(CacheCGUID guid, dbass::CacheAssistant * * item, bool delete_after_query);

	bool query(CacheCGUID guid, ::google::protobuf::Message * * entity_msg);

	bool remove(CacheCGUID guid, uint64 last_modify_time);
protected:
private:
	typedef define_unordered_map<uint64, ExpiredItem>	ExpiredItemMap_t;

	ExpiredItemMap_t m_lru_map;

	ACE_Thread_Mutex m_lru_map_mutex;
};

#endif