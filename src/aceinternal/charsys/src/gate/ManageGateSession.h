
#ifndef MANAGE_GATE_SESSION_HPP
#define MANAGE_GATE_SESSION_HPP

#include "NetStream.h"
#include "SingletonInit.h"
#include "Singleton.h"

class ManageGateSession : public ACE_Task<ACE_NULL_SYNCH>, public SingletonInit
{
public:
	ManageGateSession();
	~ManageGateSession();
public:
	static ManageGateSession * instance()
	{
		return Singleton<ManageGateSession>::instance();
	}
public:
	/// init
	virtual int init();

	/// fini, stop
	virtual int stop();

	/// wait for finish
	virtual int finit();

	virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act=0);

public:
	virtual int svc(void);

	void connectGate();

	void gateDisconnect();
protected:
private:
	ACE_Reactor * m_reactor;

	bool m_disconnect_gate;

	long m_timer_id;
};
#endif