
#ifndef MANAGE_REMOTE_CACHE_HPP
#define MANAGE_REMOTE_CACHE_HPP

#include <ace/Task.h>
#include <ace/Reactor.h>
#include "typedef.h"
#include "Cache.h"
#include "cachehdr.h"
#include "ManageRemoteCacheOutput.h"

class ManageRemoteCache 
					: public ManageCache
					, public IStreamIn
					, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageRemoteCache();
	~ManageRemoteCache();

public:
	int svc();
public:
	//virtual void input(Packet * packet);
	virtual void input(Session * session, ACE_Message_Block & msg_block);

	virtual void cacheOutput(Packet * packet, uint64 map_id, uint64 request_id, uint64 owner_guid);
public:
	virtual int stop();

	virtual int finit();

protected:
	virtual int initing();

	int connectRemoteCache();

	void registerReactor(ACE_Reactor * reactor);

	bool getMapID(uint64 guid, bool erase, uint64 & map_id, uint64 & request_id);
protected:
private:
	typedef vector<ReqPackInfo> GUIDVec_t;

	typedef vector<RemoteCache *>	 CacheVec_t;

	typedef vector<ACE_Reactor *> ReactorVec_t;

	ACE_Thread_Mutex m_output_packet_vec_mutex;

	PacketVec_t m_output_packet_vec;

	ACE_Thread_Mutex m_guid_vec_mutex;

	GUIDVec_t m_guid_vec;

	ACE_Thread_Mutex m_cache_vec_mutex;

	CacheVec_t	m_cache_vec;

	ACE_Thread_Mutex m_reactor_vec_mutex;

	ReactorVec_t m_reactor_vec;

	ManageRemoteCacheOutput m_manage_remote_cache_output;

	bool m_is_stop;
};
#endif