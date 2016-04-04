
#ifndef MANAGE_MEMBER_SESSION_COOLER_HPP
#define MANAGE_MEMBER_SESSION_COOLER_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include "typedef.h"
#include "Singleton.h"
#include "MemberSession.h"
#include "Packet.h"
#include "MemberProtocol.h"

struct CoolerInfo 
{
	CoolerInfo()
		: member_session(NULL)
		, close_time(0)
	{}

	CoolerInfo(MemberSession * ms, uint64 curr_time)
		: member_session(ms)
		, close_time(curr_time)
	{}

	~CoolerInfo()
	{
		if (NULL != member_session)
		{
			delete member_session;
			member_session = NULL;
		}
	}

	MemberSession * member_session;
	uint64 close_time;
};

class ManageMemberSessionCooler : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageMemberSessionCooler();
	~ManageMemberSessionCooler();
public:
	static ManageMemberSessionCooler * instance()
	{
		return Singleton<ManageMemberSessionCooler>::instance();
	}
public:
	int svc();

	int init();

	void lostConnection(MemberSession * member_session);
protected:
	void cleanMemberSession(MemberSession * member_session);

private:
	typedef queue<CoolerInfo *> MemberSessionQue_t;

	typedef set<MemberSession *> MemberSessionSet_t;

	MemberSessionSet_t m_lost_connection_set;

	ACE_Thread_Mutex m_lost_connection_set_mutex;

	MemberSessionQue_t m_cleaned_connection_que;
};

#endif
