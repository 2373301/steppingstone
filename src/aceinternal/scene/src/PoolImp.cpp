
#include "typedef.h"
#include <limits>
#include <boost/lexical_cast.hpp>
#include "PoolImp.h"
#include "Logger.h"
#include "cachesys_opcode.hpp"
#include "protocol/msg_cache.pb.h"
#include "entity/player.pb.h"
#include "coredef.h"
#include "PoolAssistant.h"
#include "share_fun.h"
#include "Scene.h"

string empty_string = "";

#define MK_POOL "[MK_POOL] "

#define FUNCTION_LINE_TRACE		" fun<%s> line<%d>\n"

#define POOL_LOG_DEBUG(log_info, ...)		m_pool_cfg.logger->log(LL_DEBUG, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_INFO(log_info, ...)		m_pool_cfg.logger->log(LL_INFO, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_WARNING(log_info, ...)		m_pool_cfg.logger->log(LL_WARNING, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_TRACE(log_info, ...)		m_pool_cfg.logger->log(LL_TRACE, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_ERROR(log_info, ...)		m_pool_cfg.logger->log(LL_ERROR, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define POOL_LOG_FATAL(log_info, ...)		m_pool_cfg.logger->log(LL_FATAL, "%s" MK_POOL log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)

STAT_PACKET_IMP(EntityInfo);

STAT_PACKET_IMP(DbEntityInfo);

STAT_PACKET_IMP(RequestInfo);

PoolImp::PoolImp()
: m_request_id(0)
, m_transaction_index(0)
, m_transaction_id(0)
, m_update_to_db_num(0)
, m_do_not_get_the_replay_time(0)
{

}

PoolImp::~PoolImp()
{
	cleanMapSecondContent(m_entity_map);
	cleanVectorContent(m_request_info_vec);
	for (OwnerDbEntityMap_t::iterator it = m_db_entity_map.begin(); it != m_db_entity_map.end(); ++it)
	{
		cleanMapSecondContent(it->second);
	}

	for (OwnerDbEntityMap_t::iterator it = m_updating_db_entity_map.begin(); it != m_updating_db_entity_map.end(); ++it)
	{
		cleanMapSecondContent(it->second);
	}
}

void PoolImp::cacheInput(Packet * packet, uint64 map_id, uint64 request_id)
{
	handlePacket(packet, request_id);
}

int PoolImp::init(const PoolCfg pool_cfg)
{
	if (NULL == pool_cfg.handle_output)
	{
		return -1;
	}

	m_pool_cfg = pool_cfg;

	return 0;
}

GOOGLE_MESSAGE_TYPE * PoolImp::get(uint64 guid)
{
	EntityMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		if (!it->second->is_flushing)
		{
			return it->second->message;
		}
	}

	return NULL;
}

EntityInfo * PoolImp::getEntityInfo(uint64 guid)
{
	EntityMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		return it->second;
	}

	return NULL;
}

GOOGLE_MESSAGE_TYPE * PoolImp::getWhileFlush(uint64 guid)
{
	EntityMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		if (!it->second->is_flushing)
		{
			it->second->is_flushing = true;
			return it->second->message;
		}
	}

	return NULL;
}

void PoolImp::clearFlushState(uint64 guid)
{
	EntityMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		it->second->is_flushing = false;
	}
}

void PoolImp::beginTransaction()
{
	// do nothing currently
}

void PoolImp::endtransaction()
{
	// do nothing currently
}

bool PoolImp::add(uint64 guid, GOOGLE_MESSAGE_TYPE * message, bool add_to_db, uint64 owner_guid)
{
	if (NULL == message)
	{
		return false;
	}

	POOL_LOG_DEBUG("add entity to pool, guid is <%llu>, owner guid is <%llu>", guid, owner_guid);

	m_guid_ownerguid_map[guid] = owner_guid;

	EntityMap_t::iterator it = m_entity_map.find(guid);
	if (it == m_entity_map.end())
	{
		EntityInfo * info = new EntityInfo(EOT_ADD, message, owner_guid);
		m_entity_map.insert(std::make_pair(guid, info));
		if (add_to_db)
		{
			if (EXTRACT_ENTITY_TYPE(guid) == ET_COIN)
			{
				directUpdateToDb(message, guid, owner_guid);
			}
			else
			{
				updateToDb(owner_guid, guid, EOT_ADD);
			}
		}
		return true;
	}
	else
	{
		// error
		POOL_LOG_ERROR("failed to add entity to pool, reduplicate guid <%s>", boost::lexical_cast<string>(guid).c_str());
		return false;
	}
}

