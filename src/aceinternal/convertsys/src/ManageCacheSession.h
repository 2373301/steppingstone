
#ifndef MANAGE_CACHE_SESSION_HPP
#define MANAGE_CACHE_SESSION_HPP

#include <ace/Reactor.h>
#include "Singleton.h"
#include "SingletonInit.h"
#include "CacheSession.h"
#include "ManageCacheOutput.h"
#include "ManageSQLiteFile.h"

class ManageCacheSession
	: public HandleInputx
	, public SingletonInit
	, public ACE_Task<ACE_NULL_SYNCH>
	, public SqliteOutput
{
public:
	ManageCacheSession();
	~ManageCacheSession();
public:
	static ManageCacheSession * instance()
	{
		return Singleton<ManageCacheSession>::instance();
	}
public:
	int svc();

public:
	virtual void input(Packet * packet);

	virtual void output(const string & addr, Packet * ps);

public:
	virtual int init();

	virtual int stop();

	virtual int finit();

protected:
	int connectCache();

	void handleSqliteFile(Packet * packet);

private:
	ACE_Reactor * m_reactor;

	map<string, CacheSession *> m_cache_map;

	ManageCacheOutput m_cache_output;
};

#endif