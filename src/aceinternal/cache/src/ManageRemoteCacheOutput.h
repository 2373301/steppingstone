
#ifndef MANAGE_REMOTE_CACHE_OUTPUT_HPP
#define MANAGE_REMOTE_CACHE_OUTPUT_HPP

#include <ace/Task.h>
#include "typedef.h"
#include "RemoteCache.h"

class ManageRemoteCacheOutput : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageRemoteCacheOutput();
	~ManageRemoteCacheOutput();
public:
	int init(int thread_no);

	int stop();

public:
	int svc();

public:
	void handleRemoteCache(RemoteCache * remote_cache);

private:
	deque<RemoteCache *> m_remote_cache_qeque;

	ACE_Thread_Mutex m_remote_cache_qeque_mutex;

	bool m_is_stop;
};
#endif