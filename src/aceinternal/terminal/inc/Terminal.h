
#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include "typedef.h"
#include "Packet.h"
#include "Session.h"

using namespace netstream;

struct LocalAddrCfg 
{
	LocalAddrCfg()
		: local_port(0)
	{}

	int local_port;
	string local_ip;
};

struct GateCfg 
{
	GateCfg()
		: type(false)
		, network_thread_no(1)
	{}

	typedef vector<string> GateVec_t;
	int type;
	int network_thread_no;
	GateVec_t gate_addrs;
};

struct TerminalCfg 
{
	TerminalCfg()
		: handle_input(NULL)
	{}

	HandleInput * handle_input;
	LocalAddrCfg local_addr;
	GateCfg gate_cfg;
};

class Terminal : public Session
{
public:
	virtual int init(const string & parameter, HandleInputStream * handle_input)
	{
		if (NULL == handle_input)
		{
			return false;
		}

		Session::setHandleInput(handle_input);
		m_parameter = parameter;
		return initing();
	}

	virtual int finit() = 0;

	virtual void outputPacket(Packet * packet) = 0;

protected:
	virtual int initing() = 0;

protected:
	string m_parameter;
};

#endif