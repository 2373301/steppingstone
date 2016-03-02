/// factory.cpp
/// 
/// 

#include "factory.h"
#include "coredef.h"
#include "Logger.h"
#include "dynamic_parse.h"

namespace cached
{
namespace service
{

dbass::CacheAssistant * AssFactory::create(CacheCGUID guid, const std::string& name)
{
	dbass::CacheAssistant * result = NULL;

	return result;
}

bool AssFactory::init(const std::string& proto_path)
{
	m_parse = new CDynamicParse();
	return m_parse->init(proto_path);
}

}; 
};

