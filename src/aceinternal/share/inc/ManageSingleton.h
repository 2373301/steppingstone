
#ifndef MANAGE_SINGLETON_HPP
#define MANAGE_SINGLETON_HPP

#include <ace/Thread_Mutex.h>
#include "typedef.h"
#include "Singleton.h"
#include "Share_def.h"

class SHARE_EXOPRT ManageSingleton
{
public:
	ManageSingleton();

	~ManageSingleton();

public:
	static ManageSingleton * instance();

public:
	void addSingleton(Cleanup * cleanup);

	void finit();

protected:
private:
	static ManageSingleton * m_instance;

	typedef list<Cleanup *> CleanupList_t;

	CleanupList_t m_signleton_list;

	ACE_Thread_Mutex m_signleton_list_mutex;
};
#endif