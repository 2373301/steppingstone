
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
	RequestImp(RequestType rt, uint64 entity_guid, MSG_TYPE * msg, const PoolCfgx pool_cfg, uint64 owner_g);

	~RequestImp();

	int handlePacket(Packet * packet);
	void setOwnerGuid(uint64 owner_g);

	PoolCfgx m_pool_cfg;
	STAT_PACKET_DEC();
};

class RequestListImp : RequestList
{
public:
	RequestListImp(const PoolCfgx pool_cfg);

	~RequestListImp();

public:
	typedef vector<Request *> RequestVec_t;

	const RequestVec_t & getRequestList();

	virtual void query(uint64 guid, uint64 owner_guid, MSG_TYPE * message) override;

	virtual void add(uint64 guid, MSG_TYPE * message, uint64 owner_guid) override;

	virtual void addWithFlush(uint64 guid, MSG_TYPE * message, uint64 owner_guid) override;

	virtual void update(uint64 guid, MSG_TYPE * message, uint64 owner_guid) override;

	virtual void load(uint64 guid, uint64 owner_guid, MSG_TYPE * message) override;

	virtual void loadOnce(uint64 guid, uint64 owner_guid, MSG_TYPE * message) override;

	virtual void flush(uint64 guid, MSG_TYPE * message, uint64 owner_guid) override;

	virtual void remove(uint64 guid, uint64 owner_guid) override;

	virtual bool is_complated() override;

	virtual bool is_success() override;

	virtual bool is_all_failed() override;

	virtual void setRequestID(uint64 request_id) override;

	virtual uint64 getRequestID() override;

	virtual int handlePacket(Packet * packet) override;

private:
	PoolCfgx m_pool_cfg;

	STAT_PACKET_DEC();
};

#endif