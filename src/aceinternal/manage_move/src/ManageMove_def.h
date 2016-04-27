
#ifndef MANAGE_MOVE_MACRO_HPP
#define MANAGE_MOVE_MACRO_HPP

#define MOVE_LOG_DEBUG(log_info, ...)		PLUGIN_LOG_DEBUG("%s" MK_MOVE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define MOVE_LOG_INFO(log_info, ...)		PLUGIN_LOG_INFO("%s" MK_MOVE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define MOVE_LOG_WARNING(log_info, ...)		PLUGIN_LOG_WARNING("%s" MK_MOVE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define MOVE_LOG_TRACE(log_info, ...)		PLUGIN_LOG_TRACE("%s" MK_MOVE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define MOVE_LOG_ERROR(log_info, ...)		PLUGIN_LOG_ERROR("%s" MK_MOVE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)
#define MOVE_LOG_FATAL(log_info, ...)		PLUGIN_LOG_FATAL("%s" MK_MOVE log_info FUNCTION_LINE_TRACE, "", ##__VA_ARGS__, __FUNCTION__, __LINE__)

#define ERROR_TO_PRINT_FUNCTION_LINE(str_info)	\
	MOVE_LOG_ERROR("occur error, function is <%s>, line is <%d>, info is <%s>\n", __FUNCTION__, __LINE__, str_info)

#define MOVE_TO_PRINT_FUNCTION_LINE(str_info)	\
	MOVE_LOG_DEBUG("error info, function is <%s>, line is <%d>, info is <%s>\n", __FUNCTION__, __LINE__, str_info)

#define MOVE_CHECK_NULL_POINT_WITH_LOG_RETURN(p, result)	\
	if (NULL == (p))	\
	{	\
	MOVE_LOG_ERROR("failed to check point <%s>, function is <%s>, line is <%d>\n", #p, __FUNCTION__, __LINE__);	\
	return result;	\
	}

#define MOVE_CHECK_NULL_POINT_WITH_LOG_CMD(p, command)	\
	if (NULL == (p))	\
	{	\
	MOVE_LOG_ERROR("failed to check point <%s>, function is <%s>, line is <%d>\n", #p, __FUNCTION__, __LINE__);	\
	command;	\
	}

#define MOVE_CHECK_RESULT_WITH_LOG_RETURN(res, result)	\
	if (-1 == (res))	\
	{	\
	MOVE_LOG_ERROR("failed to check result <%s>, function is <%s>, line is <%d>\n", #res, __FUNCTION__, __LINE__);	\
	return result;	\
	}

#define MOVE_CHECK_BOOLEAN_WITH_LOG_RETURN(res, result)	\
	if (!(res))	\
	{	\
	MOVE_LOG_ERROR("failed to check boolean <%s>, function is <%s>, line is <%d>\n", #res, __FUNCTION__, __LINE__);	\
	return result;	\
	}

#ifdef WIN32

	#ifdef MANAGE_MOVE_EXPORTS
	#define MMOVE_EXOPRT _declspec (dllexport)
	#else
	#define MMOVE_EXOPRT _declspec (dllimport)
	#endif

#else
	#define MMOVE_EXOPRT 
#endif

#endif