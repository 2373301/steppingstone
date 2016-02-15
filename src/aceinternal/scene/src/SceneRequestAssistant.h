
#ifndef SCENE_REQUEST_ASSISTANT_HPP
#define SCENE_REQUEST_ASSISTANT_HPP

#include "typedef.h"
#include "Pool.h"
#include "Plugin.h"
#include "Line.h"

#define FUNCTION_LINE_TRACE		" fun<%s> line<%d>\n"

#define MK_SCENE_REQ	"[SREQ] "

#define SREQ_LOG_DEBUG(log_info, ...)		m_scene_request_cfg.logger->log(LL_DEBUG,   "%s" MK_SCENE_REQ log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define SREQ_LOG_INFO(log_info, ...)		m_scene_request_cfg.logger->log(LL_INFO,    "%s" MK_SCENE_REQ log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define SREQ_LOG_WARNING(log_info, ...)		m_scene_request_cfg.logger->log(LL_WARNING, "%s" MK_SCENE_REQ log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define SREQ_LOG_TRACE(log_info, ...)		m_scene_request_cfg.logger->log(LL_TRACE,   "%s" MK_SCENE_REQ log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define SREQ_LOG_ERROR(log_info, ...)		m_scene_request_cfg.logger->log(LL_ERROR,   "%s" MK_SCENE_REQ log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define SREQ_LOG_FATAL(log_info, ...)		m_scene_request_cfg.logger->log(LL_FATAL,   "%s" MK_SCENE_REQ log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)

#define SREQ_CHECK_NULL_POINT_WITH_LOG_RETURN(p, result)	\
	if (NULL == (p))	\
	{	\
	SREQ_LOG_ERROR("failed to check point <%s>, function is <%s>, line is <%d>\n", #p, __FUNCTION__, __LINE__);	\
	return result;	\
	}


class SceneRequest;

class SceneRequestAssistant
{
public:
	SceneRequestAssistant();
	~SceneRequestAssistant();
public:
	void init(SceneRequestCfg & scene_request_cfg, SceneRequest * scene_request);

	void requestLoadPlayerFightData(uint64 player_guid);

	void collectMsg(uint64 player_guid, uint64 guid, GOOGLE_MESSAGE_TYPE * msg);

	void collectMsgFinish(uint64 player_guid, bool collect_sucess);
public:
	void loadPlayerFinish(RequestList * request_list, uint64 player_guid);

	void loadPlayerRoleFinish(RequestList * request_list, uint64 player_guid);

	void loadPlayerRoleEquipmentFinish(RequestList * request_list, uint64 player_guid);
protected:
private:
	typedef define_unordered_map<uint64, CollectMsgInfo *> CollectMsgInfoMap_t;

	SceneRequest * m_scene_request;

	SceneRequestCfg m_scene_request_cfg;

	CollectMsgInfoMap_t m_collect_msg_info_map;
};
#endif