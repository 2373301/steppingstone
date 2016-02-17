
#include "ManageConvertSession.h"
#include "cachesys_opcode.hpp"
#include "protocol/msg_binary.pb.h"
#include "ManageExpiredItem.h"
#include "SessionPoolFactory.h"

ManageConvertSession::ManageConvertSession()
: m_session_pool(NULL)
, m_stop(false)
, m_wait(true)
, m_convert_is_available(false)
{

}

ManageConvertSession::~ManageConvertSession()
{
	if (NULL != m_session_pool)
		delete m_session_pool;
}

int ManageConvertSession::init(const string & cache_listen_addr)
{
	m_session_pool = netcore::SessionPoolFactory::createSessionPool();
	if (NULL == m_session_pool)
	{
		return -1;
	}

	if (m_session_pool->init(1, 1, this, this, this) == -1)
	{
		return -1;
	}

	m_cache_addr = cache_listen_addr;

	if (!m_session_pool->listen(m_cache_addr))
	{
		DEF_LOG_ERROR("failed to init session pool in ManageConvertSession\n");
		return -1;
	}

	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

int ManageConvertSession::stop()
{
	m_stop = true;

	if (NULL != m_session_pool)
	{
		m_session_pool->stop();
	}

	return 0;
}

int ManageConvertSession::finit()
{
	m_session_pool->finit();
	//while (true)
	//{
	//	std::cout << "wait ManageConvertSession" << std::endl;
	//	ACE_OS::sleep(1);
	//}

	std::cout << "exit ManageConvertSession" << std::endl;

	return 0;
}

int ManageConvertSession::svc()
{
	FilePathVec_t file_path_vec;

	ACE_Time_Value sleep_time(0, 1 * 1000);

	while (!m_stop)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_file_path_vec_mutex, -1);
			std::copy(m_file_path_vec.begin(), m_file_path_vec.end(), std::back_inserter(file_path_vec));
			m_file_path_vec.clear();
		}

		if (file_path_vec.size() == 0)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		if (!m_convert_is_available)
		{
			ACE_OS::sleep(10);
			DEF_LOG_ERROR("the convert is not available, the path size is <%d>\n", file_path_vec.size());
			continue;
		}

		transferBinaryFile(m_convert_is_available, file_path_vec);
	}

	m_wait = false;
	return 0;
}

void ManageConvertSession::input(Packet * packet)
{
	if (DCMSG_BINARY_RECORD == packet->opcode())
	{
		handleBinaryRecord(packet);
	}

	delete packet;
}

void ManageConvertSession::sessionOpen(Session * session)
{
	DEF_LOG_INFO("get new convert connection, the point is <%x>\n", session);
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_vec_mutex, );
		m_session_vec.push_back(session);
		m_convert_is_available = m_session_vec.size() > 0;
	}
}

void ManageConvertSession::sessionClosed(Session * session)
{
	DEF_LOG_ERROR("the convert connection is closed <%x>\n", session);

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_vec_mutex, );
	SessionVec_t::iterator it = std::find(m_session_vec.begin(), m_session_vec.end(), session);
	if (it != m_session_vec.end())
	{
		m_session_vec.erase(it);
	}
	m_convert_is_available = m_session_vec.size() > 0;
}

void ManageConvertSession::sessionRouterAdd(Packet * packet)
{
	// do nothing
}

void ManageConvertSession::sessionRouterRemove(uint64 guid)
{
	// do nothing
}

Session * ManageConvertSession::getSession(Packet * packet)
{
	// do nothing
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_session_vec_mutex, NULL);
	if (m_session_vec.size() > 0)
	{
		return *m_session_vec.begin();
	}
	else
	{
		return NULL;
	}
}

void ManageConvertSession::handleFile(const string & file_name)
{
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_file_path_vec_mutex, );
		m_file_path_vec.push_back(file_name);
	}
}

void ManageConvertSession::handleBinaryRecord(Packet * packet)
{
	protocol::msg_binary_record binary_record;
	if (parsePacket(packet, &binary_record))
	{
		ManageExpiredItem::instance()->remove(binary_record.guid(), binary_record.last_modify_time());
	}
	else
	{
		// error
	}
}

void ManageConvertSession::transferBinaryFile(bool convert_available, FilePathVec_t & file_path_vec)
{
	if (!convert_available)
	{
		DEF_LOG_ERROR("failed to transfer binary file, the convert is not available, file vec size is <%d>\n", file_path_vec.size());
		return ;
	}

	protocol::msg_binary_file msg_bin_file;
	msg_bin_file.set_cache_addr(m_cache_addr);
	DEF_LOG_DEBUG("the cache addr is <%s>\n", m_cache_addr.c_str());

	Packet * packet = NULL;

	for (FilePathVec_t::iterator it = file_path_vec.begin(); it != file_path_vec.end(); ++it)
	{
		DEF_LOG_DEBUG("start to send sqlite file to convert, file name <%s>\n", (*it).c_str());
		msg_bin_file.set_binary_file(*it);
		packet = new Packet(DCMSG_BINARY_FILE, 0, msg_bin_file.SerializeAsString());
		m_session_pool->output(packet);
	}

	file_path_vec.clear();
}