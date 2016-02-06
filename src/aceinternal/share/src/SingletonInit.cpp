
#include <iostream>
#include <ace/OS.h>
#include "SingletonInit.h"
#include "ManageSingletonInit.h"

SingletonInit::SingletonInit()
: m_stop(false)
, m_wait(true)
{
	ManageSingletonInit::instance()->registerSingleton(this);
}

SingletonInit::~SingletonInit()
{
}

int SingletonInit::stop()
{
	m_stop = true;
	return 0;
}

int SingletonInit::finit()
{
	while (m_wait)
	{
		//std::cout << "wait ......" << std::endl;
		ACE_OS::sleep(1);
	}

	//std::cout << "exit" << std::endl;

	return 0;
}