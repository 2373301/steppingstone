
#ifndef GATE_SESSION_HPP
#define GATE_SESSION_HPP

#include "NetStream.h"

class GateSession : public NetStream
{
public:
	GateSession();
	~GateSession();
public:

	virtual int netConnected();
protected:

	virtual void netClosed(ACE_Reactor_Mask close_mask);

protected:
private:
};

#endif