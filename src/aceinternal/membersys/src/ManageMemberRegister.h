
#ifndef MANAGE_MEMBER_REGISTER_HPP
#define MANAGE_MEMBER_REGISTER_HPP

#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/Acceptor.h>
#include <ace/Reactor.h>
#include "MemberSession.h"
#include "Singleton.h"
#include "MemberProtocol.h"

class ManageMemberRegister : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageMemberRegister();
	~ManageMemberRegister();
public:
	static ManageMemberRegister * instance()
	{
		return Singleton<ManageMemberRegister>::instance();
	}
public:
	int init();

	int svc();

	void handleMemberSession(MemberSession * member_session);
protected:
	bool checkMemberRegister(membersys::mmsg_member_register & member_register);

	void processMemberRegister(MemberSession * member_session);

	bool checkKey(const string & security_key);
private:
	typedef queue<MemberSession *>	MemberSessionQue_t;

	MemberSessionQue_t m_member_session_que;

	ACE_Thread_Mutex m_member_session_que_mutex;
};

#endif
