
#ifndef SESSION_POOL_HPP
#define SESSION_POOL_HPP

#include "typedef.h"
#include "Packet.h"
#include "Sessionx.h"
#include "netcore_def.h"

namespace netcore
{

typedef vector<string> SessionAddrVec_t;

class NETCORE_EXOPRT HandleSessionOpenClosed
{
public:
	virtual void sessionOpen(Sessionx * session) = 0;

	virtual void sessionClosed(Sessionx * session) = 0;
};

class NETCORE_EXOPRT HandleSessionRouterAddRemove
{
public:
	virtual void sessionRouterAdd(Packet * packet) = 0;

	virtual void sessionRouterRemove(uint64 guid) = 0;

	virtual Sessionx * getSession(Packet * packet) = 0;
};

class NETCORE_EXOPRT SessionPoolx
	 :public HandleOutputx
{
public:
	virtual ~SessionPoolx(){}

	virtual int init(int input_thr_no
					,int output_thr_no
					,HandleInputx * handle_input = NULL
					,HandleSessionOpenClosed * handle_session_event = NULL
					,HandleSessionRouterAddRemove * handle_session_router = NULL) = 0;

	virtual bool connect(const SessionAddrVec_t & session_addr_vec) = 0;

	virtual bool listen(const string & listen_addr) = 0;

	virtual void setHandleInput(HandleInputx * handle_input) = 0;

	virtual void setHandleSessionEvent(HandleSessionOpenClosed * handle_event) = 0;

	virtual void setHandleSessionRouter(HandleSessionRouterAddRemove * handle_session_router) = 0;

	virtual void stop() = 0;

	virtual void finit() = 0;

	virtual void removeSession(Sessionx * session) = 0;

	virtual void savePackStream() = 0;
};


} // namespace netcore

#endif