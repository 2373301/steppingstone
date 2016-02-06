
#ifndef SESSION_POOL_IMP_HPP
#define SESSION_POOL_IMP_HPP

#include "SessionPool.h"
#include "InputSessionPool.h"
#include "OutputSessionPool.h"
#include "typedef.h"
#include "CellSession.h"

namespace netcore
{

class SessionPoolImp : public SessionPool, public HandleInput, public HandleSessionEvent, ACE_Task<ACE_NULL_SYNCH>
{
public:
	SessionPoolImp();
	~SessionPoolImp();
public:
	virtual int svc();
public:
	virtual void input(Packet * packet);

public:
	virtual int init(int input_thr_no, int output_thr_no, HandleInput * handle_input = NULL, HandleSessionEvent * handle_session_event = NULL, HandleSessionRouter * handle_session_router = NULL);

	virtual bool connect(const SessionAddrVec_t & session_addr_vec);

	virtual bool listen(const string & listen_addr);

	virtual void setHandleInput(HandleInput * handle_input);

	virtual void setHandleSessionEvent(HandleSessionEvent * handle_event);

	virtual void setHandleSessionRouter(HandleSessionRouter * handle_session_router);

	virtual void output(Packet * packet);

	virtual void stop();

	virtual void finit();

	virtual void removeSession(Session * session);

	virtual void savePackStream();

public:
	void newConnection(Session * session);

	void connectionClosed(Session * session);

protected:
private:
	HandleInput * m_handle_input;

	HandleSessionEvent * m_handle_session_event;

	HandleSessionRouter * m_handle_session_router;

	ACE_Reactor * m_reactor;

	bool m_stop;

	bool m_wait;

	bool m_save_pack_stream;

	string m_listen_addr;

	InputSessionPool m_input_session_pool;

	OutputSessionPool m_output_session_pool;

	typedef define_unordered_map<CellSession *, CellSessionInfo> CellSessionInfoMap_t;

	CellSessionInfoMap_t m_cell_session_map;

	ACE_Thread_Mutex m_cell_session_map_mutex;

};


}

#endif