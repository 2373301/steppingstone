
#ifndef REQUEST_GUID_HPP
#define REQUEST_GUID_HPP

#include <ace/Message_Block.h>
#include <ace/SOCK_Stream.h>
#include <ace/Task.h>
#include "typedef.h"
#include "coredef.h"
#include "Packet.h"

struct GuidInfo 
{
	GuidInfo()
	: seed(0)
	, guid_no(0)
	, guid_rest(0)
	, index(0)
	{}
	GuidInfo(uint32 sv, uint32 gnv, uint32 grv, uint64 iv)
	: seed(sv)
	, guid_no(gnv)
	, guid_rest(grv)
	, index(iv)
	{}

	uint32 seed;
	uint32 guid_no;
	uint32 guid_rest;
	uint64 index;
};

class MakeGuid : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	MakeGuid();
	~MakeGuid();
public:
	bool get_guid(EntityType entity_type, uint64 & guid);

public:
	int init(const string & guid_srv_addr, uint32 once_request_no = 5000);

	int stop();

	int svc();

protected:
	bool getGuidInfo(GuidInfo & guid_info);

	int connectSrv();

	bool parsePack(ACE_Message_Block & message_block);

	void parsePack(Packet * packet);

private:
	typedef deque<GuidInfo> GuidInfoDeque_t;

	uint32 m_request_no;

	GuidInfo m_guid_info;

	string m_guid_server_addr;

	ACE_SOCK_Stream m_sock_stream;

	GuidInfoDeque_t	m_guid_info_que;

	ACE_Thread_Mutex m_guid_info_que_mutex;

	bool m_is_stop;
};
#endif