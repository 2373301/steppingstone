
#ifndef POOL_IMP_HPP
#define POOL_IMP_HPP

#include "typedef.h"
#include "Pool.h"

enum EntityOpType
{
	EOT_NULL	= 0,
	EOT_ADD		= 0x1,
	EOT_REMOVE	= 0x2,
	EOT_UPDATE	= 0x4,
	EOT_QUERY	= 0x8,
	EOT_FLUSH	= 0x10,
	EOT_ADD_REMOVE = 0x11,	// not used
};

struct EntityInfo 
{
	EntityInfo()
		: op_type(EOT_NULL)
		, is_flushing(false)
		, owner_guid(0)
		, message(NULL)
		, is_remove_from_memory(false)
	{
		STAT_PACKET_INC_NEW();
	}

	EntityInfo(EntityOpType op_type, GOOGLE_MESSAGE_TYPE * msg, uint64 owner_g)
		: op_type(op_type)
		, is_flushing(false)
		, owner_guid(owner_g)
		, message(msg)
		, is_remove_from_memory(false)
	{
		STAT_PACKET_INC_NEW();
	}

	~EntityInfo()
	{
		if (NULL != message)
		{
			delete message;
		}
		STAT_PACKET_INC_DEL();
	}

	uint32	op_type;
	bool	is_flushing;		/// is_flushing is true while flush entity to db
	uint64	owner_guid;			// player guid
	GOOGLE_MESSAGE_TYPE * message;
	bool	is_remove_from_memory;

	STAT_PACKET_DEC();
};

struct DbEntityInfo 
{
	DbEntityInfo()
	: op_type(EOT_NULL)
	, guid(0)
	, already_request(false)
	, transaction_index(0)
	, transaction_num(0)
	, transaction_id(0)
	, owner_guid(0)
	{
		STAT_PACKET_INC_NEW();
	}

	DbEntityInfo(EntityOpType op_type, uint64 gd, uint32 t_index, uint32 t_id, uint64  owner_g)
	: op_type(op_type)
	, guid(gd)
	, already_request(false)
	, transaction_index(t_index)
	, transaction_num(0)
	, transaction_id(t_id)
	, owner_guid(owner_g)
	{
		STAT_PACKET_INC_NEW();
	}

	~DbEntityInfo()
	{
		STAT_PACKET_INC_DEL();
	}

	uint32	op_type;
	uint64	guid;
	bool	already_request;
	uint32	transaction_index;
	uint32  transaction_num;
	uint32  transaction_id;
	uint64  owner_guid;

	STAT_PACKET_DEC();
};

struct RequestInfo 
{
	RequestInfo(RequestList * rl, Pool::RequestCallBack cb)
		: request_list(rl)
		, call_back(cb)
	{
		STAT_PACKET_INC_NEW();
	}

	~RequestInfo()
	{
		delete request_list;
		STAT_PACKET_INC_DEL();
	}

	RequestList * request_list;
	Pool::RequestCallBack call_back;

	STAT_PACKET_DEC();
};

class PoolImp : public Pool
{
public:
	PoolImp();
	~PoolImp();
public:
	virtual void cacheInput(Packet * packet, uint64 map_id, uint64 request_id);

public:
	virtual int init(const PoolCfg pool_cfg);

	virtual GOOGLE_MESSAGE_TYPE * get(uint64 guid);

	virtual EntityInfo * getEntityInfo(uint64 guid);

	virtual GOOGLE_MESSAGE_TYPE * getWhileFlush(uint64 guid);

	virtual void clearFlushState(uint64 guid);

	virtual void beginTransaction();

	virtual void endtransaction();

	virtual bool add(uint64 guid, GOOGLE_MESSAGE_TYPE * message, bool add_to_db, uint64 owner_guid);

	virtual bool remove(uint64 guid, bool remove_from_db, bool remove_entity_msg = true);

	virtual bool update(uint64 guid);

	virtual void onlyRemoveEntity(uint64 guid, uint64 owner_guid);

	virtual bool update();

	virtual bool isLastUpdateFinish();

	virtual int updateEntityNumber();

	virtual void clearUpdateEntity(uint64 guid_owner, Uint64Set_t & insert_entity_set, Uint64Set_t & remove_entity_set);

	virtual bool commit(RequestList * request_list, RequestCallBack call_back);

	virtual Request * createRequset(RequestType rt, uint64 entity_guid, GOOGLE_MESSAGE_TYPE * msg);

	virtual RequestList * createRequestList();

	virtual int getEntityNumber();

	virtual void playerIsOnline(uint64 player_guid);

	virtual void playerIsOffline(uint64 player_guid);

	virtual bool isPlayerOnlineInThisMap(uint64 player_guid);

	virtual const PlayerGuidSet_t & getOnlinePlayer();

	//virtual void getInsertGuidAndClear(uint64 owner_guid, define_unordered_set<uint64> & insert_guid);

	void updateToDbReplay();

protected:
	typedef define_unordered_map<uint64, EntityInfo *>	EntityMap_t;

	typedef define_unordered_map<uint64, DbEntityInfo *> DbEntityMap_t;

	typedef define_unordered_map<uint64, DbEntityMap_t > OwnerDbEntityMap_t;		// owner_guid, guid, DbEntityInfo

	typedef define_unordered_map<uint64, uint64> GuidOwnerGuidMap_t;

	typedef vector<RequestInfo *>		RequestInfoVec_t;

	void handlePacket(Packet * packet, uint64 request_id);

	void complateRequest(RequestInfo * request_info);

	bool updateDbEntity(DbEntityMap_t & db_entity_map);

	void updateToDb(uint64 owner_guid, uint64 guid, EntityOpType eot_type);

	static void updateToDbCallback(RequestList * request_list, PoolImp * pool);

	void removeDbInfo(uint64 owner_guid, uint64 guid, bool success);

	void doRemoveBack(uint64 guid);

	bool isUpdating(uint64 owner_guid, uint64 guid);

	void checkShouldRemovedEntity(uint64 guid);

	void directUpdateToDb(GOOGLE_MESSAGE_TYPE * message, uint64 guid, uint64 owner_guid);
//	int handle
private:

	uint64 m_request_id;

	uint32 m_transaction_index;

	uint32 m_transaction_id;

	int32 m_update_to_db_num;

	int32 m_do_not_get_the_replay_time;

	EntityMap_t m_entity_map;

	OwnerDbEntityMap_t m_db_entity_map;

	OwnerDbEntityMap_t m_updating_db_entity_map;

	RequestInfoVec_t	m_request_info_vec;

	PlayerGuidSet_t		m_online_player;

	GuidOwnerGuidMap_t	m_guid_ownerguid_map; // guid <-> owner guid
};

#endif
