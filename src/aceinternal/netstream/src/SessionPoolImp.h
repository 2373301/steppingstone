
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

class SessionPoolImp : public SessionPool, public HandleInputStream, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	SessionPoolImp();

	virtual ~SessionPoolImp();
public:
	int svc();

public:
	virtual void input(Session * session, ACE_Message_Block & msg_block);

public:
	virtual int init(int input_thr_no, int output_thr_no, HandleSessionEvent * handle_session_event = NULL);

	virtual void setSocketBufferSize(int input_buf_size, int output_buf_size);

	virtual bool connect(const SessionAddrVec_t & session_addr_vec);

	virtual bool listen(const string & listen_addr);

	virtual void setHandleSessionEvent(HandleSessionEvent * handle_event);

	virtual bool handleOutputStream(Session_t session, char * buffer, int buff_size);

	virtual void stop();

	virtual void finit();

	virtual void removeSession(Session_t session);

	virtual void handleNewConnection(Session * session);

	virtual void connectionClosed(Session * session, int trigger_source);

protected:
private:
	HandleSessionEvent * m_handle_session_event;

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