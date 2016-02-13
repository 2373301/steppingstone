
#include <boost/lexical_cast.hpp>
#include <ace/SOCK_Connector.h>
#include <ace/Select_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include "ManageCacheSession.h"
#include "ManageConfig.h"
#include "CacheSession.h"
#include "cachesys_opcode.hpp"
#include "Logger.h"
#include "protocol/msg_binary.pb.h"
#include "ManageSQLiteFile.h"

ManageCacheSession::ManageCacheSession()
: m_reactor(NULL)
{
}

ManageCacheSession::~ManageCacheSession()
{

}

int ManageCacheSession::svc()
{
	m_reactor->owner(ACE_OS::thr_self());

	int reactor_result = 0;	

	while (!m_stop)
	{
		reactor_result = m_reactor->run_reactor_event_loop();

#ifdef WIN32
		break;
#else
		int last_error = ACE_OS::last_error();
		if (EINTR == last_error)
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

void ManageCacheSession::input(Packet * packet)
{
	switch (packet->opcode())
	{
	case DCMSG_BINARY_FILE:
		handleSqliteFile(packet);
		break;
	default:
		// error
		DEF_LOG_ERROR("get unknown opcode in ManageCacheSession::input, opcode is <%d>, size is <%d>, guid is <%llu>\n", packet->opcode(), packet->body_size(), packet->guid());
		break;
	}

	delete packet;
}

void ManageCacheSession::output(const string & addr, Packet * ps)
{
	map<string, CacheSession *>::iterator it = m_cache_map.find(addr);
	if (it != m_cache_map.end())
	{
		it->second->output(ps);
	}
	else
	{
		DEF_LOG_ERROR("error to get cache session by addr : <%s>\n", addr.c_str());
	}
}

int ManageCacheSession::init()
{
#ifdef WIN32
	m_reactor = new ACE_Reactor(new ACE_Select_Reactor(), true);
#else
	m_reactor = new ACE_Reactor(new ACE_Dev_Poll_Reactor(), true);
#endif // WIN32

	ManageSqliteFile::instance()->setHandleOutput(this);

	if (connectCache() == -1)
		return -1;

	if (this->activate() == -1)
	{
		return -1;
	}

	if (m_cache_output.activate(THR_NEW_LWP, ManageConfig::instance()->getCacheCfg().cache_vec.size()) == -1)
	{
		return -1;
	}
	return 0;
}

int ManageCacheSession::stop()
{
	return 0;
}

int ManageCacheSession::finit()
{
	return 0;
}

int ManageCacheSession::connectCache()
{
	ACE_SOCK_Connector connector;
	ACE_INET_Addr addr;

	CacheSession * cache_session = NULL;

	CacheCfg & cache_cfg = ManageConfig::instance()->getCacheCfg();
	for (vector<string>::iterator it = cache_cfg.cache_vec.begin(); it != cache_cfg.cache_vec.end(); ++it)
	{
		addr.set(it->c_str());
		cache_session = new CacheSession();

		if (connector.connect(cache_session->peer(), addr) == -1)
		{
			return -1;
		}
		else
		{
			cache_session->open();
			string save_fn = "stream";
			cache_session->setSavePackInfo(true, save_fn);
		}

		cache_session->setHandleInput(this);

		m_cache_map[*it] = cache_session;

		m_reactor->register_handler(cache_session, ACE_Event_Handler::READ_MASK);

		m_cache_output.handleCache(cache_session);
	}

	return 0;
}

void ManageCacheSession::handleSqliteFile(Packet * packet)
{
	protocol::msg_binary_file sqlite_file;
	if (parsePacket(packet, &sqlite_file))
	{
		ManageSqliteFile::instance()->handleFile(sqlite_file.cache_addr(), sqlite_file.binary_file());
	}
	else
	{
		// error
		DEF_LOG_ERROR("failed to parse msg_binary_file, opcode is <%d>, size is <%d>, last error is <%d>\n", packet->opcode(), packet->body_size(), ACE_OS::last_error());
	}
}