#include <boost/lexical_cast.hpp>
#include <ace/SOCK_Connector.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "ManageRemoteCache.h"
#include "CacheFactory.h"
#include "Logger.h"
#include "share_fun.h"

ManageRemoteCache::ManageRemoteCache()
: m_is_stop(false)
{

}

ManageRemoteCache::~ManageRemoteCache()
{
	cleanVectorContent(m_output_packet_vec);
	cleanVectorContent(m_cache_vec);
	cleanVectorContent(m_reactor_vec);
}

int ManageRemoteCache::svc()
{
	REPORT_THREAD_INFO();

	ACE_Reactor * reactor = NULL;

#ifdef WIN32
	reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(), true);
#endif // WIN32

	registerReactor(reactor);

	int reactor_result = 0;	

	while (!m_is_stop)
	{
		reactor_result = reactor->run_reactor_event_loop();
#ifdef WIN32
		break;
#else
		int last_error = ACE_OS::last_error();
		if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
		{
			continue;
		}
		else
		{
			break;
		}
#endif
	}

	return 0;
}

//void ManageRemoteCache::input(Packet * packet)
//{
//	uint64 map_id = 0;
//	uint64 request_id = 0;
//	if (getMapID(packet->guid(), true, map_id, request_id))
//	{
//		m_cache_cfg.handle_input->cacheInput(packet, map_id, request_id);
//	}
//	else
//	{
//		DEF_LOG_ERROR("failed to get packet request id, guid is <%s>\n", boost::lexical_cast<string>(packet->guid()).c_str());
//	}
//}

void ManageRemoteCache::input(Session * session, ACE_Message_Block & msg_block)
{
	PacketVec_t packet_vec;
	parsePacketFromStream(session, msg_block, packet_vec);
	for (PacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
	{
		Packet * packet = *it;
		uint64 map_id = 0;
		uint64 request_id = 0;
		if (getMapID(packet->guid(), true, map_id, request_id))
		{
			m_cache_cfg.handle_input->cacheInput(packet, map_id, request_id);
		}
		else
		{
			DEF_LOG_ERROR("failed to get packet request id, guid is <%s>\n", boost::lexical_cast<string>(packet->guid()).c_str());
		}
	}
}

void ManageRemoteCache::cacheOutput(Packet * packet, uint64 map_id, uint64 request_id, uint64 owner_guid)
{
	{
		if (0 != request_id)
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_guid_vec_mutex, );
			m_guid_vec.push_back(ReqPackInfo(request_id, packet->guid(), map_id));
		}
	}

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_vec_mutex, );
		auto index = owner_guid % m_cache_vec.size();
		m_cache_vec[index]->output(packet);
	}
}

int ManageRemoteCache::stop()
{
	m_is_stop = true;

	for (ReactorVec_t::iterator it = m_reactor_vec.begin(); it != m_reactor_vec.end(); ++it)
	{
		ACE_Reactor * reactor = *it;
		reactor->end_reactor_event_loop();
	}

	m_manage_remote_cache_output.stop();
	return 0;
}

int ManageRemoteCache::finit()
{
	return 0;
}

int ManageRemoteCache::initing()
{
	if (this->activate(THR_JOINABLE, m_cache_cfg.network_thread_no) == -1)
	{
		DEF_LOG_ERROR("failed to activate ManageRemoteCache, thread no is <%d>, last error is <%d>\n", m_cache_cfg.network_thread_no, ACE_OS::last_error());
		return -1;
	}

	if (m_manage_remote_cache_output.activate(THR_JOINABLE, (int)m_cache_cfg.remote_cfg.cache_addrs.size()) == -1)
	{	
		DEF_LOG_ERROR("failed to activate ManageRemoteCache out, thread no is <%d>, last error is <%d>\n",
			(int)m_cache_cfg.remote_cfg.cache_addrs.size(), ACE_OS::last_error());
		return -1;
	}

	if (connectRemoteCache() == -1)
	{	
		return -1;
	}

	return 0;
}

int ManageRemoteCache::connectRemoteCache()
{
	ACE_SOCK_Connector connector;
	ACE_INET_Addr addr;

	int cache_index = 0;
	for (RemoteCfg::CacheVec_t::iterator it = m_cache_cfg.remote_cfg.cache_addrs.begin(); it != m_cache_cfg.remote_cfg.cache_addrs.end(); ++it)
	{
		addr.set(it->c_str());
		RemoteCache * remote_cache = new RemoteCache();
		if (connector.connect(remote_cache->peer(), addr) == -1)
		{	
			DEF_LOG_ERROR("failed to connectRemoteCache, remote ip is <%s>, last error is <%d>\n",
				it->c_str(), ACE_OS::last_error());
			return -1;
		}
		else
		{
			remote_cache->init("", this);
			m_cache_vec.push_back(remote_cache);
		}
	}

	while (true)
	{
		bool reactor_is_ok = false;
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_reactor_vec_mutex, -1);
			reactor_is_ok = m_reactor_vec.size() == m_cache_cfg.network_thread_no;
		}

		if (!reactor_is_ok)
		{
			ACE_OS::sleep(1);
		}
		else
		{
			break;
		}
	}

	// assign cache reactor
	int index = 0;
	int reactor_number = (int)m_reactor_vec.size();
	for (CacheVec_t::iterator it = m_cache_vec.begin(); it != m_cache_vec.end(); ++it)
	{
		ACE_Reactor * reactor = m_reactor_vec[index % reactor_number];
		reactor->register_handler(*it, ACE_Event_Handler::READ_MASK);

		m_manage_remote_cache_output.handleRemoteCache(*it);
		++index;
	}

	return 0;
}

void ManageRemoteCache::registerReactor(ACE_Reactor * reactor)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_reactor_vec_mutex, );
	m_reactor_vec.push_back(reactor);
}

bool ManageRemoteCache::getMapID(uint64 guid, bool erase, uint64 & map_id, uint64 & request_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_guid_vec_mutex, 0);
	GUIDVec_t ::iterator it = m_guid_vec.begin();
	for (; it != m_guid_vec.end(); ++it)
	{
		if (it->guid == guid)
		{
			break;
		}
	}

	if (it != m_guid_vec.end())
	{
		map_id = it->map_id;
		request_id = it->request_id;
		if (erase)
		{
			m_guid_vec.erase(it);
		}
		return true;
	}

	return false;
}
