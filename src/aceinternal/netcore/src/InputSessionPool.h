
#ifndef INPUT_SESSION_POOL_HPP
#define INPUT_SESSION_POOL_HPP

#include <ace/Task.h>
#include "typedef.h"
#include "CellSession.h"
#include "SessionPool.h"

namespace netcore
{

// �̶߳�Ӧ�� reactor
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
	int init(int thread_no, SessionPool * session_pool, HandleSessionOpenClosed * handle_session_event);
	void stop();
	void finit();

	int svc();
	void handleSession(CellSession * cell_session);
	void registerSessionThreadInfo(InputSessionThreadInfo * stinfo);
public:
	virtual void sessionOpen(Session * session);
	virtual void sessionClosed(Session * session);

private:
	int m_session_thread_info_index;

	bool m_stop; // ��Ҫ��ֹͣ

	bool m_wait; // svc ���˳�

	bool m_actived; // svc ������

	SessionPool * m_session_pool;

	HandleSessionOpenClosed * m_handle_session_event;

	InputSessionThreadInfoVec_t	m_session_thread_info; // �������е��е� reactor thread

	ACE_Thread_Mutex m_session_thread_info_mutex;

	typedef map<CellSession *, InputSessionThreadInfo *> CellSessionMap_t;

	CellSessionMap_t	m_cell_session_map; // session ���ڵ� reactor
};

}
#endif