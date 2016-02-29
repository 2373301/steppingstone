
#ifndef SCENE_HPP
#define SCENE_HPP

#include <boost/array.hpp>
#include "Packet.h"
#include "Message.h"
#include "Session.h"
#include "LoadDll.h"
#include "Plugin.h"
#include "coredef.h"
#include "CacheInterface.h"
// #include "entity_ins.h"
// #include "entity/player.pb.h"
// #include "protocol_ins.h"
// #include "protocol/msg_player.pb.h"
// #include "ManageTerminal.h"

//todo 是否要 server 重合
#define  SRV_TYPE_CENTER	"center"
#define  SRV_TYPE_GAME		"game"
#define  SRV_TYPE_NAMING	"naming"
#define  SRV_GAME(x)	SRV_TYPE_GAME#x
#define  SRV_NAMING(x)	SRV_TYPE_NAMING#x

struct SCENEX_EXOPRT SceneCfg 
{
	SceneCfg()
		: scene_id(0)
		, map_id(0)
		, original_map_id(0)
		, line_no(0)
		, cache_type(0)
		//, manage_terminal(NULL)
		//, cache_handle_output(NULL)
		//, plugin_param_vec(NULL)
		, save_packet(false)
		, packet_dir("packet")
		, logger(NULL)
		//, line_scene(NULL)
		//, scene_request(NULL)
		, push_client_error_msg(false)
		, is_first_launch(false)
		, max_sceen_number(100)
		, log_dir("logs")
		, enable_gm(false)
	{}

	int scene_id;

	int line_no;

	uint64 map_id;

	uint32 original_map_id;

	int	cache_type;

	//ManageTerminal * manage_terminal;

	std::function<void(Packet * packet, uint64 map_id, uint64 request_id, uint64 owner_guid)> cache_handle_output;

	//PluginParamConfigVec_t * plugin_param_vec;

	bool save_packet;

	string packet_dir;

	Logger * logger;

// 	Line * line_scene;
// 
// 	SceneRequest * scene_request;

	bool push_client_error_msg;

	bool is_first_launch;

	int max_sceen_number;

	string log_dir;

	bool enable_gm;

	DllInfoVec_t plugin_dll_vec;
	std::string listen_addr = "127.0.0.1:1";
	
	std::string naming_addr = "127.0.0.1:2";
	std::string srv_type = SRV_TYPE_NAMING; //SRV_TYPE_GAME;
	std::string srv_id = SRV_NAMING(01); //SRV_GAME(01);
};

class SCENEX_EXOPRT Scene 
	: public HandleInput
	, public Timer
	, public Message
{
public:
	virtual ~Scene(){}

	virtual int init(const SceneCfg & scene_cfg) = 0;

	virtual int startup() = 0;
	virtual int stop() = 0;
	virtual int finit() = 0;

	virtual bool isStartupSuccess() = 0;
	virtual bool isShutdownSuccess() = 0;

	virtual bool get_guid(EntityType entity_type, uint64 & guid) = 0;
	virtual int get_random(int max_no, int min_no = 0) = 0;
};

SCENEX_EXOPRT Scene * createScene();

#endif
