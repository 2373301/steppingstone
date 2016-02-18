
#ifndef MANAGE_GRID_OPTIMIZE_HPP
#define MANAGE_GRID_OPTIMIZE_HPP

#include <ace/Task.h>
#include <ace/Guard_T.h>
#include <ace/Thread_Mutex.h>
#include <ace/Time_Value.h>
#include "Scene.h"

enum PlayerGridOpType
{
	PGOP_BASE	= 0,
	PGOP_ADD,
	PGOP_READD,
	PGOP_MOVE,
	PGOP_REMOVE,
	PGOP_LEVELUP,
	PGOP_BROADCAST,
	PGOP_FLOWERRANK,
	PGOP_UPDATE_GUILD_NAME,
	PGOP_UPDATE_FOLLOW_ROLE_ID,
	PGOP_UPDATE_TITLE,
	PGOP_UPDATE_SPECIAL_RIGHT,
	PGOP_UPDATE_HORSE,
	PGOP_UPDATE_WIFE_FOLLOW,
	PGOP_UPDATE_YELLOW_VIP_LEVEL,
};

struct PlayerGridInfo
{
	PlayerGridInfo();
	~PlayerGridInfo();

	uint64	guid;
	uint32	role_id;
	int32	level;
	int32	sex;
	int32	src_x;
	int32	src_y;
	int32	dst_x;
	int32	dst_y;
	int32	direction;
	uint64	first_role;
	bool	is_selected;
	uint32  flower_rank;
	string name;
	string guild_name;
	uint32 guild_icon;
	uint32 follow_role_id;
	Uint32Vec_t	title_vec;
	uint32 special_right;
	uint32 horse_id;
	uint32 wife_follow;
	uint32 yellow_vip_level;
	uint32 is_yellow_vip;
	uint32 is_yellow_year_vip;
	uint32 is_yellow_high_vip;
	STAT_PACKET_DEC();
};

struct PlayerMoveInfo
{
	PlayerMoveInfo();
	~PlayerMoveInfo();

	int32	src_x;
	int32	src_y;
	int32	dst_x;
	int32	dst_y;

	STAT_PACKET_DEC();
};

struct PlayerGridOperation
{
	PlayerGridOperation();

	~PlayerGridOperation();

	uint64			guid;
	int32			level;
	uint32			flower_rank;
	PlayerGridOpType player_grid_op;
	PlayerGridInfo * player_grid_info;
	PlayerMoveInfo * player_move_info;
	Packet *		 packet;
	string			 guild_name;
	uint32			 guild_icon;
	uint32			 follow_role_id;
	Uint32Vec_t		 title_vec;
	uint32			 special_right;
	uint32			 horse_id;
	uint32			 wife_follow;
	uint32			 yellow_vip_level;
	uint32			 is_yellow_vip;
	uint32			 is_yellow_year_vip;
	uint32			 is_yellow_high_vip;

	STAT_PACKET_DEC();
};

class ManageGridOptimize : public ManageGrid, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageGridOptimize();

	~ManageGridOptimize();
public:
	int init(SceneCfg * scene_cfg, Scene * scene);

	int stop();
public:
	int svc();

	void setMaxScreenNumber(int max_screen_number);

public:
	virtual void add(typed::entity::Player * player);

	virtual void readd(typed::entity::Player * player);

	virtual void move(typed::entity::Player * player);

	virtual void remove(typed::entity::Player * player);

	virtual void levelup(typed::entity::Player * player);

	virtual void flowerRank(typed::entity::Player * player);

	virtual void updateGuildName(typed::entity::Player * player);

	virtual void updateFollowRoleID(typed::entity::Player * player);

	virtual void updateTitle(typed::entity::Player * player);

	virtual void updateSpecialRight(typed::entity::Player * player);

	virtual void updateHorse(typed::entity::Player * player);

	virtual void updateWifeFollow(typed::entity::Player* player);

	virtual void updateYellowVipLevel(typed::entity::Player* player);

	virtual void broadcastPacket(Packet * packet);
protected:
	void putPlayerGridOp(PlayerGridOperation * player_grid_op);

	void fillPlayerGridInfo(typed::entity::Player * player, PlayerGridInfo * player_grid_info);

	void fillPlayerMoveInfo(typed::entity::Player * player, PlayerMoveInfo * player_move_info);
protected:
	void process(PlayerGridOperation & player_grid_op);

	void processAdd(PlayerGridOperation & player_grid_op);

	void processReadd(PlayerGridOperation & player_grid_op);

	void processMove(PlayerGridOperation & player_grid_op);

	void processRemove(PlayerGridOperation & player_grid_op);

	void processLevelup(PlayerGridOperation & player_grid_op);

	void processBroadcast(PlayerGridOperation & player_grid_op);

	void processFlowerRank(PlayerGridOperation & player_grid_op);

	void processUpdateGuildName(PlayerGridOperation & player_grid_op);

	void processFollowRoleID(PlayerGridOperation & player_grid_op);

	void processUpdateTitle(PlayerGridOperation & player_grid_op);

	void processSpecialRight(PlayerGridOperation & player_grid_op);

	void processHorse(PlayerGridOperation & player_grid_op);

	void processWifeFollow(PlayerGridOperation & player_grid_op);

	void processYellowVipLevel(PlayerGridOperation & player_grid_op);

	PlayerGridInfo * getPlayerGridInfo(uint64 guid);

	void removePlayerGridInfo(uint64 guid);

	void createOther(uint64 guid);

	void syncSelectedPlayer(PlayerGridInfo * player_grid_info);

	void fillSmsgCreateOther(PlayerGridInfo * player_grid_info, typed::protocol::smsg_create_other & sc_other);

	void playerMove(PlayerGridInfo * player_grid_info);

	void doBroadcastPacket(Packet * packet, bool for_selected_player);

	void playerRemove(uint64 guid);

	//bool isSelectedPlayer(uint64 guid);

	void addSelectedPlayer(PlayerGridInfo * player_grid_info);

	void removeSelectedPlayerMap(uint64 player_guid);
private:
	typedef define_unordered_map<uint64, PlayerGridInfo *> PlayerGridInfoMap_t;

	typedef vector<PlayerGridOperation *>	PlayerGridOperationVec_t;

	SceneCfg * m_scene_cfg;

	Scene * m_scene;

	int m_selected_player_num;

	PlayerGridInfoMap_t m_player_grid_info_map;

	PlayerGridOperationVec_t m_player_grid_op_vec;

	ACE_Thread_Mutex m_player_grid_op_vec_mutex;

	PlayerGridInfoMap_t		m_selected_player;

	Uint64Set_t		m_not_selected_player;

	bool m_is_stop;
};

#endif