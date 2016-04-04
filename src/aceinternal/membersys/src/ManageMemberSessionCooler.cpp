
#include "ManageMemberSessionCooler.h"
#include "ManageMemberSession.h"

ManageMemberSessionCooler::ManageMemberSessionCooler()
{

}

ManageMemberSessionCooler::~ManageMemberSessionCooler()
{

}

int ManageMemberSessionCooler::svc()
{
	ACE_Time_Value sleep_time(0, 10 * 1000);
	MemberSessionSet_t lost_connection_set;
	while (true)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_lost_connection_set_mutex, 1);
			for (MemberSessionSet_t::iterator it = m_lost_connection_set.begin(); it != m_lost_connection_set.end(); ++it)
			{
				lost_connection_set.insert(*it);
			}
			m_lost_connection_set.clear();
		}

		uint64 curr_time = 0;
		ACE_OS::gettimeofday().to_usec(curr_time);

		{
			for (MemberSessionSet_t::iterator it = lost_connection_set.begin(); it != lost_connection_set.end(); ++it)
			{
				cleanMemberSession(*it);
				m_cleaned_connection_que.push(new CoolerInfo(*it, curr_time));
			}
			lost_connection_set.clear();
		}

		if (m_cleaned_connection_que.size() == 0)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		while (m_cleaned_connection_que.size() > 0)
		{
			CoolerInfo * coller_info = m_cleaned_connection_que.front();
			if ((curr_time - coller_info->close_time) >= 15)
			{
				m_cleaned_connection_que.pop();
				delete coller_info;
			}
			else
			{
				break;
			}
		}
	}
	return 0;
}

int ManageMemberSessionCooler::init()
{
	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

void ManageMemberSessionCooler::lostConnection(MemberSession * member_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_lost_connection_set_mutex, );
	m_lost_connection_set.insert(member_session);
}

void ManageMemberSessionCooler::cleanMemberSession(MemberSession * member_session)
{
	ManageMemberSession::instance()->removeMemberSession(member_session);
	member_session->close();
}