bool PoolImp::remove(uint64 guid, bool remove_from_db, bool remove_entity_msg)
{
	POOL_LOG_DEBUG("remove entity to pool, guid is <%llu>, remove from db <%d>", guid, remove_from_db);

	if (IS_PLAYER_GUID(guid))
	{
		m_online_player.erase(guid);
	}

	uint64 owner_guid = 0;
	{
		GuidOwnerGuidMap_t::iterator it = m_guid_ownerguid_map.find(guid);
		if (it != m_guid_ownerguid_map.end())
		{
			owner_guid = it->second;
			m_guid_ownerguid_map.erase(it);
		}
		else
		{
			POOL_LOG_ERROR("failed to get owner_guid of guid while remove <%llu>", guid);
			return false;
		}
	}

	EntityMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		bool is_updating = false;

		if (remove_from_db)
		{
			updateToDb(owner_guid, guid, EOT_REMOVE);
		}
		else
		{
			is_updating = isUpdating(owner_guid, guid);
		}

		if (!remove_entity_msg)
		{
			it->second->message = NULL;
		}

		if (!is_updating)
		{
			delete it->second;
			m_entity_map.erase(it);
		}
		else
		{
			it->second->is_remove_from_memory = true;
		}

		return true;
	}
	else
	{
		POOL_LOG_ERROR("failed to remove guid from pool, not found <%llu>", guid);
		return false;
	}
}

bool PoolImp::update(uint64 guid)
{
	POOL_LOG_DEBUG("update entity, guid is <%llu>", guid);

	if ((IS_PLAYER_GUID(guid) || (IS_ROLE_GUID(guid))) && m_pool_cfg.scene->isLineScene())
	{
		return true;
	}

	uint64 owner_guid = 0;
	{
		GuidOwnerGuidMap_t::iterator it = m_guid_ownerguid_map.find(guid);
		if (it != m_guid_ownerguid_map.end())
		{
			owner_guid = it->second;
		}
		else
		{
			POOL_LOG_ERROR("failed to get owner_guid of guid while update <%llu>", guid);
			return false;
		}
	}

	EntityMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		it->second->op_type |= EOT_UPDATE;
		updateToDb(owner_guid, guid, EOT_UPDATE);
	}
	else
	{
		POOL_LOG_ERROR("failed to update guid from pool, not found <%s>", boost::lexical_cast<string>(guid).c_str());
	}

	return true;
}

void PoolImp::onlyRemoveEntity(uint64 guid, uint64 owner_guid)
{
	protocol::db_data_request data_request;
	data_request.set_transaction_index(0);
	data_request.set_transaction_num(0);
	data_request.set_transaction_id(0);
	data_request.set_line_id(m_pool_cfg.line_id);
	
	Packet * packet = new Packet(DCMSG_ONLY_REMOVE_ENTITY, guid, data_request.SerializeAsString());

	m_pool_cfg.handle_output->cacheOutput(packet, m_pool_cfg.map_id, 0, owner_guid);

}

bool PoolImp::update()
{
	if (m_db_entity_map.size() == 0)
	{
		return true;
	}

	if (0 != m_update_to_db_num)
	{
		++m_do_not_get_the_replay_time;
		if (m_do_not_get_the_replay_time > 200 * 10)
		{
			POOL_LOG_WARNING("do not get the replay of updating to db msg, left num <%d>, line <%d>, mpa <%llu>", m_update_to_db_num, m_pool_cfg.line_id, m_pool_cfg.map_id);
			m_update_to_db_num = 0;
		}
		return true;
	}

	m_do_not_get_the_replay_time = 0;

	if (m_updating_db_entity_map.size() > 0)
	{
		POOL_LOG_ERROR("the updating db entity map size is not zero while update");
		for (OwnerDbEntityMap_t::iterator it = m_updating_db_entity_map.begin(); it != m_updating_db_entity_map.end(); ++it)
		{
			DbEntityMap_t & db_entity_map = it->second;
			for (DbEntityMap_t::iterator db_it = db_entity_map.begin(); db_it != db_entity_map.end(); ++db_it)
			{
				POOL_LOG_ERROR("updating error, owner guid is <%llu>, guid is <%llu>, op type is <%d>", it->first, db_it->first, db_it->second->op_type);
				delete db_it->second;
			}
		}
	}

	m_updating_db_entity_map.clear();

	for (OwnerDbEntityMap_t::iterator it = m_db_entity_map.begin(); it != m_db_entity_map.end(); ++it)
	{
		m_updating_db_entity_map.insert(std::make_pair(it->first, it->second));
	}

	m_db_entity_map.clear();

	if (m_updating_db_entity_map.size() > 500)
	{
		POOL_LOG_ERROR("PoolImp::update, database request no is <%d>", m_updating_db_entity_map.size());
	}

	for (OwnerDbEntityMap_t::iterator it = m_updating_db_entity_map.begin(); it != m_updating_db_entity_map.end(); )
	{
		if (it->second.size() == 0)
		{
			m_updating_db_entity_map.erase(it++);
			continue;
		}

		if (updateDbEntity(it->second))
		{
			++it;
		}
		else
		{
			m_updating_db_entity_map.erase(it++);
		}
	}

	return true;
}

