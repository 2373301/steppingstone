/// factory.h
/// 数据库辅助类工厂类头文件
/// Eddy Zhang，2010/07/05

#ifndef __CACHE_SERVICE_FACTORY__
#define __CACHE_SERVICE_FACTORY__

#include "cached.h"
#include "cache_auto_assistant.h"

namespace cached {
namespace service {

/// 工厂类，根据guid产生相应的数据库辅助类的智能指针对象 
class AssFactory
{
public:
    /// 静态函数，根据guid产生相应的数据库辅助类的智能指针对象
    /// @param guid 对象的guid
    /// @param ca in/out参数，产生的数据库辅助类的智能指针对象
    /// @return 无返回值
	static dbass::CacheAssistant * create(CacheCGUID guid);
};

};  // namespace service
};  // namespace cached

#endif
