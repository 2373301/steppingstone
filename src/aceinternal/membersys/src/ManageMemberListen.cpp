
#include <ace/OS.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "ManageMemberListen.h"
#include "ManageConfig.h"

ManageMemberListen::ManageMemberListen()
: m_reactor(NULL)
{}

ManageMemberListen::~ManageMemberListen()
{}

int ManageMemberListen::svc()
{
#ifdef WIN32
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	m_reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(60000), true);
#endif

	ACE_INET_Addr addr(m_member_listen_addr.c_str());
	MemberAcceptor member_acceptor;
	if (member_acceptor.open(addr, m_reactor) == -1)
	{
		DEF_LOG_ERROR("failed to open the member address <%s>\n", m_member_listen_addr.c_str());
		return 1;
	}

	int reactor_result = 0;
	int last_error = 0;
	while (true)
	{
		reactor_result = m_reactor->run_reactor_event_loop();
#ifdef WIN32
		break;
#else
		last_error = ACE_OS::last_error();
		if ((EINTR == last_error) || (EAGAIN == last_error))
		{
			continue;
		}
		else
		{
			break;
		}
#endif
	}

	DEF_LOG_ERROR("leave member listen thread, reactor result is <%d>, last error is <%d>\n", reactor_result, last_error);

	return 0;
}

int ManageMemberListen::init()
{
	if (this->activate() == -1)
	{
		return -1;
	}

	m_member_listen_addr = ManageConfig::instance()->getMemberAddr();

	return 0;
}
