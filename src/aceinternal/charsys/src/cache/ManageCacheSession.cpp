
#include "ManageCacheSession.h"
#include "SessionPoolFactory.h"
#include "Logger.h"

ManageCacheSession::ManageCacheSession()
: m_session_pool(NULL)
, m_handle_input(NULL)
{

}

ManageCacheSession::~ManageCacheSession()
{

}

int ManageCacheSession::init(const CacheConfigVec_t & cache_cfg)
{
	m_session_pool = netcore::SessionPoolFactory::createSessionPool();
	if (NULL == m_session_pool)
	{
		return -1;
	}

	m_session_pool->savePackStream();

	if (m_session_pool->init(1, 1, this, this, this) == -1)
	{
		return -1;
	}

	netcore::SessionAddrVec_t session_addr;
	for (CacheConfigVec_t::const_iterator it = cache_cfg.begin(); it != cache_cfg.end(); ++it)
	{
		session_addr.push_back(it->ip);
	}

	if (!m_session_pool->connect(session_addr))
	{
		return -1;
	}

	return 0;
}

void ManageCacheSession::setHandleInput(HandleInputx * handle_input)
{
	m_handle_input = handle_input;
}

void ManageCacheSession::input(Packet * packet)
{
	if (NULL != m_handle_input)
	{
		m_handle_input->input(packet);
	}
}

void ManageCacheSession::output(Packet * packet, uint64 player_guid)
{
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_player_guid_mutex, );
		m_packet_player_guid[packet] = player_guid;
	}

	if (NULL != m_session_pool)
	{
		DEF_LOG_DEBUG("start to send packat, opcode is <%d>, guid is <%llu>\n", packet->opcode(), packet->guid());
		m_session_pool->output(packet);
	}
}

void ManageCacheSession::sessionOpen(Sessionx * session)
{
	m_session_vec.push_back(session);
}

void ManageCacheSession::sessionClosed(Sessionx * session)
{
	// never happen where is running
}

void ManageCacheSession::sessionRouterAdd(Packet * packet)
{
	// do nothing
}

void ManageCacheSession::sessionRouterRemove(uint64 guid)
{
	// do nothing
}

Sessionx * ManageCacheSession::getSession(Packet * packet)
{
	if (m_session_vec.size() == 0)
	{
		// error, should never happen
		DEF_LOG_ERROR("the session size is zero in ManageCacheSession::getSession\n");
		return NULL;
	}

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_player_guid_mutex, NULL);

	PacketPlayerGuid_t::iterator it = m_packet_player_guid.find(packet);
	if (it == m_packet_player_guid.end())
	{
		DEF_LOG_ERROR("failed to get packet player guid in ManageCacheSession::getSession\n");
		return NULL;
	}

	int index = it->second % m_session_vec.size();

	m_packet_player_guid.erase(it);

	return m_session_vec[index];
}
