
#include "EntityHander.h"
#include "Serialize.h"
#include "ManageSerialize.h"
#include "Sessionx.h"
#include "ManageExpiredItem.h"
#include "config.h"

namespace cached {
	namespace service {

EntityHander::EntityHander()
: m_handle_output(NULL)
{

}
EntityHander::~EntityHander()
{
	for (PacketVec_t::iterator it = m_packet_vec.begin(); it != m_packet_vec.end(); ++it)
	{
		delete *it;
	}

	for (CacheAssistantMap_t::iterator it = m_entity_map.begin(); it != m_entity_map.end(); ++it)
	{
		delete it->second;
	}

	for (DataRequestInfoVec_t::iterator it = m_db_load_reqplay_vec.begin(); it != m_db_load_reqplay_vec.end(); ++it)
	{
		delete *it;
	}
}

int EntityHander::init()
{
	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

int EntityHander::stop()
{
	m_stop = true;
	return 0;
}

int EntityHander::finit()
{
	while(m_wait)
	{
		DEF_LOG_INFO("waiting for EntityHander\n");
		ACE_OS::sleep(1);
	}

	return 0;
}

int EntityHander::svc()
{
	PacketVec_t packet_vec;
	DataRequestInfoVec_t db_load_reqplay_vec;
	ACE_Time_Value sleep_time(0, 1 * 1000);
	while (!m_stop)
	{
		packet_vec.clear();
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_vec_mutex, -1);
			std::copy(m_packet_vec.begin(), m_packet_vec.end(), std::back_inserter(packet_vec));
			m_packet_vec.clear();
		}

		db_load_reqplay_vec.clear();
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_db_load_reqplay_vec_mutex, -1);
			std::copy(m_db_load_reqplay_vec.begin(), m_db_load_reqplay_vec.end(), std::back_inserter(db_load_reqplay_vec));
			m_db_load_reqplay_vec.clear();
		}

		if ((packet_vec.size() == 0) && (db_load_reqplay_vec.size() == 0))
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		for (PacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
		{
			handlePacket(*it);
		}

		for (DataRequestInfoVec_t::iterator it = db_load_reqplay_vec.begin(); it != db_load_reqplay_vec.end(); ++it)
		{
			handleDbLoad(*it);
		}
	}

	m_wait = false;

	return 0;
}

void EntityHander::input(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_vec_mutex, );
	m_packet_vec.push_back(packet);
}

void EntityHander::setOutputHandle(HandleOutputx * handle_output)
{
	m_handle_output = handle_output;
}

void EntityHander::handleDbLoadReplay(DataRequestInfo * data_request_info)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_db_load_reqplay_vec_mutex, );
	m_db_load_reqplay_vec.push_back(data_request_info);
}

void EntityHander::handlePacket(Packet * packet)
{
	std::auto_ptr<Packet> pack_ptr(packet);

	DEF_LOG_DEBUG("Start to process entity in EntityHander *****, op is <%d>, size is <%d>, guid is <%llu>\n", packet->opcode(), packet->body_size(), packet->guid());

	protocol::db_data_request * data_request = new protocol::db_data_request();

	if (!PARSE_PACKET(packet, data_request))
	{
		DEF_LOG_ERROR("failed to parse db_data_request from stream in EntityHandler::handlePacket, guid is <%llu>, opcode is <%d>, body size is <%d>\n", packet->guid(), packet->opcode(), packet->body_size());
		delete data_request;
		data_request = NULL;
		return;
	}

	CacheAssistantx * entity_msg = NULL;

	if (data_request->data_stream().size() > 0)
	{
		entity_msg = CONTAINER->getParser()->create(packet->guid(), data_request->entity_name());
		if (NULL == entity_msg)
		{
			DEF_LOG_ERROR("failed to create entity by guid <%llu>, opcode is <%d>\n", packet->guid(), packet->opcode());
			return ;
		}

		if (!PARSE_PACKET(data_request->data_stream(), entity_msg->data()))
		{
			DEF_LOG_ERROR("failed to parse entity in Entityhandler::handlePacket, guid is <%llu>, opcode is <%d>\n", packet->guid(), packet->opcode());
			delete entity_msg;
			entity_msg = NULL;
			return ;
		}
	}

	int opcode = packet->opcode();

	DataRequestInfo * data_request_info = new DataRequestInfo(packet->opcode(), packet->guid(), data_request, entity_msg, packet->getOwner());

	if (DCMSG_NEW_ENEITY == opcode)
	{
		handleNewEntity(data_request_info);
	}
	else if (DCMSG_LOAD_ENTITY == opcode)
	{
		handleLoadEntity(data_request_info);
	}
	else if (DCMSG_FLUSH_ENTITY == opcode)
	{
		handleFlushEntity(data_request_info);
	}
	else if (DCMSG_UPDATE_ENTITY == opcode)
	{
		handleUpdateEntity(data_request_info);
	}
	else if (DCMSG_REMOVE_ENTITY == opcode)
	{
		handleRemoveEntity(data_request_info);
	}
	else if (DCMSG_LOAD_ENTITY_ONCE == opcode)
	{
		handleLoadEntityOnce(data_request_info);
	}
	else if (DCMSG_NEW_ENEITY_WITH_FLUSH == opcode)
	{
		handleNewEntityWithFlush(data_request_info);
	}
	else if (DCMSG_ONLY_REMOVE_ENTITY == opcode)
	{
		handleOnlyRemoveEntity(data_request_info);
	}
	else
	{
		DEF_LOG_ERROR("get unknown opcode in EntityHander::handlePacket <%d>\n", opcode);
	}
}

