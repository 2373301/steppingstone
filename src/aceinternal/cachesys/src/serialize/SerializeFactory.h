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
 * @brief ����Serializeʵ���Ĺ�����
 */
class SerializeFactory
{
public:
	SerializeFactory();

	~SerializeFactory();
public:
	/// ���������ļ������ã�����Serialize��ʵ��
	static Serialize * makeSerialize(const string & serialize_type);

protected:
private:
};

};
};
#endif