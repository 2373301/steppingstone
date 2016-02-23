
#ifndef _GateLOGGER_HPP
#define _GateLOGGER_HPP

#include <string>
#include <ace/Log_Msg.h>
#ifndef WIN32
#include <sys/types.h>
#endif

#include "Logger.h"

#define LM_DEBUG_PREFIX			ACE_TEXT("%D  LM_DEBUG     ")
#define LM_INFO_PREFIX			ACE_TEXT("%D  LM_INFO      ")
#define LM_NOTICE_PREFIX		ACE_TEXT("%D  LM_NOTICE    ")
#define LM_WARNING_PREFIX		ACE_TEXT("%D  LM_WARNING   ")
#define LM_ERROR_PREFIX			ACE_TEXT("%D  LM_ERROR     ")
#define LM_CRITICAL_PREFIX		ACE_TEXT("%D  LM_CRITICAL  ")
#define LM_ALERT_PREFIX			ACE_TEXT("%D  LM_ALERT     ")
#define LM_EMERGENCY_PREFIX		ACE_TEXT("%D  LM_EMERGENCY ")


#define GATE_LOG_DEBUG(log_info, ...)		DEF_LOG_DEBUG(log_info, ##__VA_ARGS__)	
#define GATE_LOG_INFO(log_info, ...)		DEF_LOG_INFO(log_info, ##__VA_ARGS__)		
#define GATE_LOG_WARNING(log_info, ...)		DEF_LOG_WARNING(log_info, ##__VA_ARGS__)	
#define GATE_LOG_TRACE(log_info, ...)		DEF_LOG_TRACE(log_info, ##__VA_ARGS__)	
#define GATE_LOG_ERROR(log_info, ...)		DEF_LOG_ERROR(log_info, ##__VA_ARGS__)	
#define GATE_LOG_FATAL(log_info, ...)		DEF_LOG_FATAL(log_info, ##__VA_ARGS__)	


//#define GATELogp(LM_LEVEL, s, ...)	ACE_ERROR((LM_LEVEL, LM_LEVEL##_PREFIX s, __VA_ARGS__))

//#define GATELog(LM_LEVEL, s, ...)	ACE_ERROR((LM_LEVEL, LM_LEVEL##_PREFIX s __VA_ARGS__))

#define FUNCTIONTRACE(s) FunctionExistTrace fun_trace(#s)

#define REPORT_THREAD_INFO(thread_name, thread_handle)	\
	GATE_LOG_INFO(ACE_TEXT("thred name is <%s>, handle is <%d>\n"), thread_name, thread_handle);

/***
 * @class FunctionExistTrace
 *
 * @brief 在进入函数和退出函数时记录log类
 */
class FunctionExistTrace
{
public:
	FunctionExistTrace(const std::string & trace_str)
	{
		m_trace_str = trace_str;
		GATE_LOG_INFO(ACE_TEXT("enter function %s\n"), m_trace_str.c_str());
	}
	~FunctionExistTrace()
	{
		GATE_LOG_INFO(ACE_TEXT("leave function %s\n"), m_trace_str.c_str());
	}
private:
	std::string m_trace_str;
};



#endif