
#ifndef SCENE_IMP_HPP
#define SCENE_IMP_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "Scenex_def.h"
#include "Scene.h"
//#include "PluginDepot.h"
#include "ManageTimer.h"
//#include "ManageGridOptimize.h"
//#include "MakeGuid.h"
#include "ManageRandom.h"
#include "SessionPool.h"
#include "opcode.h"
#include "protocol/msg_scene.pb.h"
#include <thread>
#include <mutex>
#include <queue>
#include "Pool.h"
#include "SimpleXML.h"

typedef std::function<int(const PackInfo & pack_info) > MsgHandler;
typedef std::map<uint64, MsgHandler> MsgHandlerMap;

#define SCENE_BEGIN_INPUT_MSG_MAP() \
	const MsgHandlerMap & getInputMsgMap()	\
	{	\
		if (m_input_msg_type_map.size() == 0)	\
		{

#define SCENE_INPUT_HANDLE_MSG(op_code, fun, msg_type_ins)	\
			{ \
				MsgHandler it = std::bind(fun, this, std::placeholders::_1); \
				m_input_msg_type_map[op_code] = it; \
				m_message_type_map[op_code] = msg_type_ins; \
			}


#define SCENE_END_INPUT_MSG_MAP() \
		}	\
		return m_input_msg_type_map;	\
	}


class SCENEX_EXOPRT SceneImp
	: public IScene
	, public ACE_Task<ACE_NULL_SYNCH>
	, public netstream::ISessionPoolEvent
{
public:
	SceneImp();
	~SceneImp();

	virtual int IScene_init(const SceneCfg & scene_cfg) override;

	virtual int IScene_startup() override;
	virtual int IScene_stop() override;
	virtual int IScene_finit() override;

	virtual bool IScene_isShutdownSuccess() override;
	virtual bool IScene_isStartupSuccess() override;


	virtual void IInput_inputPacket(Packet * packet) override;
	virtual void cacheInput(Packet * packet, uint64 map_id, uint64 request_id);

	virtual long ITimer_scheme(int interval_value, TimerCallBack timer_callback) override;
	virtual long ITimer_cancel(long timer_id) override;

	virtual bool IScene_getGuid(EntityType entity_type, uint64 & guid) override;
	virtual int IScene_getRandom(int max_no, int min_no = 0) override;


	virtual void IMessage_packInput(PackInfo * pack_info) override;

private:
	virtual int svc(void) override;
	int scene_svc(void);
	int connector_svc(void);

	virtual void IMessage_player(Packet * packet) override;
	virtual void IMessage_inlineBroad(Packet * packet) override;
	virtual void IMessage_notifyToPlugins(const PackInfo & pack_info) override;
	virtual bool IMessage_requestToPlugins(const PackInfo & pack_info) override;
	virtual bool IMessage_gmcmdToPlugins(const string & gm_name, const vector<string> & gm_param, uint64 target_guid) override;

	void savePacket(Packet * packet);


	// handleSessionEvent
	virtual void ISessionPoolEvent_newConnection(netstream::Session_t session, bool clientSide) override;
	virtual void ISessionPoolEvent_connectionClosed(netstream::Session_t session, int trigger_source) override;
	virtual void ISessionPoolEvent_handleInputStream(netstream::Session_t session, ACE_Message_Block & msg_block) override;


	SCENE_BEGIN_INPUT_MSG_MAP()
		SCENE_INPUT_HANDLE_MSG(SCENE_XS2NS_REQ_ONLINE_SCENES, &SceneImp::on_scene_xs2ns_req_online_scenes, new scene_xs2ns_req_online_scenes)
		SCENE_INPUT_HANDLE_MSG(SCENE_NS2XS_ACK_ONLINE_SCENES, &SceneImp::on_scene_ns2xs_ack_online_scenes, new scene_ns2xs_ack_online_scenes)
		SCENE_INPUT_HANDLE_MSG(SCENE_NS2XS_NTF_NEW_SCENES, &SceneImp::on_scene_ns2xs_ntf_new_scenes, new scene_ns2xs_ntf_new_scenes)
		SCENE_INPUT_HANDLE_MSG(SCENE_XS2XS_REQ_CONNECTION, &SceneImp::on_scene_xs2xs_req_connection, new scene_xs2xs_req_connection)
		SCENE_INPUT_HANDLE_MSG(SCENE_XS2XS_ACK_CONNECTION, &SceneImp::on_scene_xs2xs_ack_connection, new scene_xs2xs_ack_connection)
		SCENE_INPUT_HANDLE_MSG(SCENE_XS2XS_NTF_SCENE_LOGOUT, &SceneImp::on_scene_xs2xs_ntf_scene_logout, NULL)
	SCENE_END_INPUT_MSG_MAP()

private:
	int on_scene_xs2ns_req_online_scenes(const PackInfo & pack_info);
	int on_scene_ns2xs_ack_online_scenes(const PackInfo & pack_info);
	int on_scene_ns2xs_ntf_new_scenes(const PackInfo & pack_info);
	int on_scene_xs2xs_req_connection(const PackInfo & pack_info);
	int on_scene_xs2xs_ack_connection(const PackInfo & pack_info);
	int on_scene_xs2xs_ntf_scene_logout(const PackInfo & pack_info);



	bool loadPlugin();

	bool loadPluginCfg();

	bool loadPluginCfg(const string & config_path, ParamConfig & param_config);

	bool loadPluginCfg(Element * root_element, ParamConfig & param_config);

private:
	
	IPluginDepot * m_plugin_depot = NULL;
	Pool *	m_pool = NULL;

	uint32 m_save_packet_index = 0;
	SceneCfg m_scene_cfg;

	// IInput_inputPacket packet
	ACE_Thread_Mutex	m_input_packet_vec_mutex;
	vector<PackInfo *>	m_input_packet_vec;

	// IInput_inputPacket cache packet
	ACE_Thread_Mutex	m_cache_input_packet_vec_mutex;
	//typedef vector<CachePackInfo> CachePackInfoVec_t;
	//CachePackInfoVec_t	m_cache_input_packet_vec;

	ManageTimer m_manage_timer;
	typedef map<long, TimerCallBack> TimerCallBackMap_t;
	TimerCallBackMap_t m_timer_callback_map;

	//ManageGridOptimize m_manage_grid;

	//PacketConvert m_packet_convert;

	//MakeGuid m_make_guid;

	ManageRandom m_manage_random;

	bool m_is_startup_success;
	bool m_is_stop;
	bool m_is_shutdown_success;

	netstream::ISessionPool *m_session_pool;

	std::map<uint64, MSG_TYPE *> m_total_msg_map;
	std::map<uint64, MsgHandler> m_input_msg_type_map;
	std::map<uint64, MSG_TYPE *> m_message_type_map;

	struct OnlineSceneST
	{
		std::string srv_type;
		std::string srv_id;
		netstream::Session_t session;
		std::string srv_addr;
	};

	std::map<std::string, OnlineSceneST> m_onlines;
	std::mutex m_connector_mutex;
	uint32 m_connector_thread_num = 1;
	uint32 m_cur_connector_thread_num = 0;

	std::deque<std::string *> m_to_be_connected;
	std::mutex m_to_be_connected_mutex;

	LoadDll m_load_dll;
	PluginParamConfigVec_t m_plugin_config_vec;
};
#endif