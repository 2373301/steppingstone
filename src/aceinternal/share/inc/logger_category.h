
#ifndef LOGGER_CATEGORY_HPP
#define LOGGER_CATEGORY_HPP

#define FUNCTION_LINE_TRACE		" fun<%s> line<%d>\n"

#define PLAYER_LOG_DEBUG(log_info, ...)		PLUGIN_LOG_DEBUG("%s" MK_PLAYER log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define PLAYER_LOG_INFO(log_info, ...)		PLUGIN_LOG_INFO("%s" MK_PLAYER log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define PLAYER_LOG_WARNING(log_info, ...)	PLUGIN_LOG_WARNING("%s" MK_PLAYER log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define PLAYER_LOG_TRACE(log_info, ...)		PLUGIN_LOG_TRACE("%s" MK_PLAYER log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define PLAYER_LOG_ERROR(log_info, ...)		PLUGIN_LOG_ERROR("%s" MK_PLAYER log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define PLAYER_LOG_FATAL(log_info, ...)		PLUGIN_LOG_FATAL("%s" MK_PLAYER log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)

//////////////////////////////////////////////////////////////////////////
///			module keywords define
//////////////////////////////////////////////////////////////////////////
#define MK_BAG		"[MK_BAG] "
#define MK_BUFFER	"[MK_BUFFER] "
#define MK_CHAR		"[MK_CHAR] "
#define MK_ENEMY	"[MK_ENEMY] "
#define MK_FORM		"[MK_FORM] "
#define MK_ITEM		"[MK_ITEM] "
#define MK_LAW		"[MK_LAW] "
#define MK_MOVE		"[MK_MOVE] "
#define MK_PARTNER	"[MK_PARTNER] "
#define MK_PLAYER	"[MK_PLAYER] "
#define MK_SECT		"[MK_SECT] "
#define MK_WAR		"[MK_WAR] "

//////////////////////////////////////////////////////////////////////////
///			client error keywords define, error code -100 -- -199
//////////////////////////////////////////////////////////////////////////
#define CE_NCHECK	"[CE_NCHECK]"			// 客户端未检查错误
#define CE_EPARAM	"[CE_EPARAM]"			// 客户端参数错误
#define CE_EVIL		"[CE_EVIL]"				// 客户端非法

enum ClientErrorDefine
{
	CE_BASE_ERROR		= -1,
	CE_NCHECK_CODE		= -101,
	CE_EPARAM_CODE,
	CE_EVIL_CODE,	
};


//////////////////////////////////////////////////////////////////////////
///			server error, error code -200 -- -300
//////////////////////////////////////////////////////////////////////////
#define SE_NGUID		"[SE_NGUID]"		// 找不到guid
#define SE_NRES			"[SE_NRES]"			// 找不到配置表
#define SE_ERES			"[SE_ERES]"			// 配置表错误

#define SE_EDATA		"[SE_EDATA]"		// 数据错误
#define SD_ILLEGAL		"[SD_ILLEGAL]"		// 数据不合法
#define SD_LOGIC		"[SD_LOGIC]"		// 逻辑错误
#define SD_INTEGRITY	"[SD_INTEGRITY]"	// 数据完整性错误

#define SE_EPLUGIN		"[SE_EPLUGIN]"		// 插件错误
#define SE_LINE_REQUEST "[SE_LINE_REQUEST]"	// line数据请求错误

#define SE_GMCMD_PARAM	"[SE_GMCMD_PARAM]"

enum ServerErrorDefine
{
	SE_BASE_ERROR		= -1,
	SE_NGUID_CODE		= -201,
	SE_NRES_CODE,
	SE_ERES_CODE,		

	SE_EDATA_CODE,
	SD_ILLEGAL_CODE,
	SD_LOGIC_CODE,
	SD_INTEGRITY_CODE,

	SE_EPLUGIN_CODE,
	SE_LINE_REQUEST_CODE,

	SE_GMCMD_PARAM_CODE,
};

#endif