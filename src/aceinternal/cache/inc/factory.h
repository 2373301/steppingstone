#ifndef __CACHE_SERVICE_FACTORY__
#define __CACHE_SERVICE_FACTORY__

#include "cachehdr.h"
#include "cache_auto_assistant.h"

namespace cached 
{
namespace service 
{

class AssFactory
{
public:
	static dbass::CacheAssistant * create(CacheCGUID guid);
};

};  // namespace service

};  // namespace cached

#endif
