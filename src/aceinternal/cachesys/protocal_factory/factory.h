#ifndef __CACHE_SERVICE_FACTORY__
#define __CACHE_SERVICE_FACTORY__

#include "cached.h"
#include "cache_auto_assistant.h"
#include "Singleton.h"

namespace cached
{
namespace service
{

class CDynamicParse;
class AssFactory
{
public:
	dbass::CacheAssistant * create(CacheCGUID guid, const std::string& name);
	bool init(const std::string& proto_path);

	static AssFactory * instance()
	{
		return Singleton<AssFactory>::instance();
	}
private:
	CDynamicParse * m_parse = NULL;
};

}; 
};

#endif
