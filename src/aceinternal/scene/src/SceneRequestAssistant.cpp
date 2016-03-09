
#include "SceneRequestAssistant.h"
#include "SceneRequest.h"

SceneRequestAssistant::SceneRequestAssistant()
: m_scene_request(NULL)
{
}

SceneRequestAssistant::~SceneRequestAssistant()
{
	cleanMapSecondContent(m_collect_msg_info_map);
}

void SceneRequestAssistant::init(SceneRequestCfg & scene_request_cfg, SceneRequest * scene_request)
{
	m_scene_request_cfg = scene_request_cfg;
	m_scene_request = scene_request;
}

void SceneRequestAssistant::requestLoadPlayerFightData(uint64 player_guid)
{
	DECLARE_REQUEST_LIST(m_scene_request_cfg.pool);
	LOAD_ONCE_REQUEST(player_guid, player_guid);

	POOL_OP_COMMIT(m_scene_request_cfg.pool, boost::bind(&SceneRequestAssistant::loadPlayerFinish, this, _1, player_guid));
}

void SceneRequestAssistant::collectMsg(uint64 player_guid, uint64 guid, MSG_TYPE * msg)
{
	CollectMsgInfoMap_t::iterator it = m_collect_msg_info_map.find(player_guid);
	if (it != m_collect_msg_info_map.end())
	{
		//it->second->msg_map.insert(std::make_pair(guid, msg));
		it->second->addMsg(guid, msg);
	}
	else
	{
		CollectMsgInfo * collect_msg_info = new CollectMsgInfo();
		collect_msg_info->player_guid = player_guid;
		//collect_msg_info->msg_map.insert(std::make_pair(guid, msg));
		collect_msg_info->addMsg(guid, msg);
		m_collect_msg_info_map.insert(std::make_pair(player_guid, collect_msg_info));
	}
}

void SceneRequestAssistant::collectMsgFinish(uint64 player_guid, bool collect_sucess)
{
	CollectMsgInfoMap_t::iterator it = m_collect_msg_info_map.find(player_guid);
	if (it != m_collect_msg_info_map.end())
	{
		m_scene_request->playerFightDataBackFromCache(player_guid, collect_sucess, it->second);
		m_collect_msg_info_map.erase(it);
	}
	else
	{
		SREQ_LOG_ERROR("player fight data load finish, but do not find the palyer info from collect msg map, player <%llu>", player_guid);
	}
}

void SceneRequestAssistant::loadPlayerFinish(RequestList * request_list, uint64 player_guid)
{
	if (request_list->is_success())
	{
		if (request_list->getRequestList().size() == 1)
		{
			// success
			Request * req = *request_list->getRequestList().begin();
			if (NULL != req->message)
			{
				this->collectMsg(player_guid, req->guid, req->message);

				typed::entity::Player * player = (typed::entity::Player *)req->message;

				DECLARE_REQUEST_LIST(this->m_scene_request_cfg.pool);
				for (int i = 0; i < player->roles_size(); ++i)
				{
					if (player->roles(i) < 10)
					{
						continue;
					}

					LOAD_ONCE_REQUEST(player->roles(i), player_guid);
				}

				LOAD_ONCE_REQUEST(player->form(), player_guid);

				if (player->player_wife() != 0)
				{
					LOAD_ONCE_REQUEST(player->player_wife(), player_guid);
				}

				for (int i = 0; i < player->wifes_size(); ++i)
				{
					if (player->wifes(i) > 10)
					{
						LOAD_ONCE_REQUEST(player->wifes(i), player_guid);
					}
				}

				if (player->runes() != 0)
				{
					LOAD_ONCE_REQUEST(player->runes(), player_guid);
				}

				if (player->dodge() != 0)
				{
					LOAD_ONCE_REQUEST(player->dodge(), player_guid);
				}

				POOL_OP_COMMIT(this->m_scene_request_cfg.pool, boost::bind(&SceneRequestAssistant::loadPlayerRoleFinish, this, _1, player_guid));
			}
			else
			{
				// todo
			}
		}
		else
		{
			// failed 
		}
	}
	else
	{
		// todo
	}
}

void SceneRequestAssistant::loadPlayerRoleFinish(RequestList * request_list, uint64 player_guid)
{
	if (request_list->is_success())
	{
		DECLARE_REQUEST_LIST(this->m_scene_request_cfg.pool);

		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			this->collectMsg(player_guid, req->guid, req->message);

			if (IS_ROLE_GUID(req->guid))
			{
				typed::entity::Role * role = (typed::entity::Role *)req->message;
				if (NULL != role)
				{
					for (int i = role->attributes_size(); i < RAT_ENDING; ++i)
					{
						role->add_attributes(0);
					}

					for (int i = 0; i < role->equipments_size(); ++i)
					{
						if (role->equipments(i) < 10)
						{
							continue;
						}

						LOAD_ONCE_REQUEST(role->equipments(i), player_guid);
					}
				}
			}
		}

		if (REQUEST_IS_EMPTY())
		{
			DELETE_REQUEST();
			this->collectMsgFinish(player_guid, true);
		}
		else
		{
			POOL_OP_COMMIT(this->m_scene_request_cfg.pool, boost::bind(&SceneRequestAssistant::loadPlayerRoleEquipmentFinish, this, _1, player_guid));
		}

		//this->collectMsgFinish(player_guid, true);
	}
	else
	{
		// todo
	}
}

void SceneRequestAssistant::loadPlayerRoleEquipmentFinish(RequestList * request_list, uint64 player_guid)
{
	if (request_list->is_success())
	{
		for (RequestList::RequestVec_t::const_iterator it = request_list->getRequestList().begin(); it !=request_list->getRequestList().end(); ++it)
		{
			Request * req = *it;
			this->collectMsg(player_guid, req->guid, req->message);
		}
		
		this->collectMsgFinish(player_guid, true);
	}
	else
	{
		// todo
	}
}