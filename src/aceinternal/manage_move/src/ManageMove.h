
#ifndef MANAGE_MOVE_HPP
#define MANAGE_MOVE_HPP

#include <boost/unordered_map.hpp>
#include "typedef.h"
#include "Plugin.h"
#include "Scene.h"
#include "opcode.h"
#include "ManageMove_def.h"
#include "protocol/msg_player.pb.h"


extern "C" MMOVE_EXOPRT IPlugin * make_plugin();

class MMOVE_EXOPRT ManageMove : public IPlugin
{
public:
	BEGING_INPUT_MSG_MAP()
		INPUT_HANDLE_MSG(CMSG_PLAYER_MOVE, &ManageMove::cmsg_player_move, new typed::protocol::cmsg_player_move)
	END_INPUT_MSG_MAP()

	BEGING_NOTIFY_MSG_MAP()
		NOTIFY_HANDLE_MSG(NMSG_PLAYER_IS_OFFLINE, &ManageMove::nmsg_player_is_offline)
		NOTIFY_HANDLE_MSG(NMSG_PLAYER_IS_ONLINE, &ManageMove::nmsg_player_is_online)
		NOTIFY_HANDLE_MSG(NMSG_PLAYER_JUMP_MAP, &ManageMove::nmsg_player_jump_map)
	END_NOTIFY_MSG_MAP()

public:
	ManageMove();

	~ManageMove();
public:
	virtual int IPlugin_init(const PluginCfg & plugin_cfg);

	//virtual int input(const PackInfo & pack_info);

	//virtual int notify(const PackInfo & pack_info);

	//virtual int request(const PackInfo & pack_info);
public:
	void update();
public:
	int cmsg_player_move(const PackInfo & pack_info);

public:
	int nmsg_player_is_offline(const PackInfo & pack_info);

	int nmsg_player_is_online(const PackInfo & pack_info);

	int nmsg_player_jump_map(const PackInfo & pack_info);

protected:
	virtual int IPlugin_initing();

	bool move(uint64 player_guid, bool & is_arrival);

	bool playerCanMove(uint64 player_guid);
private:
	long m_update_timer_id;

	typedef boost::unordered_map<uint64, bool>	MovePlayerMap_t;	// guid, arrive

	MovePlayerMap_t	m_move_player_map;

	Uint64Set_t m_in_stage_player;
};

#endif