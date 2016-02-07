
#include <ace/OS.h>
#include "ManageSession.h"
#include "SessionPoolFactory.h"
#include "Logger.h"
#include "opcode.h"
#include "ManageConfig.h"

ManageSession::ManageSession()
: m_session_pool(NULL)
{
}

ManageSession::~ManageSession()
{}

int ManageSession::init()
{
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

	if (!m_session_pool->listen(ManageConfig::instance()->getGuidCfg().addr))
	{
		DEF_LOG_ERROR("failed to listen, addr is <%s>, last error is <%d>\n", ManageConfig::instance()->getGuidCfg().addr.c_str(), ACE_OS::last_error());
		return -1;
	}

	m_alloc_guid.init(ManageConfig::instance()->getGuidCfg().seed_value, ManageConfig::instance()->getGuidCfg().index);

	DEF_LOG_INFO("success to init ManageSession, guidsys is running\n");
	return 0;
}

int ManageSession::stop()
{
	m_stop = true;
	return 0;
}

int ManageSession::finit()
{
	return 0;
}

void ManageSession::input(Packet * packet)
{
	std::auto_ptr<Packet> auto_pak(packet);
	if (packet->opcode() == SMSG_GUID_REQUEST)
	{
		Packet * ps = m_alloc_guid.processRequest(packet);
		if (NULL != ps)
		{
			m_session_pool->output(ps);
		}
		else
		{
			// error 
			DEF_LOG_ERROR("failed to process guid request\n");
		}
	}
}

void ManageSession::newConnection(Session * session)
{

}

void ManageSession::connectionClosed(Session * session)
{

}

void ManageSession::addRoute(Packet * packet)
{

}

void ManageSession::removeRoute(uint64 guid)
{

}

Session * ManageSession::getSession(Packet * packet)
{
	return (Session *)packet->getOwner();
}
