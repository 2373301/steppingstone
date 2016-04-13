
#include "ManageLocalCache.h"

int ManageLocalCache::stop()
{
	return 0;
}

int ManageLocalCache::finit()
{
	return 0;
}

void ManageLocalCache::cacheOutput(Packet * packet, uint64 map_id, uint64 request_id, uint64 owner_guid)
{
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_guid_vec_mutex, );
		m_guid_vec.push_back(ReqPackInfo(request_id, packet->guid(), map_id));
	}

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_vec_mutex, );
		m_output_packet_vec.push_back(packet);
	}
}

//void ManageLocalCache::input(Packet * packet)
//{
//	uint64 map_id = 0;
//	uint64 request_id = 0;
//	getMapID(packet->guid(), true, map_id, request_id);
//	m_cache_cfg.handle_input->cacheInput(packet, map_id, request_id);
//}

void ManageLocalCache::IStreamIn_read(Session * session, ACE_Message_Block & msg_block)
{
	
}

int ManageLocalCache::svc()
{
	REPORT_THREAD_INFO();

	PacketVec_t ouput_packet_vec;

	while (true)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_vec_mutex, 0);
			std::copy(m_output_packet_vec.begin(), m_output_packet_vec.end(), std::back_inserter(ouput_packet_vec));
			m_output_packet_vec.clear();
		}

		for (PacketVec_t::iterator it = ouput_packet_vec.begin(); it != ouput_packet_vec.end(); ++it)
		{
			m_local_cache.output(*it);
		}
		ouput_packet_vec.clear();
	}

	return 0;
}

int ManageLocalCache::initing()
{
	if (this->activate() == -1)
	{
		return -1;
	}

	return m_local_cache.init(m_cache_cfg.local_path, this);
}

bool ManageLocalCache::getMapID(uint64 guid, bool erase, uint64 & map_id, uint64 & request_id)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_guid_vec_mutex, 0);
	GUIDVect_t::iterator it = m_guid_vec.begin();
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