bool PoolImp::isLastUpdateFinish()
{
	return 0 == m_update_to_db_num;
}

int PoolImp::updateEntityNumber()
{
	return m_update_to_db_num + m_db_entity_map.size();
}

void PoolImp::clearUpdateEntity(uint64 guid_owner, Uint64Set_t & insert_entity_set, Uint64Set_t & remove_entity_set)
{
	OwnerDbEntityMap_t::iterator it_owner = m_db_entity_map.find(guid_owner);
	if (it_owner != m_db_entity_map.end())
	{
		DbEntityMap_t & db_entity_map = it_owner->second;
		for (DbEntityMap_t::iterator it = db_entity_map.begin(); it != db_entity_map.end(); ++it)
		{
			DbEntityInfo * db_entity_info = it->second;
			if (EOT_ADD == db_entity_info->op_type)
			{
				insert_entity_set.insert(db_entity_info->guid);
			}
			else if (EOT_REMOVE == db_entity_info->op_type)
			{
				remove_entity_set.insert(db_entity_info->guid);
			}
			delete it->second;
		}

		m_db_entity_map.erase(it_owner);
	}
}

bool PoolImp::commit(RequestList * request_list, RequestCallBack call_back)
{
	m_request_info_vec.push_back(new RequestInfo(request_list, call_back));
	const RequestList::RequestVec_t & request_vec = request_list->getRequestList();
	Packet * packet = NULL;

	m_transaction_index = 0;
	m_transaction_id = ++m_transaction_id % numeric_limits<uint32>::max();
	m_transaction_id = 0 == m_transaction_id ? 1 : m_transaction_id;
	TransactionInfo trans_info(m_transaction_index, request_list->getRequestList().size(), m_transaction_id);

	protocol::db_data_request db_request;

	for (RequestList::RequestVec_t::const_iterator it = request_vec.begin(); it != request_vec.end(); ++it)
	{
		Request * request = *it;

		db_request.set_transaction_index(++trans_info.index);
		db_request.set_transaction_num(trans_info.num);
		db_request.set_transaction_id(trans_info.id);
		db_request.set_line_id(m_pool_cfg.line_id);
		db_request.set_data_stream(empty_string);
		db_request.set_owner_guid(request->owner_guid);

		switch(request->request_type)
		{
		case RT_ADD:
			if (NULL != request->message)
			{
				db_request.set_data_stream(request->message->SerializeAsString());
				packet = new Packet(DCMSG_NEW_ENEITY, request->guid, db_request.SerializeAsString());
			}
			else
			{
				// error
			}
			break;
		case RT_ADD_WITH_FLUSH:
			if (NULL != request->message)
			{
				db_request.set_data_stream(request->message->SerializeAsString());
				packet = new Packet(DCMSG_NEW_ENEITY_WITH_FLUSH, request->guid, db_request.SerializeAsString());
			}
			else
			{
				// error
			}
			break;
		case RT_LOAD:
			//db_request.set_data_stream(empty_string);
			packet = new Packet(DCMSG_LOAD_ENTITY, request->guid, db_request.SerializeAsString());
			break;
		case RT_QUERY:
			//db_request.set_data_stream(empty_string);
			packet = new Packet(DCMSG_QUERY_ENTITY, request->guid, db_request.SerializeAsString());
			break;
		case RT_UPDATE:
			if (NULL != request->message)
			{
				db_request.set_data_stream(request->message->SerializeAsString());
				packet = new Packet(DCMSG_UPDATE_ENTITY, request->guid, db_request.SerializeAsString());
			}
			else
			{
				// error
			}
			break;
		case RT_FLUSH:
			if (NULL != request->message)
			{
				db_request.set_data_stream(request->message->SerializeAsString());
				packet = new Packet(DCMSG_FLUSH_ENTITY, request->guid, db_request.SerializeAsString());
			}
			else
			{
				// error
			}
		    break;
		case RT_DELETE:
			//db_request.set_data_stream(empty_string);
			packet = new Packet(DCMSG_REMOVE_ENTITY, request->guid, db_request.SerializeAsString());
			break;
		case RT_LOADONCE:
			packet = new Packet(DCMSG_LOAD_ENTITY_ONCE, request->guid, db_request.SerializeAsString());
			break;
		default:
		    break;
		}

		if (NULL != packet)
		{
			m_pool_cfg.handle_output->cacheOutput(packet, m_pool_cfg.map_id, request_list->getRequestID(), request->owner_guid);
			packet = NULL;
		}
	}
	return true;
}

