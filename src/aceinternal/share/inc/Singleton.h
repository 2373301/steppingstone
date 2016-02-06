
#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include "Share_def.h"

#define NULL 0

// CClearup定义了单实例类模板类的基类，用于对象的自我清理操作
class SHARE_EXOPRT Cleanup
{
public:
	virtual ~Cleanup();
	///用于清理该对象所占用的资源
	virtual void cleanup();

	virtual void registerSingleton();
};


//Singleton定义了单实例类的模板类
template<class TYPE>
class Singleton : public Cleanup
{
public:
	virtual ~Singleton(void);
protected:
	Singleton(void);
public:
	///用于获取TYPE类型的单实例对象
	static TYPE *instance ();
protected:
	///TYPE类型对象
	TYPE  m_instance;

	///该类单实例对象指针
	static Singleton<TYPE> * m_singleton;

};

template<class TYPE>
Singleton<TYPE> * Singleton<TYPE>::m_singleton = NULL;

template<class TYPE>
Singleton<TYPE>::Singleton(void)
{
}

template<class TYPE>
Singleton<TYPE>::~Singleton(void)
{
	m_singleton = NULL;
}

template<class TYPE>
TYPE * Singleton<TYPE>::instance ()
{
	if (NULL == m_singleton)
	{
		if (NULL == m_singleton)
		{
			m_singleton = new Singleton();
			m_singleton->registerSingleton();
		}
	}
	return &m_singleton->m_instance;
}


#endif
