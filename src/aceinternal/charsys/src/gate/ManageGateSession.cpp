
#include <ace/os_include/netinet/os_tcp.h>
#include <ace/SOCK_Connector.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "ManageGateSession.h"
#include "ManageConfig.h"
#include "GateSession.h"
#include "RouteGatePacket.h"

ManageGateSession::ManageGateSession()
: m_reactor(NULL)
, m_disconnect_gate(true)
, m_timer_id(0)
{

}

ManageGateSession::~ManageGateSession()
{

}

int ManageGateSession::init()
{
#ifdef WIN32
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	m_reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(100000), true);
#endif

	connectGate();

	m_disconnect_gate = false;

	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

int ManageGateSession::stop()
{
	return 0;
}

int ManageGateSession::finit()
{
	return 0;
}

int ManageGateSession::handle_timeout(const ACE_Time_Value &current_time, const void *act)
{
	if (m_disconnect_gate)
	{
		connectGate();
		m_disconnect_gate = false;
	}
	return 0;
}

int ManageGateSession::svc(void)
{
	m_reactor->owner(ACE_OS::thr_self());
	int reactor_result = 0;

	try
	{

		//注册一个timer，用于检查是否需要退出
		m_timer_id = m_reactor->schedule_timer(this, NULL, ACE_Time_Value(3, 0), ACE_Time_Value(3, 0));
		if (m_timer_id < 0)
		{
			DEF_LOG_ERROR("Failed to register timer in ManageClientSession::svc, the last error is <%d>", ACE_OS::last_error());
			return 1;
		}

		while (true)
		{
			reactor_result = m_reactor->run_reactor_event_loop();
		#ifdef WIN32
			break;
		#else
			if (ACE_OS::last_error() == EINTR)
			{
				continue;
			}
			else
			{
				break;
			}
		#endif
		}
	}
	catch (...)
	{
		DEF_LOG_ERROR("Raise unknown exception in ManageGateSession::svc, last error is <%d>\n", ACE_OS::last_error());
		return 2;
	}

	return 0;
}

void ManageGateSession::connectGate()
{
	const GateCfgVec_t & gate_cfg_vec = ManageConfig::instance()->getGateCfg();
	ACE_SOCK_Connector connector;
	ACE_INET_Addr addr;
	for (GateCfgVec_t::const_iterator it = gate_cfg_vec.begin(); it != gate_cfg_vec.end(); )
	{
		addr.set(it->port, it->ip.c_str());
		GateSession * gate_session = new GateSession();
		if (connector.connect(gate_session->peer(), addr) == -1)
		{
			DEF_LOG_ERROR("Failed to connector gate, ip is <%s>, port is <%d>, last error is <%d>\n", it->ip.c_str(), it->port, ACE_OS::last_error());
			ACE_OS::sleep(1);
			continue;
		}
		else
		{
			gate_session->setPacketHandler(RouteGatePacket::instance());
			gate_session->netConnected();

			gate_session->peer().enable(ACE_NONBLOCK);
			int flag = 1;
			gate_session->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));
			m_reactor->register_handler(gate_session, ACE_Event_Handler::READ_MASK);
			++it;
		}
	}
}

void ManageGateSession::gateDisconnect()
{
	m_disconnect_gate = true;
}