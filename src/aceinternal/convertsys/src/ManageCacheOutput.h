
#ifndef MANAGE_CACHE_OUTPUT_HPP
#define MANAGE_CACHE_OUTPUT_HPP

#include <ace/Task.h>
#include "typedef.h"
#include "CacheSession.h"

class ManageCacheOutput 
	: public ACE_Task<ACE_NULL_SYNCH>
{
public:
	int init(int thread_no);

	int svc();
	void handleCache(CacheSession * cache_session);

private:
	deque<CacheSession *> m_cache_session_qeque;

	ACE_Thread_Mutex m_cache_session_qeque_mutex;
};

#endif