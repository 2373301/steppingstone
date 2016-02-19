
#ifndef INPUT_SESSION_POOL_HPP
#define INPUT_SESSION_POOL_HPP

#include <ace/Task.h>
#include "typedef.h"
#include "CellSessionx.h"
#include "SessionPoolx.h"

namespace netcore
{

// 线程对应的 reactor
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


class InputSessionPool : public HandleSessionOpenClosed, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	InputSessionPool();
	~InputSessionPool();
public:
	int init(int thread_no, SessionPoolx * session_pool, HandleSessionOpenClosed * handle_session_event);
	void stop();
	void finit();

	int svc();
	void handleSession(CellSessionx * cell_session);
	void registerSessionThreadInfo(InputSessionThreadInfo * stinfo);
public:
	virtual void sessionOpen(Sessionx * session);
	virtual void sessionClosed(Sessionx * session);

private:
	int m_session_thread_info_index;

	bool m_stop; // 被要求停止

	bool m_wait; // svc 已退出

	bool m_actived; // svc 已运行

	SessionPoolx * m_session_pool;

	HandleSessionOpenClosed * m_handle_session_event;

	InputSessionThreadInfoVec_t	m_session_thread_info; // 现在运行当中的 reactor thread

	ACE_Thread_Mutex m_session_thread_info_mutex;

	typedef map<CellSessionx *, InputSessionThreadInfo *> CellSessionMap_t;

	CellSessionMap_t	m_cell_session_map; // session 所在的 reactor
};

}
#endif