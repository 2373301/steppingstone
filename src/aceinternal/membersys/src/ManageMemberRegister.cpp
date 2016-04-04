
#include <ace/OS.h>
#include <ace/Time_Value.h>
#include "ManageMemberRegister.h"
#include "ManageConfig.h"
#include "ManageMemberSession.h"
#include "opcode.h"

ManageMemberRegister::ManageMemberRegister()
{
}

ManageMemberRegister::~ManageMemberRegister()
{
}

int ManageMemberRegister::init()
{
	if (this->activate(THR_JOINABLE, 10) == -1)
	{
		return -1;
	}

	return 0;
}

int ManageMemberRegister::svc()
{
	ACE_Time_Value sleep_time(0, 10000);


	MemberSession * member_session = NULL;

	while(true)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_member_session_que_mutex, 1);
			if (m_member_session_que.size() > 0)
			{
				member_session = m_member_session_que.front();
				m_member_session_que.pop();
			}
		}

		if (NULL == member_session)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		processMemberRegister(member_session);

		member_session = NULL;

	}

	return 0;
}

void ManageMemberRegister::handleMemberSession(MemberSession * member_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_member_session_que_mutex, );
	m_member_session_que.push(member_session);
}

void ManageMemberRegister::processMemberRegister(MemberSession * member_session)
{
	// todo
	ACE_Time_Value timeout_value(1000, 0);

	char buffer[100];
	memset(buffer, 0, 100);
	int buf_index = 0;

	int recv_result = 0;

	uint32 register_msg_len = sizeof(MemberHead) + sizeof(membersys::mmsg_member_register);

	while (true)
	{
		recv_result = member_session->peer().recv_n(buffer + buf_index, register_msg_len - buf_index, 0, &timeout_value);
		if (recv_result > 0)
		{
			buf_index += recv_result;
			if (buf_index == register_msg_len)
			{
				// 
				MemberHead member_head;
				memcpy(&member_head, buffer, sizeof(MemberHead));

				membersys::mmsg_member_register member_register;
				memcpy(&member_register, buffer + sizeof(MemberHead), register_msg_len);

				if ((700 == member_head.opcode) && checkMemberRegister(member_register))
				{
					ManageMemberSession::instance()->handleMemberSession(member_session);
				}
				else
				{
					MemberHead member_head;
					member_head.opcode = SMSG_MEMBER_REGISTER_FAILED;
					member_head.size = 0;

					member_session->peer().send_n(&member_head, sizeof(member_head), 0, &timeout_value);

					ACE_Time_Value sleep_time(0, 10 * 1000);
					ACE_OS::sleep(sleep_time);

					member_session->close();
					delete member_session;
				}
				member_session = NULL;
				memset(buffer, 0, 100);
				buf_index = 0;

				break;
			}
		}
		else if (recv_result == 0)
		{
			member_session->close();
			delete member_session;
			member_session = NULL;
			memset(buffer, 0, 100);
			buf_index = 0;
			break;
		}
		else
		{
			int last_error = ACE_OS::last_error();
//			if ((EAGAIN == last_error) || (EAGAIN == last_error))
			if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error) || (ETIME == last_error))
			{
				continue;
			}
			else
			{
				// bad recv register message
				member_session->close();
				delete member_session;
				member_session = NULL;
				memset(buffer, 0, 100);
				buf_index = 0;
				break;
			}
		}
	}
}

bool ManageMemberRegister::checkMemberRegister(membersys::mmsg_member_register & member_register)
{
	string str_key;
	str_key.resize(100);
	copySecurityKey((char *)str_key.data(), member_register.security_key);

	bool check_key = checkKey(str_key);
//	if (check_key && (ManageConfig::instance()->getLineId() == member_register.line_id))
	if (check_key)
	{
		return true;
	}
	
	return false;
}

bool ManageMemberRegister::checkKey(const string & security_key)
{
	if (strcmp(ManageConfig::instance()->getMemberSecurityKey().c_str(), security_key.c_str()) == 0)
	{
		return true;
	}

	return false;
}

