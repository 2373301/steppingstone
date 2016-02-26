
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
#include <boost/lockfree/queue.hpp>

typedef std::function<int(const PackInfo & pack_info) > MsgHandler;
typedef std::map<uint64, MsgHandler> MsgHandlerMap;

#define SCENE_BEGIN_INPUT_MSG_MAP() \
	const MsgHandlerMap & getInputMsgMap()	\
	{	\
		if (m_input_msg_type_map.size() == 0)	\
		{

#define SCENE_INPUT_HANDLE_MSG(op_code, fun, msg_type_ins)	\
			m_input_msg_type_map[op_code] = std::bind(fun, this, std::placeholders::_1);	\
			m_message_type_map[op_code] = msg_type_ins;


#define SCENE_END_INPUT_MSG_MAP() \
		}	\
		return m_input_msg_type_map;	\
	}


class SCENEX_EXOPRT SceneImp
	: public Scene
	, public ACE_Task<ACE_NULL_SYNCH>
	, public netstream::HandleSessionEvent
{
public:
	SceneImp();
	~SceneImp();

	virtual int init(const SceneCfg & scene_cfg) override;

	virtual int startup() override;
	virtual int stop() override;
	virtual int finit() override;

	virtual bool isShutdownSuccess() override;
	virtual bool isStartupSuccess() override;


	virtual void input(Packet * packet) override;
	virtual void cacheInput(Packet * packet, uint64 map_id, uint64 request_id);

	virtual long schemeTimer(int interval_value, TimerCallBack timer_callback) override;
	virtual long cancelTimer(long timer_id) override;

	virtual bool get_guid(EntityType entity_type, uint64 & guid) override;
	virtual int get_random(int max_no, int min_no = 0) override;


	virtual void packInput(PackInfo * pack_info) override;

private:
	virtual int svc(void) override;
	int scene_svc(void);
	int connector_svc(void);

	virtual void playerMsg(Packet * packet) override;
	virtual void inlineBroadMsg(Packet * packet) override;
	virtual void notifyMsgToPlugins(const PackInfo & pack_info) override;
	virtual bool requestMsgToPlugins(const PackInfo & pack_info) override;
	virtual bool gmcmdMsgToPlugins(const string & gm_name, const vector<string> & gm_param, uint64 target_guid) override;

	void savePacket(Packet * packet);


	// handleSessionEvent
	virtual void newConnection(netstream::Session_t session, bool clientSide) override;
	virtual void connectionClosed(netstream::Session_t session, int trigger_source) override;
	virtual void handleInputStream(netstream::Session_t session, ACE_Message_Block & msg_block) override;


	SCENE_BEGIN_INPUT_MSG_MAP()
		SCENE_INPUT_HANDLE_MSG(SCENE_XS2NS_REQ_ONLINE_SCENES, &SceneImp::on_scene_xs2ns_req_online_scenes, new scene_xs2ns_req_online_scenes)
		SCENE_INPUT_HANDLE_MSG(SCENE_NS2XS_ACK_ONLINE_SCENES, &SceneImp::on_scene_ns2xs_ack_online_scenes, new scene_ns2xs_ack_online_scenes)
		SCENE_INPUT_HANDLE_MSG(SCENE_NS2XS_NTF_NEW_SCENES, &SceneImp::on_scene_ns2xs_ntf_new_scenes, new scene_ns2xs_ntf_new_scenes)
		SCENE_INPUT_HANDLE_MSG(SCENE_XS2XS_REQ_CONNECTION, &SceneImp::on_scene_xs2xs_req_connection, new scene_xs2xs_req_connection)
		SCENE_INPUT_HANDLE_MSG(SCENE_XS2XS_ACK_CONNECTION, &SceneImp::on_scene_xs2xs_ack_connection, new scene_xs2xs_ack_connection)
	SCENE_END_INPUT_MSG_MAP()

private:
	int on_scene_xs2ns_req_online_scenes(const PackInfo & pack_info);
	int on_scene_ns2xs_ack_online_scenes(const PackInfo & pack_info);
	int on_scene_ns2xs_ntf_new_scenes(const PackInfo & pack_info);
	int on_scene_xs2xs_req_connection(const PackInfo & pack_info);
	int on_scene_xs2xs_ack_connection(const PackInfo & pack_info);

private:
	
	PluginDepot * m_plugin_depot;

	uint32 m_save_packet_index;
	SceneCfg m_scene_cfg;

	// input packet
	ACE_Thread_Mutex	m_input_packet_vec_mutex;
	vector<PackInfo *>	m_input_packet_vec;

	// input cache packet
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

	netstream::SessionPool *m_session_pool;

	std::map<uint64, GOOGLE_MESSAGE_TYPE *> m_total_msg_map;
	std::map<uint64, MsgHandler> m_input_msg_type_map;
	std::map<uint64, GOOGLE_MESSAGE_TYPE *> m_message_type_map;

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

	boost::lockfree::queue<std::string *> m_to_be_connected;
};
#endif