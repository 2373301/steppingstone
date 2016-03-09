
#ifndef SCENE_REQUEST_HPP
#define SCENE_REQUEST_HPP

#include "Line.h"
#include "Pool.h"
#include "Scene.h"
#include "SceneRequestAssistant.h"

enum DataRequestType
{
	DRT_BASE				= 0,
	DRT_FROM_OTHER_SCENE,
	DRT_FROM_CACHESYS,
};

struct GuidRequestInfo
{
	GuidRequestInfo(uint64 guid);

	void addMsg(uint64 guid, MSG_TYPE * msg);

	uint64 req_guid;
	DataRequestType req_type;		// 1 : other scene, 2 : cache
	bool req_back;
	bool req_sucess;
	typedef define_unordered_map<uint64, MSG_TYPE *> GuidMsgMap_t;
	GuidMsgMap_t guid_msg_map;
	Uint64Vec_t guid_vec;
};

typedef define_unordered_map<uint64, GuidRequestInfo *>	GuidRequestInfoMap_t;

struct DataRequestInfo 
{
	DataRequestInfo(uint32 request_id, SceneRequest::FightDataRequestCallBack callback);

	~DataRequestInfo();

	void addGuidRequest(GuidRequestInfo * guid_req_info);

	void modifyGuidRequestTypeFromCache(uint64 guid);

	void requestDataBack(uint64 player_guid, CollectMsgInfo * collect_msg_info);

	void requestDataBackFailed(uint64 player_guid);

	bool queryFinish();

	uint32 req_id;
	uint32 req_back_num;
	bool   req_finish;
	bool   req_success;
	GuidRequestInfoMap_t guid_req_info_map;
	SceneRequest::FightDataRequestCallBack fight_data_request_callback;
};

struct ReferenceInfo 
{
	ReferenceInfo()
	: reference_no(0)
	, guid(0)
	{}

	ReferenceInfo(uint64 gv, Uint64Vec_t & impack_g)
		: reference_no(1)
		, guid(gv)
	{
		std::copy(impack_g.begin(), impack_g.end(), std::back_inserter(impack_guid));
	}

	int reference_no;
	uint64 guid;
	Uint64Vec_t impack_guid;
};

typedef define_unordered_map<uint64, DataRequestInfo *>	PlayerDataRequestInfoMap_t;

typedef define_unordered_map<uint32, DataRequestInfo *>	DataRequestIdInfoMap_t;

typedef define_unordered_map<uint64, int>	GuidReferenceMap_t;

class SCENE_EXOPRT SceneRequestImp : public SceneRequest
{
public:
	SceneRequestImp();

	~SceneRequestImp();
public:
	virtual int init(SceneRequestCfg scene_request_cfg);

	// about player fight player
	// return request id, it is failed of returning 0
	uint32 requestPlayerFightData(const Uint64Vec_t &  player_guid, FightDataRequestCallBack fight_data_request_callback);		

	void playerFightDataBackFromScene(uint64 player_guid, bool is_success);

	void playerFightDataBackFromCache(uint64 player_guid, bool load_success, CollectMsgInfo * collect_msg_info);

	void playerFightDataBack(uint64 player_guid, bool is_success, CollectMsgInfo * collect_msg_info);

protected:
	uint32 getRequestId();

	bool doRequestPlayerFightData(const Uint64Vec_t & player_guid, DataRequestInfo* data_req_info);

	void doRequestPlayerFightData(uint64 guid, GuidRequestInfo * guid_req_info);

	bool requestPlayerFightDataFromLocate(uint64 player_guid, GuidRequestInfo * guid_req_info);

	// guid reference manage
	void addReferenceNo(GuidRequestInfo * guid_req_info);

	void addReferenceNo(uint64 guid, Uint64Vec_t & guid_vec);

	void reduceReferenceNo(GuidRequestInfo * guid_req_info);

	void reduceReferenceNo(uint64 guid);

	void reduceReferenceNo(DataRequestInfo * data_req_info);

	// data request info 
	void addRequestInfo(uint32 req_id, DataRequestInfo * data_req_info);

	void removeRequestInfo(uint32 req_id);

	//
	void addGuidRequestReference(uint64 guid, DataRequestInfo* data_req_info, bool & already_in_request);

	void complateDataRequest(DataRequestInfo * data_req_info);

	void freeDataRequestInfo(vector< DataRequestInfo *> & data_req_inf_vec);

private:
	typedef define_unordered_map<uint64, ReferenceInfo>	GuidReferenceMap_t;

	typedef define_unordered_map<uint64, vector< DataRequestInfo *> > GuidRequestReferenceMap_t;

	uint32 m_request_id_index;

	SceneRequestCfg m_scene_request_cfg;

	SceneRequestAssistant m_scene_request_assistent;

	PlayerDataRequestInfoMap_t m_player_data_request_info_map;

	DataRequestIdInfoMap_t m_data_request_id_info_map;

	GuidReferenceMap_t m_guid_reference_map;

	GuidRequestReferenceMap_t m_guid_request_reference_map;
};

#endif