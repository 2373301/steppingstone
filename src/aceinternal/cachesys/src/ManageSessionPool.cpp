
#include "ManageSessionPool.h"
#include "config.h"

namespace cached {
	namespace service {

ManageSessionPool::ManageSessionPool()
: m_session_pool(NULL)
{

}

ManageSessionPool::~ManageSessionPool()
{
	delete m_session_pool;
}

int ManageSessionPool::init()
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

	string addr = CacheConfiguration::instance()->lookup(TERMINAL_LISTEN_IP) + ":" + CacheConfiguration::instance()->lookup(TERMINAL_LISTEN_PORT);

	if (!m_session_pool->listen(addr))
	{
		DEF_LOG_ERROR("failed to listen at addr : <%s>\n", CacheConfiguration::instance()->lookup(SERIALIZE_LISTEN_ADDR).c_str());
		return -1;
	}

	return 0;
}

int ManageSessionPool::stop()
{
	m_session_pool->stop();
	return 0;
}

int ManageSessionPool::finit()
{
	m_session_pool->finit();
	return 0;
}

void ManageSessionPool::input(Packet * packet)
{
	EntityHander::instance()->input(packet);
}

void ManageSessionPool::output(Packet * packet)
{
	m_session_pool->output(packet);
}

//
void ManageSessionPool::sessionOpen(Sessionx * session)
{
	m_session_set.insert(session);
}

void ManageSessionPool::sessionClosed(Sessionx * session)
{
	m_session_set.erase(session);
}

//
void ManageSessionPool::sessionRouterAdd(Packet * packet)
{
	// do nothing
}

void ManageSessionPool::sessionRouterRemove(uint64 guid)
{
	// do nothing
}

Sessionx * ManageSessionPool::getSession(Packet * packet)
{
	SessionSet_t::iterator it = m_session_set.find((Sessionx *)packet->getOwner());
	if (it != m_session_set.end())
	{
		return *it;
	}
	else
	{
		DEF_LOG_ERROR("failed to get session for packet, guid is <%llu>, session point is <%x>\n", packet->guid(), packet->getOwner());
		return NULL;
	}
}

	};
};