#ifndef SERIALIZE_FACTORY_HPP
#define SERIALIZE_FACTORY_HPP

#include <ace/Singleton.h>
#include <ace/Recursive_Thread_Mutex.h>
#include "Serialize.h"

namespace cached {
namespace service {

/***
 * @class SerializeFactory
 *
 * @brief 创建Serialize实例的工厂类
 */
class SerializeFactory
{
public:
	SerializeFactory();

	~SerializeFactory();
public:
	/// 根据配置文件的配置，创建Serialize的实例
	static Serialize * makeSerialize(const string & serialize_type);

protected:
private:
};

};
};
#endif