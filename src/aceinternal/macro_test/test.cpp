
#include <ace/Log_Msg.h>


int main(int argc, ACE_TCHAR *argv[])
{
	
#if defined  ACE_HAS_NONSTATIC_OBJECT_MANAGER
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:ACE_HAS_NONSTATIC_OBJECT_MANAGER!\n")));
#endif

#if defined ACE_HAS_WCHAR
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:ACE_HAS_WCHAR!\n")));
#endif 

#if defined ACE_USES_WCHAR
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:ACE_USES_WCHAR!\n")));
#endif 

#if defined (_MSC_VER)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:_MSC_VER!\n")));
#endif 
    
    
#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:ACE_HAS_NONSTATIC_OBJECT_MANAGER!\n")));
#endif 

#if defined (ACE_HAS_TSS_EMULATION)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:ACE_HAS_TSS_EMULATION!\n")));
#endif 
    
 #if defined (ACE_NEEDS_FUNC_DEFINITIONS)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:ACE_NEEDS_FUNC_DEFINITIONS!\n")));
#endif   


#if defined (__GNUC__)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:__GNUC__!\n")));
#endif  

#if defined (__STDC__)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:__STDC__!\n")));
#endif 

#if defined (__C99__FUNC__)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:__C99__FUNC__!\n")));
#endif 

#if defined (__cplusplus)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:__cplusplus!\n")));
#endif 

#if defined (ACE_POSIX_AIOCB_PROACTOR)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:ACE_POSIX_AIOCB_PROACTOR!\n")));
#endif 

#if defined (ACE_POSIX_SIG_PROACTOR)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:ACE_POSIX_SIG_PROACTOR!\n")));
#endif 

#if defined (ACE_HAS_BROKEN_SIGEVENT_STRUCT)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:ACE_HAS_BROKEN_SIGEVENT_STRUCT!\n")));
#endif 

#if defined (ACE_HAS_POSIX_REALTIME_SIGNALS)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("has:ACE_HAS_POSIX_REALTIME_SIGNALS!\n")));
#endif 
      
    return 0;

}
