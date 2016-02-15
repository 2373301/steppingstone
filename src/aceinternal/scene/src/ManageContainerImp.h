
#ifndef MANAGE_CONTAINER_IMP_HPP
#define MANAGE_CONTAINER_IMP_HPP

#include <ace/Condition_Thread_Mutex.h>
#include <ace/Guard_T.h>
#include "typedef.h"
#include "coredef.h"
#include "Line.h"
#include "entity/container.pb.h"

class ManageContainerImp : public ManageContainer
{
public:
	ManageContainerImp();
	~ManageContainerImp();
public:
	virtual bool isRegister(EntityType et_type);
	virtual bool registerEntityType(EntityType et_type);
	virtual bool addGuid(EntityType et_type, uint64 entity_guid);
	virtual bool removeGuid(EntityType et_type, uint64 entity_guid);

	// call this function only once when startup
	// player of CollectMsgInfo is not used
	virtual MsgSet_t getEntityMessages(EntityType et_type);
	virtual Uint64Vec_t getContainerGuids(EntityType et_type);
	virtual bool isAddedGuid(EntityType et_type, uint64 guid);

	virtual bool addContainer(EntityType et_type, GOOGLE_MESSAGE_TYPE * ent_msg);
	virtual bool addContainerEntity(EntityType et_type, GOOGLE_MESSAGE_TYPE * ent_msg);
	virtual Uint64Set_t getUpdateGuids();
	virtual MsgSet_t getAddedContainer();
protected:
private:
	typedef define_unordered_map<EntityType, typed::entity::Container *> ContainerMap_t;

	typedef define_unordered_map<EntityType, vector<GOOGLE_MESSAGE_TYPE *> > ContainerEntityMap_t;

	typed::entity::ContainerContainer * m_container_container;

	ContainerMap_t m_container_map;

	ACE_Thread_Mutex m_container_mutex;

	MsgSet_t m_added_msg_set;

	Uint64Set_t m_updated_guid_set;

	ContainerEntityMap_t m_container_entity_map;
};

#endif