
#ifndef INPUT_SESSION_POOL_HPP
#define INPUT_SESSION_POOL_HPP

#include <ace/Task.h>
#include "typedef.h"
#include "CellSession.h"
#include "SessionPool.h"

namespace netstream
{

struct InputSessionThreadInfo 
{
	InputSessionThreadInfo()
		: reactor(NULL)
	{}

	~InputSessionThreadInfo()
	{
		if (NULL != reactor)
		{
			delete reactor;
		}
	}

	ACE_Reactor * reactor;
};

typedef vector<InputSessionThreadInfo *> InputSessionThreadInfoVec_t;

class InputSessionPool 
	: public ACE_Task<ACE_NULL_SYNCH>
{
public:
	InputSessionPool();
	~InputSessionPool();

	int init(int thread_no, SessionPool * session_pool);
	void stop();
	void finit();

	int svc();

	void handleSession(CellSession * cell_session);
	void removeSession(CellSession * cell_session);
	void registerSessionThreadInfo(InputSessionThreadInfo * stinfo);

private:
	int m_session_thread_info_index;
	bool m_stop;
	bool m_wait;
	bool m_actived;
	SessionPool * m_session_pool;
	InputSessionThreadInfoVec_t	m_session_thread_info;
	ACE_Thread_Mutex m_session_thread_info_mutex;

	typedef map<CellSession *, InputSessionThreadInfo *> CellSessionMap_t;
	CellSessionMap_t	m_cell_session_map;
};

}
#endif