Request * PoolImp::createRequset(RequestType rt, uint64 entity_guid, GOOGLE_MESSAGE_TYPE * msg)
{
	// not used
	return NULL;
	//return (Request *)new RequestImp(rt, entity_guid, msg, m_pool_cfg);
}

RequestList * PoolImp::createRequestList()
{
	RequestListImp * req = new RequestListImp(m_pool_cfg);
	req->setRequestID(++m_request_id);
	return (RequestList *)req;
}

int PoolImp::getEntityNumber()
{
	return m_entity_map.size();
}

void PoolImp::playerIsOnline(uint64 player_guid)
{
	m_online_player.insert(player_guid);
}

void PoolImp::playerIsOffline(uint64 player_guid)
{
	m_online_player.erase(player_guid);
}

bool PoolImp::isPlayerOnlineInThisMap(uint64 player_guid)
{
	PlayerGuidSet_t::iterator it = m_online_player.find(player_guid);

	return m_online_player.end() != it ? true : false;
}

const PlayerGuidSet_t & PoolImp::getOnlinePlayer()
{
	return m_online_player;
}

//void PoolImp::getInsertGuidAndClear(uint64 owner_guid, define_unordered_set<uint64> & insert_guid)
//{
//	OwnerDbEntityMap_t::iterator it = m_db_entity_map.find(owner_guid);
//	if (it != m_db_entity_map.end())
//	{
//		// find
//		DbEntityMap_t & db_entity_map = it->second;
//		for (DbEntityMap_t::iterator db_it = db_entity_map.begin(); db_it != db_entity_map.end(); )
//		{
//			DbEntityInfo * db_entity = db_it->second;
//			if (EOT_REMOVE != db_entity->op_type)
//			{
//				if (EOT_ADD == db_entity->op_type)
//				{
//					insert_guid.insert(db_entity->guid);
//				}
//
//				delete db_entity;
//				db_entity_map.erase(db_it++);
//			}
//			else
//			{
//				// do nothing
//				db_it++;
//			}
//		}
//
//		if (db_entity_map.size() == 0)
//		{
//			m_db_entity_map.erase(it);
//		}
//	}
//}

void PoolImp::updateToDbReplay()
{
	if (m_update_to_db_num > 0)
	{
		m_update_to_db_num -= 1;
	}
	else
	{
		POOL_LOG_ERROR("error to compute variable : m_update_to_db_num");
	}
}

void PoolImp::handlePacket(Packet * packet, uint64 request_id)
{
	POOL_LOG_DEBUG("get cache packet, object guid is <%llu>, opcode is <%d>", packet->guid(), packet->opcode());

	std::auto_ptr<Packet> auto_pk(packet);

	int packet_process_result = -1;
	for (RequestInfoVec_t::iterator it = m_request_info_vec.begin(); it != m_request_info_vec.end(); ++it)
	{
		RequestInfo * request_info = *it;
		if (request_info->request_list->getRequestID() != request_id)
		{
			continue;
		}

		packet_process_result = request_info->request_list->handlePacket(packet);
		if (request_info->request_list->is_complated())
		{
			// complated, notify callback
			m_request_info_vec.erase(it);
			complateRequest(request_info);
			delete request_info;
		}

		break;

		//if (0 == packet_process_result)
		//{
		//	if (request_info->request_list->is_complated())
		//	{
		//		// complated, notify callback
		//		m_request_info_vec.erase(it);
		//		complateRequest(request_info);
		//		delete request_info;
		//	}
		//	break;
		//}
	}
}

