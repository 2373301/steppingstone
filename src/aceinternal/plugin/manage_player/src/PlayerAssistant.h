
#ifndef SINGLE_TERMINAL_ASSISTANT_HPP
#define SINGLE_TERMINAL_ASSISTANT_HPP

#include "typedef.h"
#include <entity/player.pb.h>
#include "Pool.h"
#include "Plugin.h"
#include "ManagePlayer_def.h"

class ManagePlayer;

#define GET_ENTITY_FROM_PLD(entity_type, guid, entity_ins)	entity_type * entity_ins = (entity_type *)pld_ins->getEntity(guid);

#define GET_PLAYER_FROM_PLD(guid, entity_ins)			GET_ENTITY_FROM_PLD(typed::entity::Player, guid, entity_ins)

#define GET_ROLE_FROM_PLD(guid, entity_ins)					GET_ENTITY_FROM_PLD(typed::entity::Role, guid, entity_ins)

#define GET_TASKBRIEF_FROM_PLD(guid, entity_ins)			GET_ENTITY_FROM_PLD(typed::entity::TasksBrief, guid, entity_ins)

#define GET_CHAKRA_FROM_PLD(guid, entity_ins)				GET_ENTITY_FROM_PLD(typed::entity::Chakra, guid, entity_ins)

#define GET_WIFE_FROM_PLD(guid, entity_ins)				GET_ENTITY_FROM_PLD(typed::entity::Wife, guid, entity_ins)

class PlayerLoadingData
{
public:
	PlayerLoadingData();
	
	~PlayerLoadingData();

public:
	void setPlayerGuid(uint64 guid);

	bool addEntity(uint64 guid, MSG_TYPE * entity_msg);

	void addEntityToPool(Pool * pool);

	void clearEntity();

	MSG_TYPE * getEntity(uint64 guid);

protected:
private:
	typedef define_unordered_map<uint64, MSG_TYPE *> EntityMap_t;

	uint64 m_player_guid;

	EntityMap_t m_entity_map;

	STAT_PACKET_DEC();
};

class PlayerAssistant
{
public:
	PlayerAssistant(ManagePlayer * manage_player);
	
public:
	void load_player_role_equipment(RequestList * request_list, ManagePlayer * manage_player, uint64 player_guid);

	void load_player_role(RequestList * request_list, ManagePlayer * manage_player, uint64 player_guid);

	void load_player_from_db(RequestList * request_list, uint64 player_guid, ManagePlayer * manage_player);

	void load_player(typed::entity::Player * player);

	void load_player(typed::entity::Coin * coin);

	void flush_player(RequestList * request_list, uint64 player_guid, ManagePlayer * manage_player);

	//static void update_player(RequestList * request_list, uint64 player_guid, ManagePlayer * manage_player);

	//static void flush_player_data();

public:
	typedef define_unordered_map<uint64, MSG_TYPE *> EntityGuidMap_t;

	void init(PluginCfg & plugin_cfg);

	//void request_load_player(uint64 player_guid);
	void request_load_player(uint64 player_guid);

	void clearPlayerStatus(uint64 player_guid);

	//void request_flush_player(uint64 player_guid);

	void request_flush_player(uint64 player_guid, bool is_updating);

	bool initFirstEnterPlayer(uint64 player_guid);

	void clearEntityFlushState(uint64 player_guid);

	void getPlayerEntityMap(uint64 player_guid, EntityGuidMap_t & entity_guid_map, bool is_flush);

public:
	void initPlayer(uint64 guid, typed::entity::Player * role);

	PlayerLoadingData * getPlayerLoadingData(uint64 guid);

	void addPlayerLoadingData(uint64 guid);

	void removePlayerLoadingData(uint64 guid);

	void playerLoadFailed(uint64 guid, RequestList * request_list);

	void playerLoadSuccess(uint64 guid);

	//void initRoleLocaltion(uint64 guid, typed::entity::RoleLocaltion * role_localtion);

	//void initRoleItem(uint64 guid, typed::entity::RoleItem * role_item);

	//void initRoleSkill(uint64 guid, typed::entity::RoleSkill * role_skill);

	//void initRoleBuffer(uint64 guid, typed::entity::RoleBuffer * role_buffer);
private:

	ManagePlayer * m_manage_player;

	PluginCfg m_plugin_cfg;

	typedef define_unordered_map<uint64, PlayerLoadingData *> PlayerLoadingDataMap_t;

	PlayerLoadingDataMap_t m_player_loading_data_map;
};
#endif