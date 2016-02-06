
#include <ace/Guard_T.h>
#include "ManageSingleton.h"

ManageSingleton * ManageSingleton::m_instance = NULL;

ManageSingleton::ManageSingleton()
{
}

ManageSingleton::~ManageSingleton()
{
	for (CleanupList_t::iterator it = m_signleton_list.begin(); it != m_signleton_list.end(); ++it)
	{
		(*it)->cleanup();
	}
}


ManageSingleton * ManageSingleton::instance()
{
	if (NULL == m_instance)
	{
		m_instance = new ManageSingleton();
	}
	return m_instance;
}

void ManageSingleton::addSingleton(Cleanup * cleanup)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_signleton_list_mutex, );
	m_signleton_list.push_front(cleanup);
}

void ManageSingleton::finit()
{
	if (NULL != m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}
