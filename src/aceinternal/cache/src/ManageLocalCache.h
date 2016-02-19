
#ifndef MANAGE_LOCAL_CACHE_HPP
#define MANAGE_LOCAL_CACHE_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "typedef.h"
#include "cachehdr.h"
#include "LocalCache.h"
#include "Packet.h"

class ManageLocalCache : public ManageCache, public HandleInputStream, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	virtual int stop();

	virtual int finit();
public:
	virtual void cacheOutput(Packet * packet, uint64 map_id, uint64 request_id, uint64 owner_guid);

	//virtual void input(Packet * packet);
	virtual void input(Session * session, ACE_Message_Block & msg_block);
public:
	// ace svc
	virtual int svc(); 

protected:
	virtual int initing();

	bool getMapID(uint64 guid, bool erase, uint64 & map_id, uint64 & request_id);
protected:
private:
	typedef vector<ReqPackInfo> GUIDVect_t;

	LocalCache m_local_cache;

	GUIDVect_t m_guid_vec;

	ACE_Thread_Mutex m_guid_vec_mutex;

	PacketVec_t m_output_packet_vec;

	ACE_Thread_Mutex m_output_packet_vec_mutex;
};

#endif