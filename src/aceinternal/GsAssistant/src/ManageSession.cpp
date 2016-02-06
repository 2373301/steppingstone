
#include "Logger.h"
#include "coredef.h"
#include "opcode.h"
#include "SessionPoolFactory.h"
#include "ManageSession.h"

ManageSession::ManageSession()
: m_session_pool(NULL)
, m_session(NULL)
, m_check_gs_type(CGT_BASE)
, m_recv_report_msg(false)
, m_is_success(false)
, m_is_finish(false)
{
}

ManageSession::~ManageSession()
{
	if (NULL != m_session_pool)
	{
		delete m_session_pool;
	}
}

void ManageSession::input(Packet * packet)
{
	if (packet->opcode() == IMSG_GS_STARTUP_REPORT)
	{
		handleGsStartupReport(packet);
	}
	else if (packet->opcode() == IMSG_GS_SHUTDOWN_REPORT)
	{
		handleGsShutdownReport(packet);
	}
	else
	{
		DEF_LOG_ERROR("get unknown opcode <%d>", packet->opcode());
	}

	delete packet;
}

// handle session event
void ManageSession::newConnection(Session * session)
{
	if (NULL != m_session)
	{
		DEF_LOG_ERROR("get new session but the session pointer is not NULL\n");
	}

	m_session = session;
}

void ManageSession::connectionClosed(Session * session)
{
	m_session = NULL;
}

// handle session router
void ManageSession::addRoute(Packet * packet)
{

}

void ManageSession::removeRoute(uint64 guid)
{

}

Session * ManageSession::getSession(Packet * packet)
{
	return m_session;
}

int ManageSession::init(int monitor_type, char * port)
{
	CheckGsType check_type = (CheckGsType)monitor_type;
	if ((CGT_STARTUP != check_type) && (CGT_SHUTDOWN != check_type))
	{
		DEF_LOG_ERROR("failed to get unknow monitor type <%d>\n", monitor_type);
		return -1;
	}

	m_check_gs_type = check_type;

	m_listen_port = port;

	m_session_pool = netcore::SessionPoolFactory::createSessionPool();
	if (NULL == m_session_pool)
	{
		DEF_LOG_ERROR("failed to create session pool\n");
		return -1;
	}

	if (m_session_pool->init(1, 1, this, this, this) == -1)
	{
		DEF_LOG_ERROR("failed to init session pool\n");
		return -1;
	}

	if (!m_session_pool->listen("0.0.0.0:" + m_listen_port))
	{
		DEF_LOG_ERROR("failed to listen on port : <%s>", m_listen_port.c_str());
		return -1;
	}

	return 0;
}

void ManageSession::finit()
{
	if (NULL != m_session_pool)
	{
		m_session_pool->stop();
		m_session_pool->finit();
	}
}

void ManageSession::setCheckType(CheckGsType check_gs_type)
{
	m_check_gs_type = check_gs_type;
}

void ManageSession::setPort(const std::string & port)
{
	m_listen_port = port;
}

bool ManageSession::isFinish()
{
	return m_is_finish;
}

bool ManageSession::isSuccess()
{
	return m_is_success;
}

void ManageSession::handleGsStartupReport(Packet * packet)
{
	if (CGT_STARTUP == m_check_gs_type)
	{
		if (packet->guid() == 1)
		{
			// success
			m_is_success = true;
			DEF_LOG_INFO("recv gs startup report message, startup success\n");
		}
		else
		{
			m_is_success = false;
			DEF_LOG_ERROR("recv gs startup report msg, the the value is <%llu>\n", packet->guid());
		}
		m_is_finish = true;
	}
	else
	{
		DEF_LOG_ERROR("the check gs type is <%d>, but recv the msg : <%d>\n", m_check_gs_type, packet->opcode());
	}
}

void ManageSession::handleGsShutdownReport(Packet * packet)
{
	if (CGT_SHUTDOWN == m_check_gs_type)
	{
		if (packet->guid() == 1)
		{
			// success
			m_is_success = true;
			DEF_LOG_INFO("recv gs shutdown report message, shutdown success\n");
		}
		else
		{
			m_is_success = false;
			DEF_LOG_ERROR("recv gs shutdown report msg, the the value is <%llu>\n", packet->guid());
		}
		m_is_finish = true;
	}
	else
	{
		DEF_LOG_ERROR("the check gs type is <%d>, but recv the msg : <%d>\n", m_check_gs_type, packet->opcode());
	}
}