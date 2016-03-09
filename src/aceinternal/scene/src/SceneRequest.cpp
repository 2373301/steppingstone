
#include "opcode.h"
#include "SceneRequest.h"
#include "protocol_ins.h"
#include "entity_ins.h"

GuidRequestInfo::GuidRequestInfo(uint64 guid)
: req_guid(guid)
, req_type(DRT_BASE)
, req_back(false)
, req_sucess(false)
{}

void GuidRequestInfo::addMsg(uint64 guid, MSG_TYPE * msg)
{
	guid_msg_map.insert(std::make_pair(guid, msg));
	guid_vec.push_back(guid);
}

DataRequestInfo::DataRequestInfo(uint32 request_id, SceneRequest::FightDataRequestCallBack callback)
: req_id(request_id)
, req_back_num(0)
, fight_data_request_callback(callback)
, req_finish(false)
, req_success(false)
{}

DataRequestInfo::~DataRequestInfo()
{
	for (GuidRequestInfoMap_t::iterator it = guid_req_info_map.begin(); it != guid_req_info_map.end(); ++it)
	{
		delete it->second;
	}
}

void DataRequestInfo::addGuidRequest(GuidRequestInfo * guid_req_info)
{
	guid_req_info_map.insert(std::make_pair(guid_req_info->req_guid, guid_req_info));
}

void DataRequestInfo::modifyGuidRequestTypeFromCache(uint64 guid)
{
	GuidRequestInfoMap_t::iterator it = guid_req_info_map.find(guid);
	if (it != guid_req_info_map.end())
	{
		it->second->req_type = DRT_FROM_CACHESYS;
	}
}

void DataRequestInfo::requestDataBack(uint64 player_guid, CollectMsgInfo * collect_msg_info)
{
	GuidRequestInfoMap_t::iterator gui_req_it = guid_req_info_map.find(player_guid);
	if (gui_req_it != guid_req_info_map.end())
	{
		GuidRequestInfo * guid_req_info = gui_req_it->second;
		guid_req_info->req_back = true;

		if (NULL != collect_msg_info)
		{
			guid_req_info->req_sucess = true;
			for (GuidMsgMap_t::iterator it = collect_msg_info->msg_map.begin(); it !=collect_msg_info->msg_map.end(); ++it)
			{
				guid_req_info->addMsg(it->first, it->second);
			}
		}
		else
		{
			guid_req_info->req_sucess = false;
		}
	}
	else
	{
		// error 
	}
}

void DataRequestInfo::requestDataBackFailed(uint64 player_guid)
{
	GuidRequestInfoMap_t::iterator gui_req_it = guid_req_info_map.find(player_guid);
	if (gui_req_it != guid_req_info_map.end())
	{
		GuidRequestInfo * guid_req_info = gui_req_it->second;
		guid_req_info->req_back = true;
		guid_req_info->req_sucess = false;
	}	
}

