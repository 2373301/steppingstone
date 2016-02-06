
#ifndef INPUT_SESSION_POOL_HPP
#define INPUT_SESSION_POOL_HPP

#include <ace/Task.h>
#include "typedef.h"
#include "CellSession.h"
#include "SessionPool.h"

namespace netcore
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


class InputSessionPool : public HandleSessionEvent, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	InputSessionPool();
	~InputSessionPool();
public:
	int init(int thread_no, SessionPool * session_pool, HandleSessionEvent * handle_session_event);

	void stop();

	void finit();
public:
	int svc();

	void handleSession(CellSession * cell_session);

	void registerSessionThreadInfo(InputSessionThreadInfo * stinfo);
public:
public:
	virtual void newConnection(Session * session);

	virtual void connectionClosed(Session * session);

protected:
private:
	int m_session_thread_info_index;

	bool m_stop;

	bool m_wait;

	bool m_actived;

	SessionPool * m_session_pool;

	HandleSessionEvent * m_handle_session_event;

	InputSessionThreadInfoVec_t	m_session_thread_info;

	ACE_Thread_Mutex m_session_thread_info_mutex;

	typedef map<CellSession *, InputSessionThreadInfo *> CellSessionMap_t;

	CellSessionMap_t	m_cell_session_map;
};

}
#endif