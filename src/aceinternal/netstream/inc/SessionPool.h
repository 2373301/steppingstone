
#ifndef SESSION_POOL_HPP
#define SESSION_POOL_HPP

#include <ace/Message_Block.h>
#include "typedef.h"
#include "netstream_def.h"

namespace netstream
{

typedef void * Session_t;
typedef vector<string> SessionAddrVec_t;

class NETSTREAM_EXOPRT ISessionPoolEvent
{
public:
	virtual void ISessionPoolEvent_newConnection(Session_t session, bool clientSide) = 0;

	virtual void ISessionPoolEvent_connectionClosed(Session_t session, int trigger_source) = 0;

	virtual void ISessionPoolEvent_handleInputStream(Session_t session, ACE_Message_Block & msg_block) = 0;
};

class NETSTREAM_EXOPRT ISessionPool
{
public:
	virtual ~ISessionPool(){}

	virtual int ISessionPool_init(int input_thr_no, int output_thr_no, ISessionPoolEvent * handle_session_event = NULL) = 0;

	virtual void ISessionPool_setBufSize(int input_buf_size, int output_buf_size) = 0;

	virtual Session_t ISessionPool_connect(const string & connect_addr) = 0;

	virtual bool ISessionPool_listen(const string & listen_addr) = 0;

	// send data
	virtual bool ISessionPool_asyncWrite(Session_t session, char * buffer, int buff_size) = 0;

	virtual void ISessionPool_stop() = 0;

	virtual void ISessionPool_finit() = 0;

	// Ö÷¶¯¹Ø±Õsession
	virtual void ISessionPool_removeSession(Session_t session) = 0;

};

}

#endif