bool DataRequestInfo::queryFinish()
{
	req_finish = true;
	req_success = true;
	for (GuidRequestInfoMap_t::iterator it = guid_req_info_map.begin(); it != guid_req_info_map.end(); ++it)
	{
		GuidRequestInfo * guid_req_info = it->second;

		if (!guid_req_info->req_sucess)
		{
			req_success = false;
		}

		if (!guid_req_info->req_back)
		{
			req_finish = false;
			req_success = false;
			break;
		}

	}

	if (req_finish)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////

SceneRequestImp::SceneRequestImp()
: m_request_id_index(0)
{
}

SceneRequestImp::~SceneRequestImp()
{
	cleanMapSecondContent(m_data_request_id_info_map);
}

int SceneRequestImp::init(SceneRequestCfg scene_request_cfg)
{
	if ((NULL == scene_request_cfg.scene)
		|| (NULL == scene_request_cfg.line)
		|| (NULL == scene_request_cfg.pool)
		|| (NULL == scene_request_cfg.logger)
		)
	{
		DEF_LOG_ERROR("failed to init SceneRequestImp\n");
		return -1;
	}

	m_scene_request_cfg = scene_request_cfg;
	m_scene_request_assistent.init(m_scene_request_cfg, this);

	return 0;
}

uint32 SceneRequestImp::requestPlayerFightData(const Uint64Vec_t & player_guid, FightDataRequestCallBack fight_data_request_callback)
{
	if (player_guid.size() == 0)
	{
		SREQ_LOG_ERROR("the player guid size is zero");
		return 0;
	}

	Uint64Set_t player_set;

	// check player guid, must be player guid
	for (Uint64Vec_t::const_iterator it = player_guid.begin(); it != player_guid.end(); ++it)
	{
		if (!IS_PLAYER_GUID(*it))
		{
			SREQ_LOG_ERROR("request to load player fight data info, but the guid is not player guid, <%llu>, size is <%d>", *it, player_guid.size());
			return 0;
		}

		player_set.insert(*it);
	}

	if (player_set.size() != player_guid.size())
	{
		SREQ_LOG_ERROR("get repeated player guid while call requestPlayerFightData\n");
		return 0;
	}

	uint32 req_id = getRequestId();
	DataRequestInfo * data_req_info = new DataRequestInfo(req_id, fight_data_request_callback);

	for (Uint64Vec_t::const_iterator it = player_guid.begin(); it != player_guid.end(); ++it)
	{
		SREQ_LOG_DEBUG("start to request player fight data, guid <%llu>, req id is <%d>", *it, req_id);
	}

	if (!doRequestPlayerFightData(player_guid, data_req_info))
	{
		delete data_req_info;
		return 0;
	}

	addRequestInfo(req_id, data_req_info);
	
	complateDataRequest(data_req_info);
	return req_id;
}

void SceneRequestImp::playerFightDataBackFromScene(uint64 player_guid, bool is_success)
{
	SREQ_LOG_DEBUG("recv player fight data from scene <%llu>, is success <%d>", player_guid, is_success);
	// todo
	if (!is_success)
	{
		GuidRequestReferenceMap_t::iterator ref_it = m_guid_request_reference_map.find(player_guid);
		if (ref_it != m_guid_request_reference_map.end())
		{
			for (vector< DataRequestInfo *>::iterator it = ref_it->second.begin(); it != ref_it->second.end(); ++it)
			{
				DataRequestInfo * data_req_info = *it;
				data_req_info->modifyGuidRequestTypeFromCache(player_guid);
			}

			m_scene_request_assistent.requestLoadPlayerFightData(player_guid);
		}

		return ;
	}

	CollectMsgInfo * collect_msg_info = m_scene_request_cfg.line->getCollectMessage(player_guid);

	playerFightDataBack(player_guid, is_success, collect_msg_info);

}

void SceneRequestImp::playerFightDataBackFromCache(uint64 player_guid, bool load_success, CollectMsgInfo * collect_msg_info)
{
	// todo
	SREQ_LOG_DEBUG("recv player fight data from cache <%llu>, is success <%d>", player_guid, load_success);

	playerFightDataBack(player_guid, load_success, collect_msg_info);
}

void SceneRequestImp::playerFightDataBack(uint64 player_guid, bool is_success, CollectMsgInfo * collect_msg_info)
{
	std::auto_ptr<CollectMsgInfo> auto_cmi(collect_msg_info);

	GuidRequestReferenceMap_t::iterator ref_it = m_guid_request_reference_map.find(player_guid);
	if (ref_it != m_guid_request_reference_map.end())
	{
		if ((!is_success) || (NULL == collect_msg_info))
		{
			for (vector< DataRequestInfo *>::iterator it = ref_it->second.begin(); it != ref_it->second.end(); ++it)
			{
				DataRequestInfo * data_req_info = *it;
				data_req_info->requestDataBackFailed(player_guid);
			}
		}
		else
		{
			for (GuidMsgMap_t::iterator guid_msg_it = collect_msg_info->msg_map.begin(); guid_msg_it != collect_msg_info->msg_map.end(); ++guid_msg_it)
			{
				POOL_OP_ADD(m_scene_request_cfg.pool, guid_msg_it->first, guid_msg_it->second, false, collect_msg_info->player_guid);
			}

			for (vector< DataRequestInfo *>::iterator it = ref_it->second.begin(); it != ref_it->second.end(); ++it)
			{
				DataRequestInfo * data_req_info = *it;
				data_req_info->requestDataBack(player_guid, collect_msg_info);
				addReferenceNo(player_guid, collect_msg_info->guid_vec);
			}

			collect_msg_info->clear();
		}

		for (vector< DataRequestInfo *>::iterator it = ref_it->second.begin(); it != ref_it->second.end(); ++it)
		{
			DataRequestInfo * data_req_info = *it;
			complateDataRequest(data_req_info);
		}

		m_guid_request_reference_map.erase(ref_it);
	}
	else
	{
		SREQ_LOG_ERROR("failed to get player request reference, player <%llu>", player_guid);
	}
}

uint32 SceneRequestImp::getRequestId()
{
	uint32 result = m_request_id_index % 0xFFFFFF + 1;
	++m_request_id_index;
	return result;
}

bool SceneRequestImp::doRequestPlayerFightData(const Uint64Vec_t & player_guid, DataRequestInfo* data_req_info)
{
	GuidRequestInfo * guid_req_info = NULL;
	for (Uint64Vec_t::const_iterator it = player_guid.begin(); it != player_guid.end(); ++it)
	{
		uint64 guid = *it;
		guid_req_info = new GuidRequestInfo(guid);
		data_req_info->addGuidRequest(guid_req_info);

		// get from locate pool
		if (requestPlayerFightDataFromLocate(guid, guid_req_info))
		{
			addReferenceNo(guid_req_info);
			continue;
		}

		bool already_in_request = false;
		addGuidRequestReference(guid, data_req_info, already_in_request);
		if (already_in_request)
		{
			continue;
		}

		guid_req_info->req_type = DRT_FROM_CACHESYS;
		doRequestPlayerFightData(guid, guid_req_info);

	}
	return true;
}

void SceneRequestImp::doRequestPlayerFightData(uint64 guid, GuidRequestInfo * guid_req_info)
{
	TransferPlayerInfo * trans_player_info = NULL;
	uint32 scene_id = m_scene_request_cfg.line->getPlayerFightData(guid, &trans_player_info);
	if (0 == scene_id)
	{
		if (NULL != trans_player_info)
		{
			guid_req_info->req_type = DRT_FROM_OTHER_SCENE;
			for (GuidMsgMap_t::iterator it = trans_player_info->msg_map.begin(); it != trans_player_info->msg_map.end(); ++it)
			{
				guid_req_info->addMsg(it->first, it->second);
			}
			guid_req_info->req_back = true;
			guid_req_info->req_sucess = true;
			SREQ_LOG_DEBUG("get player fight data from line scene, player is jumpping map <%llu>", guid);

			delete trans_player_info;
			trans_player_info = NULL;
		}
		else
		{
			// load from cache
			guid_req_info->req_type = DRT_FROM_CACHESYS;
			m_scene_request_assistent.requestLoadPlayerFightData(guid);
			SREQ_LOG_DEBUG("load from cache while get player fight data <%llu>", guid);
		}
	}
	else
	{
		guid_req_info->req_type = DRT_FROM_OTHER_SCENE;
		PackInfo * pi = new PackInfo(LMSG_REQUEST_PLAYER_FIGHT_DATA, guid, NULL, 2);
		if (!m_scene_request_cfg.line->sendSceneMsg(scene_id, pi))
		{
			guid_req_info->req_back = true;
			guid_req_info->req_sucess = false;
		}

		SREQ_LOG_DEBUG("load from scene while get player fight data <%llu>", guid);
	}

}

bool SceneRequestImp::requestPlayerFightDataFromLocate(uint64 player_guid, GuidRequestInfo * guid_req_info)
{
	GET_PLAYER_FROM_POOL(m_scene_request_cfg.pool, player_guid, player);
	if (NULL == player)
	{
		return false;
	}

	SREQ_LOG_DEBUG("get player form local, player is <%llu>", player_guid);

	guid_req_info->addMsg(player_guid, player);
	for (int i = 0; i < player->roles_size(); ++i)
	{
		GET_ROLE_FROM_POOL(m_scene_request_cfg.pool, player->roles(i), role);
		if (NULL == role)
		{
			// todo error?
			SREQ_LOG_ERROR("get player fight from local, but did not fing the role <%llu>", player->roles(i));
			continue;
		}

		guid_req_info->addMsg(player->roles(i), role);
	}

	GET_FORM_FROM_POOL(m_scene_request_cfg.pool, player->form(), form);
	if (NULL != form)
	{
		guid_req_info->addMsg(player->form(), form);
	}
	else
	{
		SREQ_LOG_ERROR("get player fight from local, but did not fing the form <%llu>", player->form());
	}

	guid_req_info->req_back = true;
	guid_req_info->req_sucess = true;

	return true;
}

void SceneRequestImp::addReferenceNo(GuidRequestInfo * guid_req_info)
{
	addReferenceNo(guid_req_info->req_guid, guid_req_info->guid_vec);

	//for (GuidMsgMap_t::iterator it = guid_req_info->guid_msg_map.begin(); it != guid_req_info->guid_msg_map.end(); ++it)
	//{
	//	addReferenceNo(it->first, it->second, guid_req_info->req_guid);
	//}
}

void SceneRequestImp::addReferenceNo(uint64 guid, Uint64Vec_t & guid_vec)
{
	int v_size = guid_vec.size();
	GuidReferenceMap_t::iterator it = m_guid_reference_map.find(guid);
	if (it != m_guid_reference_map.end())
	{
		it->second.reference_no += 1;
	}
	else
	{
		m_guid_reference_map.insert(std::make_pair(guid, ReferenceInfo(guid, guid_vec)));
		//POOL_OP_ADD(m_scene_request_cfg.pool, guid, msg, false, owner_guid);
		SREQ_LOG_DEBUG("add entity refrence <%llu>", guid);
	}
}

void SceneRequestImp::reduceReferenceNo(GuidRequestInfo * guid_req_info)
{
	reduceReferenceNo(guid_req_info->req_guid);
}

void SceneRequestImp::reduceReferenceNo(uint64 guid)
{
	GuidReferenceMap_t::iterator it = m_guid_reference_map.find(guid);
	if (it != m_guid_reference_map.end())
	{
		it->second.reference_no -= 1;
		if (it->second.reference_no <= 0)
		{
			Uint64Vec_t & impack_guid = it->second.impack_guid;
			int v_size = impack_guid.size();
			for (Uint64Vec_t::iterator g_it = impack_guid.begin(); g_it != impack_guid.end(); ++g_it)
			{
				SREQ_LOG_DEBUG("start to remove guid from pool <%llu>", guid);
				POOL_OP_REMOVE(m_scene_request_cfg.pool, *g_it, false);
			}

			m_guid_reference_map.erase(it);
		}
	}
	else
	{
		// error
		SREQ_LOG_ERROR("reduce guid refence, but not find the guid <%llu>", guid);
	}
}

void SceneRequestImp::reduceReferenceNo(DataRequestInfo * data_req_info)
{
	for (GuidRequestInfoMap_t::iterator it = data_req_info->guid_req_info_map.begin(); it != data_req_info->guid_req_info_map.end(); ++it)
	{
		reduceReferenceNo(it->second);
	}
}

void SceneRequestImp::addRequestInfo(uint32 req_id, DataRequestInfo * data_req_info)
{
	m_data_request_id_info_map.insert(std::make_pair(req_id, data_req_info));
}

void SceneRequestImp::removeRequestInfo(uint32 req_id)
{
	DataRequestIdInfoMap_t::iterator req_it = m_data_request_id_info_map.find(req_id);
	if (req_it != m_data_request_id_info_map.end())
	{
		SREQ_LOG_DEBUG("start to remove req id <%d>", req_id);
		DataRequestInfo * data_req_info = req_it->second;
		delete data_req_info;
		m_data_request_id_info_map.erase(req_it);
	}
	else
	{
		// error
		SREQ_LOG_ERROR("failed to get request info by req id <%d>", req_id);
	}
}

void SceneRequestImp::addGuidRequestReference(uint64 guid, DataRequestInfo* data_req_info, bool & already_in_request)
{
	GuidRequestReferenceMap_t::iterator it = m_guid_request_reference_map.find(guid);
	if (it != m_guid_request_reference_map.end())
	{
		it->second.push_back(data_req_info);
		already_in_request = true;
		SREQ_LOG_DEBUG("get player is in request list, player is <%llu>, request list size is <%d>", guid, m_guid_request_reference_map.size());
	}
	else
	{
		already_in_request = false;
		m_guid_request_reference_map[guid].push_back(data_req_info);
	}
}

void SceneRequestImp::complateDataRequest(DataRequestInfo * data_req_info)
{
	bool is_finish = data_req_info->queryFinish();
	if (is_finish)
	{
		// todo
		SREQ_LOG_DEBUG("player fight data collect finish, req id is <%d>", data_req_info->req_id);

		uint64 start_time = 0;
		uint64 end_time = 0;
		ACE_OS::gettimeofday().to_usec(start_time);

		data_req_info->fight_data_request_callback(data_req_info->req_id, data_req_info->req_success);
		ACE_OS::gettimeofday().to_usec(end_time);

		uint64 time_dif = end_time - start_time;
		if (time_dif < 20 * 1000)
		{
			SREQ_LOG_DEBUG("line scene call back spend time is <%llu>", time_dif);
		}
		else
		{
			SREQ_LOG_ERROR("error, line scene call back spend time is <%llu>", time_dif);
		}
		reduceReferenceNo(data_req_info);
		removeRequestInfo(data_req_info->req_id);
	}
}

void SceneRequestImp::freeDataRequestInfo(vector< DataRequestInfo *> & data_req_inf_vec)
{
	for (vector< DataRequestInfo *>::iterator it = data_req_inf_vec.begin(); it != data_req_inf_vec.end(); ++it)
	{
		delete *it;
	}
}