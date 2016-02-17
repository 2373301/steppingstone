
#ifndef SESSION_POOL_IMP_HPP
#define SESSION_POOL_IMP_HPP

#include "SessionPool.h"
#include "InputSessionPool.h"
#include "OutputSessionPool.h"
#include "typedef.h"
#include "CellSession.h"

namespace netcore
{

class SessionPoolImp 
	:public SessionPool
	,public HandleInput
	,public HandleSessionOpenClosed
	,ACE_Task<ACE_NULL_SYNCH>
{
public:
	SessionPoolImp();
	~SessionPoolImp();

	virtual int svc() override;
	
	virtual int init(int input_thr_no
					,int output_thr_no
					,HandleInput * handle_input = NULL
					,HandleSessionOpenClosed * handle_session_event = NULL
					,HandleSessionRouterAddRemove * handle_session_router = NULL) override;
	virtual void stop() override;
	virtual void finit() override;

	virtual bool connect(const SessionAddrVec_t & session_addr_vec) override;
	virtual bool listen(const string & listen_addr) override; // 使自己成为actor

	virtual void setHandleInput(HandleInput * handle_input) override; // 设置别的 handler代替自己来处理输入
	virtual void setHandleSessionEvent(HandleSessionOpenClosed * handle_event) override;
	virtual void setHandleSessionRouter(HandleSessionRouterAddRemove * handle_session_router) override;

	virtual void input(Packet * packet) override;
	virtual void output(Packet * packet) override;

	virtual void removeSession(Session * session) override;
	virtual void savePackStream() override;

	void sessionOpen(Session * session);
	void sessionClosed(Session * session);

private:
	HandleInput * m_handle_input;
	HandleSessionOpenClosed * m_handle_session_event;
	HandleSessionRouterAddRemove * m_handle_session_router;

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