void EntityHander::handleNewEntity(DataRequestInfo * data_request_info)
{
	std::auto_ptr<DataRequestInfo> auto_p(data_request_info);

	bool adding_success = addEntity(data_request_info->entity_msg, data_request_info->guid);
	if (adding_success)
	{
		ManageSerialize::instance()->writeRecord(data_request_info, RT_INSERT);
	}

	replayNewEntity(data_request_info);

	data_request_info->entity_msg = NULL;
}

void EntityHander::handleNewEntityWithFlush(DataRequestInfo * data_request_info)
{
	std::auto_ptr<DataRequestInfo> auto_p(data_request_info);

	ManageExpiredItem::instance()->update(data_request_info->guid, data_request_info->entity_msg, data_request_info->last_modify_time);

	ManageSerialize::instance()->writeRecord(data_request_info, RT_INSERT_WITH_FLUSH);

	replayNewEntityWithFlush(data_request_info);

	data_request_info->entity_msg = NULL;
}

void EntityHander::handleLoadEntity(DataRequestInfo * data_request_info)
{
	std::auto_ptr<DataRequestInfo> auto_p(data_request_info);

	CacheAssistantx * entity_msg = getEntity(data_request_info->guid);
	if (NULL != entity_msg)
	{
		DEF_LOG_DEBUG("find entity in map, start to replay load entity\n");
		data_request_info->entity_msg = entity_msg;
		replayLoadEntity(data_request_info);
		data_request_info->entity_msg = NULL;
	}
	else
	{
		CacheAssistantx * entity_msg = NULL;
		if (ManageExpiredItem::instance()->query(data_request_info->guid, &entity_msg, true))
		{
			DEF_LOG_DEBUG("find entity in expire object, start to replay load entity\n");
			data_request_info->entity_msg = entity_msg;
			addEntity(entity_msg, data_request_info->guid);
			replayLoadEntity(data_request_info);
			data_request_info->entity_msg = NULL;
		}
		else
		{
			DEF_LOG_DEBUG("do not find entity in map, start to load from db\n");
			// todo
			auto_p.release();
			DbLoader::instance()->process(data_request_info);
		}
	}
}

void EntityHander::handleDbLoad(DataRequestInfo * data_request_info)
{
	std::auto_ptr<DataRequestInfo> auto_p(data_request_info);
	if (data_request_info->db_load_success)
	{
		if (DCMSG_LOAD_ENTITY == data_request_info->opcode)
		{
			addEntity(data_request_info->entity_msg, data_request_info->guid);
			replayLoadEntity(data_request_info);
			data_request_info->entity_msg = NULL;
		}
		else if (DCMSG_LOAD_ENTITY_ONCE == data_request_info->opcode)
		{
			replayLoadEntityOnce(data_request_info);
		}
		else
		{
			DEF_LOG_ERROR("get unknown opcode in EntityHandler::handleDbLoad, opcode is <%d>, guid is <%llu>\n", data_request_info->opcode, data_request_info->guid);
		}
	}
	else
	{
		DEF_LOG_ERROR("failed to load entity from db, opcode is <%d>, guid is <%llu>\n", data_request_info->opcode, data_request_info->guid);
		this->replayError(data_request_info, protocol::OpResult_status_t_STATE_ENTITY_NOT_FOUND);
	}
}

void EntityHander::handleFlushEntity(DataRequestInfo * data_request_info)
{
	std::auto_ptr<DataRequestInfo> auto_p(data_request_info);

	CacheAssistantx * entity_msg = getEntity(data_request_info->guid);
	if (NULL == entity_msg)
	{
		DEF_LOG_ERROR("error to get null pointer of entity while flush entity, guid is <%llu>\n", data_request_info->guid);
		this->replayError(data_request_info, protocol::OpResult_status_t_STATE_ENTITY_NOT_FOUND);
		return ;
	}

	ManageExpiredItem::instance()->update(data_request_info->guid, data_request_info->entity_msg, data_request_info->last_modify_time);
	ManageSerialize::instance()->writeRecord(data_request_info, RT_FLUSH);
	replayFlushEntity(data_request_info);
	removeEntity(data_request_info->guid);
	data_request_info->entity_msg = NULL;
}

