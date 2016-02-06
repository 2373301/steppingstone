
#include <iostream>
#include "ManageSingletonInit.h"

void ManageSingletonInit::registerSingleton(SingletonInit * singletion_init)
{
	m_singleton_ctn.push_back(singletion_init);
}

int ManageSingletonInit::init()
{
	int index = 0;
	int r = 0;
	for (SingletonInitCtn_t::iterator it = m_singleton_ctn.begin(); it != m_singleton_ctn.end(); ++it, ++index)
	{
		r = (*it)->init();
		if (-1 == r)
		{
			std::cout << "failed to init index : " << index << std::endl;
			return -1;
		}
	}

	return 0;
}

int ManageSingletonInit::stop()
{
	int result = 0;
	for (SingletonInitCtn_t::reverse_iterator it = m_singleton_ctn.rbegin(); it != m_singleton_ctn.rend(); ++it)
	{
		if ((*it)->stop() == -1)
		{
			result = -1;
		}
	}

	return result;
}

int ManageSingletonInit::finit()
{
	int result = 0;
	for (SingletonInitCtn_t::reverse_iterator it = m_singleton_ctn.rbegin(); it != m_singleton_ctn.rend(); ++it)
	{
		if ((*it)->finit() == -1)
		{
			result = -1;
		}
	}

	return result;
}
