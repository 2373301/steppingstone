
#ifndef MANAGE_PLAYER_MACRO_HPP
#define MANAGE_PLAYER_MACRO_HPP

#define ERROR_TO_PRINT_FUNCTION_LINE(str_info)	\
	PLAYER_LOG_ERROR("occur error, function is <%s>, line is <%d>, info is <%s>\n", __FUNCTION__, __LINE__, str_info)

#define PLAYER_TO_PRINT_FUNCTION_LINE(str_info)	\
	PLAYER_LOG_DEBUG("error info, function is <%s>, line is <%d>, info is <%s>\n", __FUNCTION__, __LINE__, str_info)

#define PLAYER_CHECK_NULL_POINT_WITH_LOG_RETURN(p, result)	\
	if (NULL == (p))	\
	{	\
	PLAYER_LOG_ERROR("failed to check point <%s>, function is <%s>, line is <%d>\n", #p, __FUNCTION__, __LINE__);	\
	return result;	\
	}

#define PLAYER_CHECK_NULL_POINT_WITH_LOG_CMD(p, command)	\
	if (NULL == (p))	\
	{	\
	PLAYER_LOG_ERROR("failed to check point <%s>, function is <%s>, line is <%d>\n", #p, __FUNCTION__, __LINE__);	\
	command;	\
	}

#define PLAYER_CHECK_RESULT_WITH_LOG_RETURN(res, result)	\
	if (-1 == (res))	\
	{	\
	PLAYER_LOG_ERROR("failed to check result <%s>, function is <%s>, line is <%d>\n", #res, __FUNCTION__, __LINE__);	\
	return result;	\
	}

#define PLAYER_CHECK_BOOLEAN_WITH_LOG_RETURN(res, result)	\
	if (!(res))	\
	{	\
	PLAYER_LOG_ERROR("failed to check boolean <%s>, function is <%s>, line is <%d>\n", #res, __FUNCTION__, __LINE__);	\
	return result;	\
	}

#define PLAYER_CHECK_BOOLEAN_WITH_LOG_CMD(res, command)	\
	if (!(res))	\
	{	\
	PLAYER_LOG_ERROR("failed to check boolean <%s>, function is <%s>, line is <%d>\n", #res, __FUNCTION__, __LINE__);	\
	command;	\
	}

#ifdef WIN32

	#ifdef MANAGE_PLAYER_EXPORTS
	#define MPLAYER_EXOPRT _declspec (dllexport)
	#else
	#define MPLAYER_EXOPRT _declspec (dllimport)
	#endif

#else
	#define MPLAYER_EXOPRT 
#endif

#endif