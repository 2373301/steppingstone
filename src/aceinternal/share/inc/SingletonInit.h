
#ifndef SINGLETON_INIT_HPP
#define SINGLETON_INIT_HPP

#include "Share_def.h"

class SHARE_EXOPRT SingletonInit
{
public:
	SingletonInit();

	virtual ~SingletonInit();
public:
	virtual int init() = 0;

	virtual int stop();

	virtual int finit();
protected:
	bool m_stop;
	bool m_wait;
};

#endif