
#ifndef MANAGE_TRANSCRIPT_STREAM_HPP
#define MANAGE_TRANSCRIPT_STREAM_HPP

#include <list>
#include <ace/Task.h>
#include "AppNotifyInterface.h"
#include "Singleton.h"
#include "GSSession.h"

class ManageTranscriptStream : public ACE_Task<ACE_NULL_SYNCH>, public AppNotifyInterface
{
public:
	ManageTranscriptStream();
	~ManageTranscriptStream();
public:
	static ManageTranscriptStream * instance()
	{
		return Singleton<ManageTranscriptStream>::instance();
	}
public:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	virtual int svc();
public:
	/// AppNotifyInterface init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// AppNotifyInterface fini, stop
	virtual int fini();

	/// AppNotifyInterface wait for finish
	virtual int wait();
public:
	bool handleTranscriptStream(GSSession * gs_session);

protected:
private:
	ACE_Reactor * m_reactor;

	bool m_stop;
};

#endif