void PoolImp::complateRequest(RequestInfo * request_info)
{
	request_info->call_back(request_info->request_list);
	//std::auto_ptr<RequestInfo> auto_req(request_info);
}

bool PoolImp::updateDbEntity(DbEntityMap_t & db_entity_map)
{
	DECLARE_REQUEST_LIST(this);
	GOOGLE_MESSAGE_TYPE * entity_ins = NULL;
	EntityInfo * entity_info = NULL;
	for (DbEntityMap_t::iterator it = db_entity_map.begin(); it != db_entity_map.end(); )
	{
		DbEntityInfo * db_info = it->second;
		if (db_info->already_request)
		{
			// never happen
			POOL_LOG_ERROR("get already request entity, guid is <%llu>", it->first);
			delete it->second;
			db_entity_map.erase(it++);
			continue;
		}

		if (EOT_REMOVE != db_info->op_type)
		{
			entity_info = this->getEntityInfo(db_info->guid);
			if (NULL == entity_info)
			{
				POOL_LOG_WARNING("failed to get entity from pool in PoolImp::update, eot_type is <%d>, guid is <%s>", db_info->op_type, boost::lexical_cast<string>(db_info->guid).c_str());
				delete it->second;
				db_entity_map.erase(it++);
				continue;
			}

			if (entity_info->is_flushing)
			{
				delete it->second;
				db_entity_map.erase(it++);
				continue;
			}

			entity_ins = entity_info->message;
		}
		else
		{
			entity_ins = NULL;
		}

		db_info->already_request = true;
		switch (db_info->op_type)
		{
		case EOT_ADD:
			ADD_REQUEST(db_info->guid, entity_ins, db_info->owner_guid);
			//db_info->op_type = EOT_NULL;
			POOL_LOG_DEBUG("update entity to db, op is add, guid is <%llu>", db_info->guid);
			break;
		case EOT_REMOVE:
			REMOVE_REQUEST(db_info->guid, db_info->owner_guid);
			//db_info->op_type = EOT_NULL;
			POOL_LOG_DEBUG("update entity to db, op is remove, guid is <%llu>", db_info->guid);
			break;
		case EOT_UPDATE:
			UPDATE_REQUEST(db_info->guid, entity_ins, db_info->owner_guid);
			//db_info->op_type = EOT_NULL;
			POOL_LOG_DEBUG("update entity to db, op is update, guid is <%llu>", db_info->guid);
			break;
			//case EOT_FLUSH:
			//	FLUSH_REQUEST(db_info->guid, entity_ins, db_info->owner_guid);
			//	POOL_LOG_DEBUG("update entity to db, op is flush, guid is <%s>", boost::lexical_cast<string>(db_info->guid).c_str());
			//	break;
		default:
			POOL_LOG_ERROR("unknown db op type <%d> while call PoolImp::update, guid is <%llu>, owern guid is <%llu>", (int)db_info->op_type, db_info->guid, db_info->owner_guid);
			break;
		}

		++it;
	}

	if (REQUEST_IS_EMPTY())
	{
		DELETE_REQUEST();
	}
	else
	{
		m_update_to_db_num += 1;
		POOL_OP_COMMIT(this, boost::bind(&PoolImp::updateToDbCallback, _1, this));
		return true;
	}

	return false;
}

void PoolImp::updateToDb(uint64 owner_guid, uint64 guid, EntityOpType eot_type)
{
	OwnerDbEntityMap_t::iterator it = m_db_entity_map.find(owner_guid);
	if (m_db_entity_map.end() == it)
	{
		// omit small transaction
		DbEntityInfo * db_info = new DbEntityInfo(eot_type, guid, 0, 0, owner_guid);
		m_db_entity_map[owner_guid].insert(std::make_pair(guid, db_info));
	}
	else
	{
		// update entity status
		DbEntityMap_t & db_entity_map = it->second;
		DbEntityMap_t::iterator db_it = db_entity_map.find(guid);
		if (db_it == db_entity_map.end())
		{
			DbEntityInfo * db_info = new DbEntityInfo(eot_type, guid, 0, 0, owner_guid);
			db_entity_map.insert(std::make_pair(guid, db_info));
		}
		else
		{
			DbEntityInfo * db_info = db_it->second;
			db_info->already_request = false;

			if (EOT_ADD == db_info->op_type)
			{
				if (EOT_REMOVE == eot_type)
				{
					//db_info->op_type = EOT_ADD_REMOVE;
					delete db_info;
					db_info = NULL;
					db_entity_map.erase(db_it);
				}
			}
			else
			{
				db_info->op_type = eot_type;
			}
		}
	}
}