void EntityHander::handleUpdateEntity(DataRequestInfo * data_request_info)
{
	std::auto_ptr<DataRequestInfo> auto_p(data_request_info);
	if (NULL == data_request_info->entity_msg)
	{
		DEF_LOG_ERROR("get null point of entity while update, guid is <%llu>\n", data_request_info->guid);
		this->replayError(data_request_info, protocol::OpResult_status_t_STATE_ENTITY_NOT_FOUND);
		return ;
	}

	ManageSerialize::instance()->writeRecord(data_request_info, RT_UPDATE);
	updateEntity(data_request_info->entity_msg, data_request_info->guid);
	replayUpdateEntity(data_request_info);
	data_request_info->entity_msg = NULL;
}

void EntityHander::handleRemoveEntity(DataRequestInfo * data_request_info)
{
	std::auto_ptr<DataRequestInfo> auto_p(data_request_info);

	ManageExpiredItem::instance()->updateToDelete(data_request_info->guid);

	ManageSerialize::instance()->writeRecord(data_request_info, RT_REMOVE);
	removeEntity(data_request_info->guid);
	replayRemoveEntity(data_request_info);
}

void EntityHander::handleLoadEntityOnce(DataRequestInfo * data_request_info)
{
	std::auto_ptr<DataRequestInfo> auto_p(data_request_info);

	CacheAssistantx * entity_msg = getEntity(data_request_info->guid);
	if (NULL != entity_msg)
	{
		data_request_info->entity_msg = entity_msg;
		replayLoadEntityOnce(data_request_info);
		data_request_info->entity_msg = NULL;
	}
	else
	{
		::google::protobuf::Message * entity_msg = NULL;
		if (ManageExpiredItem::instance()->query(data_request_info->guid, &entity_msg))
		{
			replayLoadEntityOnce(data_request_info, entity_msg);
			delete entity_msg;
		}
		else
		{
			// todo
			auto_p.release();
			DbLoader::instance()->process(data_request_info);
		}
	}

	// all load once message, load entity from db, for data integrality   
	//std::auto_ptr<DataRequestInfo> auto_p(data_request_info);

	//auto_p.release();
	//DbLoader::instance()->process(data_request_info);
}

void EntityHander::handleOnlyRemoveEntity(DataRequestInfo * data_request_info)
{
	std::auto_ptr<DataRequestInfo> auto_p(data_request_info);

	DEF_LOG_INFO("start to only remove entity <%llu>\n", data_request_info->guid);
	removeEntity(data_request_info->guid);
}

void EntityHander::replayNewEntity(DataRequestInfo * data_request_info)
{
	protocol::OpResult op_result;
	op_result.set_status(protocol::OpResult_status_t_STATE_NORMAL);
	MAKE_NEW_PACKET(ps, data_request_info->opcode, data_request_info->guid, op_result.SerializeAsString());
	ps->setOwner(data_request_info->packet_owner);
	
	outputPacket(ps);
}

void EntityHander::replayNewEntityWithFlush(DataRequestInfo * data_request_info)
{
	protocol::OpResult op_result;
	op_result.set_status(protocol::OpResult_status_t_STATE_NORMAL);
	MAKE_NEW_PACKET(ps, data_request_info->opcode, data_request_info->guid, op_result.SerializeAsString());
	ps->setOwner(data_request_info->packet_owner);

	outputPacket(ps);
}

void EntityHander::replayLoadEntity(DataRequestInfo * data_request_info)
{
	if (NULL == data_request_info->entity_msg)
	{
		DEF_LOG_ERROR("get null pointer of entity while replay load entity, guid <%llu>\n", data_request_info->guid);
		return;
	}

	DEF_LOG_DEBUG("start to replay load entity <%llu>\n", data_request_info->guid);

	protocol::OpResult op_result;
	op_result.set_status(protocol::OpResult_status_t_STATE_NORMAL);
	op_result.set_message(data_request_info->entity_msg->data()->SerializeAsString());
	MAKE_NEW_PACKET(ps, data_request_info->opcode, data_request_info->guid, op_result.SerializeAsString());
	ps->setOwner(data_request_info->packet_owner);
	outputPacket(ps);
}

