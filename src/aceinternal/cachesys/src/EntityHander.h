
#ifndef ENTITY_HANDER_HPP
#define ENTITY_HANDER_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "Packet.h"
#include "DbLoader.h"
#include "Singleton.h"
#include "SingletonInit.h"

class HandleOutputx;

namespace cached 
{
namespace service 
{
class EntityHander 
	: public ACE_Task<ACE_NULL_SYNCH>
	, public SingletonInit
{
public:
	EntityHander();
	~EntityHander();

	static EntityHander * instance()
	{
		return Singleton<EntityHander>::instance();
	}

	int init();
	int stop();
	int finit();

	int svc();

	void input(Packet * packet);
	void setOutputHandle(HandleOutputx * handle_output);
	void handleDbLoadReplay(DataRequestInfo * data_request_info);
protected:
	void handlePacket(Packet * packet);

	void handleNewEntity(DataRequestInfo * data_request_info);

	void handleNewEntityWithFlush(DataRequestInfo * data_request_info);

	void handleLoadEntity(DataRequestInfo * data_request_info);

	void handleDbLoad(DataRequestInfo * data_request_info);

	void handleFlushEntity(DataRequestInfo * data_request_info);

	void handleUpdateEntity(DataRequestInfo * data_request_info);

	void handleRemoveEntity(DataRequestInfo * data_request_info);

	void handleLoadEntityOnce(DataRequestInfo * data_request_info);

	void handleOnlyRemoveEntity(DataRequestInfo * data_request_info);
protected:
	void replayNewEntity(DataRequestInfo * data_request_info);

	void replayNewEntityWithFlush(DataRequestInfo * data_request_info);

	void replayLoadEntity(DataRequestInfo * data_request_info);

	void replayFlushEntity(DataRequestInfo * data_request_info);

	void replayUpdateEntity(DataRequestInfo * data_request_info);

	void replayRemoveEntity(DataRequestInfo * data_request_info);

	void replayLoadEntityOnce(DataRequestInfo * data_request_info);

	void replayLoadEntityOnce(DataRequestInfo * data_request_info, ::google::protobuf::Message * entity_msg);

	void replayError(DataRequestInfo * data_request_info, ::protocol::OpResult_status_t error_code);
protected:
	bool addEntity(CacheAssistantx * entity_msg, uint64 guid);

	void removeEntity(uint64 guid);

	void updateEntity(CacheAssistantx * entity_msg, uint64 guid);

	CacheAssistantx * getEntity(uint64 guid);

	void outputPacket(Packet * ps);
private:
	typedef define_unordered_map<uint64, CacheAssistantx *>	CacheAssistantMap_t;

	typedef vector<DataRequestInfo *>	DataRequestInfoVec_t;

	HandleOutputx * m_handle_output;

	CacheAssistantMap_t m_entity_map;

	PacketVec_t m_packet_vec;

	ACE_Thread_Mutex m_packet_vec_mutex;

	DataRequestInfoVec_t m_db_load_reqplay_vec;

	ACE_Thread_Mutex m_db_load_reqplay_vec_mutex;
};

};

};
#endif