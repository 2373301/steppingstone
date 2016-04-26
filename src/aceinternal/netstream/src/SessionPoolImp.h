
#ifndef SESSION_POOL_IMP_HPP
#define SESSION_POOL_IMP_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Reactor.h>
#include "SessionPool.h"
#include "InputSessionPool.h"
#include "OutputSessionPool.h"

namespace netstream
{

struct SessionStatus 
{
	SessionStatus()
	: ref_no(2)
	, closed_read(false)
	, closed_write(false)
	{}

	int ref_no;
	bool closed_read;
	bool closed_write;
};

class SessionPoolImp
	: public ISessionPool
	, public ISessionIn
	, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	SessionPoolImp();
	virtual ~SessionPoolImp();

	virtual int	 ISessionPool_init(int input_thr_no, int output_thr_no, ISessionPoolEvent * handle_session_event = NULL)  override;
	virtual void ISessionPool_setBufSize(int input_buf_size, int output_buf_size) override;
	virtual Session_t	ISessionPool_connect(const string & connect_addr) override;
	virtual bool ISessionPool_listen(const string & listen_addr) override;
	virtual bool ISessionPool_asyncWrite(Session_t session, char * buffer, int buff_size) override;
	virtual void ISessionPool_stop() override;
	virtual void ISessionPool_finit() override;
	virtual void ISessionPool_removeSession(Session_t session) override;

	// ManageNetEventNotify callback
	void onSessionOpenNotify(Session * session);
	void onSessionCloseNotify(Session * session, int trigger_source);

	virtual void ISessionIn_syncRead(Session * session, ACE_Message_Block & msg_block) override;

	virtual int svc() override; // ACE_Task

private:
	ISessionPoolEvent * m_handle_session_event;
	ACE_Reactor * m_reactor;

	// default 0, 1:sucess, 2:failed
	int m_listen_status;
	int m_socket_intput_buffer_size;
	int m_socket_output_buffer_size;
	string m_listen_addr;

	InputSessionPool m_input_session_pool;
	OutputSessionPool m_output_session_pool;

	typedef define_unordered_map<Session *, SessionStatus>	SessionInfoMap_t;
	SessionInfoMap_t	m_session_info_map;

	ACE_Thread_Mutex	m_session_info_map_mutex;
};

}

#endif