void EntityHander::replayFlushEntity(DataRequestInfo * data_request_info)
{
	protocol::OpResult op_result;
	op_result.set_status(protocol::OpResult_status_t_STATE_NORMAL);
	MAKE_NEW_PACKET(ps, data_request_info->opcode, data_request_info->guid, op_result.SerializeAsString());
	ps->setOwner(data_request_info->packet_owner);
	outputPacket(ps);
}

void EntityHander::replayUpdateEntity(DataRequestInfo * data_request_info)
{
	protocol::OpResult op_result;
	op_result.set_status(protocol::OpResult_status_t_STATE_NORMAL);
	MAKE_NEW_PACKET(ps, data_request_info->opcode, data_request_info->guid, op_result.SerializeAsString());
	ps->setOwner(data_request_info->packet_owner);
	outputPacket(ps);
}

void EntityHander::replayRemoveEntity(DataRequestInfo * data_request_info)
{
	protocol::OpResult op_result;
	op_result.set_status(protocol::OpResult_status_t_STATE_NORMAL);
	MAKE_NEW_PACKET(ps, data_request_info->opcode, data_request_info->guid, op_result.SerializeAsString());
	ps->setOwner(data_request_info->packet_owner);
	outputPacket(ps);
}

void EntityHander::replayLoadEntityOnce(DataRequestInfo * data_request_info)
{
	if (NULL == data_request_info->entity_msg)
	{
		DEF_LOG_ERROR("get null pointer of entity while replay load entity once, guid <%llu>\n", data_request_info->guid);
		return;
	}

	protocol::OpResult op_result;
	op_result.set_status(protocol::OpResult_status_t_STATE_NORMAL);
	op_result.set_message(data_request_info->entity_msg->data()->SerializeAsString());
	MAKE_NEW_PACKET(ps, data_request_info->opcode, data_request_info->guid, op_result.SerializeAsString());
	ps->setOwner(data_request_info->packet_owner);
	outputPacket(ps);
}

void EntityHander::replayLoadEntityOnce(DataRequestInfo * data_request_info, ::google::protobuf::Message * entity_msg)
{
	if (NULL == entity_msg)
	{
		DEF_LOG_ERROR("get null pointer of entity while replay load entity once, guid <%llu>\n", data_request_info->guid);
		return;
	}

	protocol::OpResult op_result;
	op_result.set_status(protocol::OpResult_status_t_STATE_NORMAL);
	op_result.set_message(entity_msg->SerializeAsString());
	MAKE_NEW_PACKET(ps, data_request_info->opcode, data_request_info->guid, op_result.SerializeAsString());
	ps->setOwner(data_request_info->packet_owner);
	outputPacket(ps);
}

void EntityHander::replayError(DataRequestInfo * data_request_info, ::protocol::OpResult_status_t error_code)
{
	protocol::OpResult op_result;
	op_result.set_status(error_code);
	MAKE_NEW_PACKET(ps, data_request_info->opcode, data_request_info->guid, op_result.SerializeAsString());
	ps->setOwner(data_request_info->packet_owner);
	outputPacket(ps);
}

bool EntityHander::addEntity(CacheAssistantx * entity_msg, uint64 guid)
{
	if (NULL == entity_msg)
	{
		DEF_LOG_ERROR("error to add NULL point of entity, guid is <%llu>\n", guid);
		return false;
	}

	CacheAssistantMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		DEF_LOG_ERROR("failed to add entity with reduplicate guid <%llu>\n", guid);
		delete entity_msg;
		return false;
	}
	else
	{
		m_entity_map.insert(std::make_pair(guid, entity_msg));
		return true;
	}
}

void EntityHander::removeEntity(uint64 guid)
{
	CacheAssistantMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		delete it->second;
		m_entity_map.erase(it);
	}
	else
	{
		DEF_LOG_ERROR("failed to find entity while remve, guid is <%llu>\n", guid);
	}
}

void EntityHander::updateEntity(CacheAssistantx * entity_msg, uint64 guid)
{
	CacheAssistantMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		delete it->second;
		it->second = entity_msg;
	}
	else
	{
		DEF_LOG_ERROR("failed to find entity while update, guid is <%llu>\n", guid);
	}
}

CacheAssistantx * EntityHander::getEntity(uint64 guid)
{
	CacheAssistantMap_t::iterator it = m_entity_map.find(guid);
	if (it != m_entity_map.end())
	{
		return it->second;
	}

	return NULL;
}

void EntityHander::outputPacket(Packet * ps)
{
	if (NULL != m_handle_output)
	{
		DEF_LOG_DEBUG("start to replay, opcode is <%d>, guid is <%llu>\n", ps->opcode(), ps->guid());
		m_handle_output->output(ps);
	}
	else
	{
		DEF_LOG_ERROR("start to handle output package in entity handle, but the output handle is NULL\n");
	}
}

};
};