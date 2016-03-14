
#ifndef POOL_HPP
#define POOL_HPP

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <google/protobuf/message.h>
#include "typedef.h"
#include "coredef.h"
#include "pool_def.h"
#include "Packet.h"
#include "logger.h"


// request operator

#define DECLARE_REQUEST_LIST(pool)							RequestList * request_list_ins = pool->createRequestList()

#define QUERY_REQUEST(guid, owner_guid)						request_list_ins->query(guid, owner_guid)

#define ADD_REQUEST(guid, message, owner_guid)				request_list_ins->add(guid, message, owner_guid)

#define ADD_WITH_FLUSH_REQUEST(guid, message, owner_guid)	request_list_ins->addWithFlush(guid, message, owner_guid)

#define UPDATE_REQUEST(guid, message, owner_guid)			request_list_ins->update(guid, message, owner_guid)

#define LOAD_REQUEST(guid, owner_guid)						request_list_ins->load(guid, owner_guid)

#define LOAD_ONCE_REQUEST(guid, owner_guid, msg)			request_list_ins->loadOnce(guid, owner_guid, msg)

#define FLUSH_REQUEST(guid, message, owner_guid)			request_list_ins->flush(guid, message, owner_guid)

#define REMOVE_REQUEST(guid, owner_guid)					request_list_ins->remove(guid, owner_guid)

#define REQUEST_IS_EMPTY()									request_list_ins->getRequestList().size() == 0

#define DELETE_REQUEST()									delete request_list_ins

// pool operator

#define GET_ENTITY_FROM_POOL(pool, ENTITY_TYPE, guid, entity_ins)	\
	ENTITY_TYPE * entity_ins = NULL;	\
	{	\
		MSG_TYPE * msg = pool->get(guid);	\
		if (NULL != msg)	\
		{	\
			entity_ins = dynamic_cast<ENTITY_TYPE *>(msg);	\
		}	\
	}	\
	if (NULL == entity_ins)	\
	{	\
	}	

#define GET_PLAYER_FROM_POOL(pool, guid, entity_ins)			GET_ENTITY_FROM_POOL(pool, typed::entity::Player, guid, entity_ins)

#define GET_ROLE_FROM_POOL(pool, guid, entity_ins)				GET_ENTITY_FROM_POOL(pool, typed::entity::Role, guid, entity_ins)

//////////////////////////////////////////////////////////////////////////

#define POOL_OP_ADD(pool, guid, message, add_to_db, woner_guid)		pool->add(guid, message, add_to_db, woner_guid)

#define POOL_OP_REMOVE(pool, guid, remove_from_db)					pool->remove(guid, remove_from_db)

#define POOL_OP_UPDATE(pool, guid)									pool->update(guid)

#define POOL_OP_COMMIT(pool, call_back)								pool->commit(request_list_ins, call_back)

#define POOL_OP_REMOVE_PENDING_UPDATE(pool)							pool->removePendingUpdate(request_list_ins)

#define POOL_OP_CLEAR_FLUSH_STATE(pool, guid)						pool->clearFlushState(guid);


enum RequestType
{
	RT_ADD		= 1,
	RT_LOAD,
	RT_QUERY,
	RT_UPDATE,
	RT_FLUSH,
	RT_DELETE,
	RT_LOADONCE,
	RT_ADD_WITH_FLUSH,
};

class POOL_EXOPRT Request 
{
public:
	virtual ~Request()
	{
		// deleted by caller
	}

	virtual int handlePacket(Packet * packet) = 0;

	RequestType request_type;
	uint64		guid;
	bool		success;
	bool		complated;
	uint64		owner_guid;
	MSG_TYPE *	message;
	std::string	name;
};

POOL_EXOPRT bool extractEntity(uint64 guid, const string & stream, MSG_TYPE * * entity_msg);

class POOL_EXOPRT RequestList 
{
public:
	virtual ~RequestList()
	{
		for (RequestVec_t::iterator it = m_request_vec.begin(); it != m_request_vec.end(); ++it)
		{
			delete *it;
		}
	}

public:
	typedef vector<Request *> RequestVec_t;

	virtual const RequestVec_t & getRequestList() = 0;

	virtual void query(uint64 guid, uint64 owner_guid, MSG_TYPE * message) = 0; //+

	virtual void add(uint64 guid, MSG_TYPE * message, uint64 owner_guid) = 0;

	virtual void addWithFlush(uint64 guid, MSG_TYPE * message, uint64 owner_guid) = 0;

	virtual void update(uint64 guid, MSG_TYPE * message, uint64 owner_guid) = 0;

	virtual void loadOnce(uint64 guid, uint64 owner_guid, MSG_TYPE * message) = 0; //+

	virtual void load(uint64 guid, uint64 owner_guid, MSG_TYPE * message) = 0; //+

	virtual void flush(uint64 guid, MSG_TYPE * message, uint64 owner_guid) = 0;

	virtual void remove(uint64 guid, uint64 owner_guid) = 0;

	virtual bool is_complated() = 0;

	virtual bool is_success() = 0;

	virtual bool is_all_failed() = 0;

	virtual void setRequestID(uint64 request_id) = 0;

	virtual uint64 getRequestID() = 0;

	virtual int handlePacket(Packet * packet) = 0;

protected:
	bool	m_complated;
	int		m_complated_count;
	uint64	m_request_id;
	RequestVec_t m_request_vec;
};

class Scene;
struct PoolCfgx
{
	std::function<void(Packet * packet, uint64 map_id, uint64 request_id, uint64 owner_guid)> handle_output;
	uint64				map_id = 0;
	uint32				line_id = 0;
	Logger*				logger = NULL;
	Scene*				scene = NULL;
};

struct EntityInfo;
class POOL_EXOPRT Pool
{
public:
	virtual ~Pool(){}

	virtual int init(const PoolCfgx pool_cfg) = 0;
	virtual void cacheInput(Packet * packet, uint64 map_id, uint64 request_id) = 0; // cin


	virtual MSG_TYPE * get(uint64 guid) = 0;
	virtual EntityInfo * getEntityInfo(uint64 guid) = 0;
	virtual MSG_TYPE * getWhileFlush(uint64 guid) = 0;	// get entity and set flush state
	virtual void clearFlushState(uint64 guid) = 0;

	
	virtual void beginTransaction() = 0;
	virtual void endtransaction() = 0;

	// add_to_db add to pool and database
	virtual bool add(uint64 guid, MSG_TYPE * message, bool add_to_db, uint64 owner_guid) = 0;

	/// remove_from_db remove from pool and database
	virtual bool remove(uint64 guid, bool remove_from_db, bool remove_entity_msg = true) = 0;

	// update guid to database
	virtual bool update(uint64 guid) = 0;

	virtual void onlyRemoveEntity(uint64 guid, uint64 owner_guid) = 0;

	// for frame calling
	virtual bool update() = 0;

	virtual bool isLastUpdateFinish() = 0;

	virtual int updateEntityNumber() = 0;

	virtual void clearUpdateEntity(uint64 guid_owner, Uint64Set_t & insert_entity_set, Uint64Set_t & remove_entity_set) = 0;

	typedef boost::function<void(RequestList *)>	RequestCallBack;
	// commit data to cache system, action : update, query, load, flush
	virtual bool commit(RequestList * request_list, RequestCallBack call_back) = 0;

	virtual RequestList * createRequestList() = 0;

	virtual int getEntityNumber() = 0;

protected:
	PoolCfgx m_pool_cfg;
};

Pool *	createPool();

#endif
