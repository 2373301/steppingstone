
#ifndef SESSION_POOL_HPP
#define SESSION_POOL_HPP

#include "typedef.h"
#include "Packet.h"
#include "Session.h"
#include "netcore_def.h"

namespace netcore
{

typedef vector<string> SessionAddrVec_t;

class NETCORE_EXOPRT HandleSessionEvent
{
public:
	virtual void newConnection(Session * session) = 0;

	virtual void connectionClosed(Session * session) = 0;
};

class NETCORE_EXOPRT HandleSessionRouter
{
public:
	virtual void addRoute(Packet * packet) = 0;

	virtual void removeRoute(uint64 guid) = 0;

	virtual Session * getSession(Packet * packet) = 0;
};

class NETCORE_EXOPRT SessionPool
{
public:
	virtual ~SessionPool()
	{}

	virtual int init(int input_thr_no, int output_thr_no, HandleInput * handle_input = NULL,
					HandleSessionEvent * handle_session_event = NULL, HandleSessionRouter * handle_session_router = NULL) = 0;

	virtual bool connect(const SessionAddrVec_t & session_addr_vec) = 0;

	virtual bool listen(const string & listen_addr) = 0;

	virtual void setHandleInput(HandleInput * handle_input) = 0;

	virtual void setHandleSessionEvent(HandleSessionEvent * handle_event) = 0;

	virtual void setHandleSessionRouter(HandleSessionRouter * handle_session_router) = 0;

	virtual void output(Packet * packet) = 0;

	virtual void stop() = 0;

	virtual void finit() = 0;

	virtual void removeSession(Session * session) = 0;

	virtual void savePackStream() = 0;
protected:
private:
};


} // namespace netcore

#endif