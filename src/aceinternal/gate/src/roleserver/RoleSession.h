
#ifndef ROLE_SESSION_HPP
#define ROLE_SESSION_HPP

#include <ace/SOCK_Stream.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include "NetStream.h"

class RoleSession;

typedef ACE_Acceptor<RoleSession, ACE_SOCK_ACCEPTOR> RoleAcceptor;

class RoleSession : public NetStream
{
public:
	RoleSession();
	~RoleSession();
protected:
	virtual int netConnected();

	virtual void netClosed(ACE_Reactor_Mask close_mask);
protected:
private:

};

#endif