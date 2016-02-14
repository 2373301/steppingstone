/// cache.h
/// 缓存服务头文件
/// Eddy Zhang，2010/07/05

#ifndef __CACHE_SERVICE_MAIN_HEADER__
#define __CACHE_SERVICE_MAIN_HEADER__

///
#include <map>
#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <vector>

/// 
#include <ace/ACE.h>
#include <ace/SOCK_Stream.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/Connector.h>
#include <ace/Reactor.h>
#include <ace/Select_Reactor.h>
#include <ace/Singleton.h>
#include <ace/Reactor_Notification_Strategy.h>
#include <ace/Time_Value.h>
#include <ace/Date_Time.h>
#include <ace/Log_Msg.h>

///
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/limits.hpp>
///
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>

#include <ace/OS.h>

#include "coredef.h"
#define define_unordered_map std::map
/// 
typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> CacheMTSvcHandler;
typedef ACE_Reactor_Notification_Strategy CacheRNS;
typedef ACE_Task<ACE_MT_SYNCH> CacheMTask;
typedef ACE_Message_Queue<ACE_MT_SYNCH> CacheMTQueue;
/// 
typedef ::std::vector< ::std::string > CacheStrings;
typedef ::std::map< ::std::string, ::std::string > CacheS2SMap;
typedef uint64 CacheCGUID;
typedef ::std::vector<CacheCGUID> CacheCGUIDS;
typedef define_unordered_map<ACE_HANDLE, CacheMTQueue*> CacheMTQueueMap;

#endif
