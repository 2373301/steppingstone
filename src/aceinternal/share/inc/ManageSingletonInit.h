
#ifndef MANAGE_SINGLETON_INIT_HPP
#define MANAGE_SINGLETON_INIT_HPP

#include "typedef.h"
#include "Singleton.h"
#include "SingletonInit.h"
#include "Share_def.h"

class SHARE_EXOPRT ManageSingletonInit
{
public:
	static ManageSingletonInit * instance()
	{
		return Singleton<ManageSingletonInit>::instance();
	}
public:
	void registerSingleton(SingletonInit * singletion_init);

	int init();

	int stop();

	int finit();
private:
	// Ctn = Container = list vector queue map
	typedef list<SingletonInit *> SingletonInitCtn_t;

	SingletonInitCtn_t m_singleton_ctn;
};
#endif