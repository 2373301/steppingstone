
#ifndef POOL_ASSISTANT_HPP
#define POOL_ASSISTANT_HPP

#include <google/protobuf/message.h>
#include "typedef.h"
#include "coredef.h"
#include "Share_def.h"
#include "Packet.h"
#include "Pool.h"

class RequestImp : public Request 
{
public:
	RequestImp(RequestType rt, uint64 entity_guid, GOOGLE_MESSAGE_TYPE * msg, const PoolCfg pool_cfg, uint64 owner_g);

	~RequestImp();

	int handlePacket(Packet * packet);

	//bool extractEntity(const string & stream);

	void setOwnerGuid(uint64 owner_g);

	PoolCfg m_pool_cfg;
	//RequestType request_type;
	//uint64		guid;
	//bool		success;
	//bool		complated;
	//GOOGLE_MESSAGE_TYPE *	message;
	//string		stream_data;
	STAT_PACKET_DEC();
};

class RequestListImp : RequestList
{
public:
	RequestListImp(const PoolCfg pool_cfg);

	~RequestListImp();

public:
	typedef vector<Request *> RequestVec_t;

	const RequestVec_t & getRequestList();

	void query(uint64 guid, uint64 owner_guid);

	void add(uint64 guid, GOOGLE_MESSAGE_TYPE * message, uint64 owner_guid);

	void addWithFlush(uint64 guid, GOOGLE_MESSAGE_TYPE * message, uint64 owner_guid);

	void update(uint64 guid, GOOGLE_MESSAGE_TYPE * message, uint64 owner_guid);

	void load(uint64 guid, uint64 owner_guid);

	void loadOnce(uint64 guid, uint64 owner_guid);

	void flush(uint64 guid, GOOGLE_MESSAGE_TYPE * message, uint64 owner_guid);

	void remove(uint64 guid, uint64 owner_guid);

	bool is_complated();

	bool is_success();

	bool is_all_failed();

	virtual void setRequestID(uint64 request_id);

	virtual uint64 getRequestID();

	int handlePacket(Packet * packet);

private:
	//bool	m_complated;
	//int		m_complated_count;
	//RequestVec_t m_request_vec;
	PoolCfg m_pool_cfg;

	STAT_PACKET_DEC();
};

#endif