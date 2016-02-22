
#include <boost/lexical_cast.hpp>
#include "PoolAssistant.h"
#include "protocol/msg_cache.pb.h"
#include "entity/player.pb.h"
#include "protocol/msg_pool.pb.h"
#include "coredef.h"
#include "Logger.h"
#define MK_POOL "[MK_POOL] "

#define FUNCTION_LINE_TRACE		" fun<%s> line<%d>\n"

#define POOL_LOG_DEBUG(log_info, ...)		m_pool_cfg.logger->log(LL_DEBUG, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_INFO(log_info, ...)		m_pool_cfg.logger->log(LL_INFO, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_WARNING(log_info, ...)		m_pool_cfg.logger->log(LL_WARNING, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_TRACE(log_info, ...)		m_pool_cfg.logger->log(LL_TRACE, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_ERROR(log_info, ...)		m_pool_cfg.logger->log(LL_ERROR, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_FATAL(log_info, ...)		m_pool_cfg.logger->log(LL_FATAL, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)

STAT_PACKET_IMP(RequestImp);

STAT_PACKET_IMP(RequestListImp);

bool extractEntity(uint64 guid, const string & stream, GOOGLE_MESSAGE_TYPE * * entity_msg)
{
	bool result = false;
	GOOGLE_MESSAGE_TYPE * message = NULL;
	if (NULL != message)
	{
		if (parsePacket(stream, message))
		{
			*entity_msg = message;
			result = true;
		}
		else
		{
			delete message;
			result = false;
		}
	}
	else
	{
		result = false;
	}

	return result;
}

RequestImp::RequestImp(RequestType rt, uint64 entity_guid, GOOGLE_MESSAGE_TYPE * msg, const PoolCfgx pool_cfg, uint64 owner_g)
{
	request_type = rt;
	guid = entity_guid;
	success = false;
	complated = false;
	message = msg;
	owner_guid = owner_g;
	m_pool_cfg = pool_cfg;

	STAT_PACKET_INC_NEW();
}

RequestImp::~RequestImp()
{
	STAT_PACKET_INC_DEL();
}

int RequestImp::handlePacket(Packet * packet)
{
	int result = -1;
	typed::cached::protocol::Result op_result;
	if (PARSE_PACKET(packet, &op_result))
	{
		success = op_result.status() == typed::cached::protocol::Result_status_t_STATE_NORMAL;
		if (success)
		{
			result = 0;
			switch(request_type)
			{
			case RT_ADD:
			case RT_ADD_WITH_FLUSH:
				// ok , do nothing
				break;
			case RT_LOAD:
			case RT_LOADONCE:
				if (op_result.has_message())
				{
					if (!extractEntity(this->guid, op_result.message(), &this->message))
					{
						POOL_LOG_ERROR("failed to extract entity from cache packet, guid is <%s>", boost::lexical_cast<string>(packet->guid()).c_str());
						result = 1;
					}
				}
				else
				{
					// error
					POOL_LOG_ERROR("load entity while get empty entity message from cache, guid is <%s>", boost::lexical_cast<string>(packet->guid()).c_str());
					result = 1;
				}
				break;
			case RT_QUERY:
				if (op_result.has_message())
				{
					if (!extractEntity(this->guid, op_result.message(), &this->message))
					{
						result = -1;
					}
				}
				else
				{
					// error
					result = -1;
				}
				break;
			case RT_UPDATE:		// ok , do nothing
				break;
			case RT_FLUSH:		// ok , do nothing
				break;
			case RT_DELETE:		// ok , do nothing
				break;
			default:
				break;
			}
		}
		else
		{
			// error
			POOL_LOG_ERROR("get unnormal state packet from cache, op result is <%d>, guid is <%llu>", op_result.status(), packet->guid());
		}
	}
	else
	{
		POOL_LOG_ERROR("failed to parse packet from cache, guid is <%llu>", packet->guid());
		// error
	}

	return 0;
}

void RequestImp::setOwnerGuid(uint64 owner_g)
{
	owner_guid = owner_g;
}

RequestListImp::RequestListImp(const PoolCfgx pool_cfg)
{
	m_complated = false;
	m_complated_count = 0;
	m_request_id = 0;
	m_pool_cfg = pool_cfg;

	STAT_PACKET_INC_NEW();
}

RequestListImp::~RequestListImp()
{
	STAT_PACKET_INC_DEL();
}

const RequestListImp::RequestVec_t & RequestListImp::getRequestList()
{
	return m_request_vec;
}

void RequestListImp::query(uint64 guid, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_QUERY, guid, NULL, m_pool_cfg, owner_guid));
}

void RequestListImp::add(uint64 guid, GOOGLE_MESSAGE_TYPE * message, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_ADD, guid, message, m_pool_cfg, owner_guid));
}

void RequestListImp::addWithFlush(uint64 guid, GOOGLE_MESSAGE_TYPE * message, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_ADD_WITH_FLUSH, guid, message, m_pool_cfg, owner_guid));
}

void RequestListImp::update(uint64 guid, GOOGLE_MESSAGE_TYPE * message, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_UPDATE, guid, message, m_pool_cfg, owner_guid));
}

void RequestListImp::load(uint64 guid, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_LOAD, guid, NULL, m_pool_cfg, owner_guid));
}

void RequestListImp::loadOnce(uint64 guid, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_LOADONCE, guid, NULL, m_pool_cfg, owner_guid));
}

void RequestListImp::flush(uint64 guid, GOOGLE_MESSAGE_TYPE * message, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_FLUSH, guid, message, m_pool_cfg, owner_guid));
}

void RequestListImp::remove(uint64 guid, uint64 owner_guid)
{
	m_request_vec.push_back(new RequestImp(RT_DELETE, guid, NULL, m_pool_cfg, owner_guid));
}

bool RequestListImp::is_complated()
{
	return m_complated;
}

bool RequestListImp::is_success()
{
	for (RequestVec_t::iterator it = m_request_vec.begin(); it != m_request_vec.end(); ++it)
	{
		if (!(*it)->success)
		{
			return false;
		}
	}

	return true;
}

bool RequestListImp::is_all_failed()
{
	for (RequestVec_t::iterator it = m_request_vec.begin(); it != m_request_vec.end(); ++it)
	{
		if ((*it)->success)
		{
			return false;
		}
	}

	return true;
}

void RequestListImp::setRequestID(uint64 request_id)
{
	m_request_id = request_id;
}

uint64 RequestListImp::getRequestID()
{
	return m_request_id;
}

int RequestListImp::handlePacket(Packet * packet)
{
	int result = -1;
	int packet_process_result = -1;
	for (RequestVec_t::iterator it = m_request_vec.begin(); it != m_request_vec.end(); ++it)
	{
		Request * request = *it;
		if (request->guid == packet->guid())
		{
			result = 0;

			packet_process_result = request->handlePacket(packet);
			if (++m_complated_count == m_request_vec.size())
			{
				m_complated = true;
			}

			break;
		}
	}

	return result;
}