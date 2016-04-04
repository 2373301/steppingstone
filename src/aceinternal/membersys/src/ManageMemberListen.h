
#ifndef MANAGE_MEMBER_LISTEN_HPP
#define MANAGE_MEMBER_LISTEN_HPP

#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/Acceptor.h>
#include <ace/Reactor.h>
#include "MemberSession.h"
#include "Singleton.h"

typedef ACE_Acceptor<MemberSession, ACE_SOCK_ACCEPTOR> MemberAcceptor;

class ManageMemberListen : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageMemberListen();
	~ManageMemberListen();
public:
	static ManageMemberListen * instance()
	{
		return Singleton<ManageMemberListen>::instance();
	}
public:
	int svc();

	int init();

protected:
private:
	ACE_Reactor * m_reactor;

	string m_member_listen_addr;
};

#endif
