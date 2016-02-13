/// factory.h
/// ���ݿ⸨���๤����ͷ�ļ�
/// Eddy Zhang��2010/07/05

#ifndef __CACHE_SERVICE_FACTORY__
#define __CACHE_SERVICE_FACTORY__

#include "cached.h"
#include "cache_auto_assistant.h"

namespace cached {
namespace service {

/// �����࣬����guid������Ӧ�����ݿ⸨���������ָ����� 
class AssFactory
{
public:
    /// ��̬����������guid������Ӧ�����ݿ⸨���������ָ�����
    /// @param guid �����guid
    /// @param ca in/out���������������ݿ⸨���������ָ�����
    /// @return �޷���ֵ
	static dbass::CacheAssistant * create(CacheCGUID guid);
};

};  // namespace service
};  // namespace cached

#endif
