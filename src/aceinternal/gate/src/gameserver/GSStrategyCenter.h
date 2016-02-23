
#ifndef _GS_STRATEGY_CENTER_HPP
#define _GS_STRATEGY_CENTER_HPP

#include <ace/Task.h>
#include "AppNotifyInterface.h"
#include "Singleton.h"
#include "GSSession.h"

class GSStrategyCenter : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	GSStrategyCenter();

	~GSStrategyCenter();
public:
	static GSStrategyCenter * instance()
	{
		return Singleton<GSStrategyCenter>::instance();
	}
public:
	virtual int svc(void);

public:
	/// AppNotifyInterface init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// AppNotifyInterface fini, stop
	virtual int fini();

	/// AppNotifyInterface wait for finish
	virtual int wait();
public:
	void GSClosedNotifyClient(GSSession * gs_session);

private:
	bool m_stop;

	bool m_wait;
};

#endif