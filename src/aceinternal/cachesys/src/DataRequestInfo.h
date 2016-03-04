
#ifndef DATA_REQUEST_INFO_HPP
#define DATA_REQUEST_INFO_HPP

#include "Packet.h"
#include "typedef.h"
#include "protocol/msg_cache.pb.h"
#include "ace/OS_NS_sys_time.h"
#include "dynamicparse.h"

namespace cached 
{
namespace service 
{

struct DataRequestInfo 
{
	DataRequestInfo()
		: opcode(0)
		, guid(0)
		, data_request(NULL)
		, entity_msg(NULL)
		, last_modify_time(0)
		, db_load_success(false)
		, packet_owner(NULL)
		, owner_guid(0)
	{
		ACE_OS::gettimeofday().to_usec(last_modify_time);
		STAT_PACKET_INC_NEW();
	}

	DataRequestInfo(int op_code, uint64 gv, protocol::db_data_request * drv, CacheAssistantx * emv, void * packet_o)
		: opcode(op_code)
		, guid(gv)
		, data_request(drv)
		, entity_msg(emv)
		, db_load_success(true)
		, packet_owner(packet_o)
		, owner_guid(data_request->owner_guid())
	{
		ACE_OS::gettimeofday().to_usec(last_modify_time);
		STAT_PACKET_INC_NEW();
	}

	~DataRequestInfo()
	{
		if (NULL != data_request)
		{
			delete data_request;
		}

		if (NULL != entity_msg)
		{
			delete entity_msg;
		}

		STAT_PACKET_INC_DEL();
	}

	int opcode;
	uint64 guid;
	protocol::db_data_request * data_request;
	CacheAssistantx * entity_msg;
	uint64 last_modify_time;
	bool	db_load_success;
	void *	packet_owner;
	uint64	owner_guid;

	STAT_PACKET_DEC();
};

};

};

#endif