void PoolImp::updateToDbCallback(RequestList * request_list, PoolImp * pool)
{
	pool->updateToDbReplay();
	for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
	{
		Request * req = *it;
		pool->removeDbInfo(req->owner_guid, req->guid, req->success);
	}
}

void PoolImp::removeDbInfo(uint64 owner_guid, uint64 guid, bool success)
{
	POOL_LOG_DEBUG("start to remove db info, guid is <%llu>, owner guid is <%llu>", guid, owner_guid);

	OwnerDbEntityMap_t::iterator it = m_updating_db_entity_map.find(owner_guid);
	if (it != m_updating_db_entity_map.end())
	{
		DbEntityMap_t & db_entity_map = it->second;
		DbEntityMap_t::iterator db_it = db_entity_map.find(guid);
		if (db_it != db_entity_map.end())
		{
			DbEntityInfo * db_info = db_it->second;
			if ((success) && (db_info->already_request))
			{
				switch (db_info->op_type)
				{
				case EOT_ADD:
				case EOT_UPDATE:
					// do nothing
					checkShouldRemovedEntity(db_info->guid);
					break;
				case EOT_REMOVE:
					// do nothing
//					POOL_OP_REMOVE(this, db_info->guid, false);
					// remove it while request
					//this->doRemoveBack(guid);
					checkShouldRemovedEntity(db_info->guid);
					break;
				case EOT_FLUSH:
					POOL_OP_REMOVE(this, db_info->guid, false);
					break;
				default:
					break;
				}

				db_info = NULL;
				delete db_it->second;
				db_entity_map.erase(db_it);
			}
			else if (!success)
			{
				db_it->second->already_request = false;
				POOL_LOG_ERROR("failed to update entity to database, guid is <%s>", boost::lexical_cast<string>(guid).c_str());
			}
		}
		else
		{
			POOL_LOG_ERROR("failed to find guid <%llu>", guid);
		}

		if (db_entity_map.size() == 0)
		{
			m_updating_db_entity_map.erase(it);
		}
	}
	else
	{
		POOL_LOG_ERROR("failed to find owner guid <%llu>", owner_guid);
	}
}

void PoolImp::doRemoveBack(uint64 guid)
{
	EntityMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		delete it->second;
		m_entity_map.erase(it);
	}
}

bool PoolImp::isUpdating(uint64 owner_guid, uint64 guid)
{
	OwnerDbEntityMap_t::iterator it = m_db_entity_map.find(owner_guid);
	if (m_db_entity_map.end() == it)
	{
		return false;
	}

	// update entity status
	DbEntityMap_t & db_entity_map = it->second;
	DbEntityMap_t::iterator db_it = db_entity_map.find(guid);
	if (db_it != db_entity_map.end())
	{
		DbEntityInfo * db_info = db_it->second;
		if (EOT_ADD == db_info->op_type)
		{
			return true;
		}
	}

	return false;
}

void PoolImp::checkShouldRemovedEntity(uint64 guid)
{
	EntityMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		EntityInfo * entity_info = it->second;
		if (entity_info->is_remove_from_memory)
		{
			delete entity_info;
			m_entity_map.erase(it);
		}
	}
}

void PoolImp::directUpdateToDb(GOOGLE_MESSAGE_TYPE * message, uint64 guid, uint64 owner_guid)
{
	m_transaction_id = ++m_transaction_id % numeric_limits<uint32>::max();
	m_transaction_id = 0 == m_transaction_id ? 1 : m_transaction_id;

	++m_request_id;

	protocol::db_data_request db_request;

	db_request.set_transaction_index(m_transaction_id);
	db_request.set_transaction_num(1);
	db_request.set_transaction_id(1);
	db_request.set_line_id(m_pool_cfg.line_id);
	db_request.set_data_stream(empty_string);
	db_request.set_owner_guid(owner_guid);

	db_request.set_data_stream(message->SerializeAsString());
	Packet * packet = new Packet(DCMSG_NEW_ENEITY, guid, db_request.SerializeAsString());

	m_pool_cfg.handle_output->cacheOutput(packet, m_pool_cfg.map_id, m_request_id, owner_guid);
}