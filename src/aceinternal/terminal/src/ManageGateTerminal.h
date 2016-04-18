
#ifndef MANAGE_GATE_TERMINAL_HPP
#define MANAGE_GATE_TERMINAL_HPP

#include <ace/Task.h>
#include <ace/Reactor.h>
#include "typedef.h"
#include "ManageTerminal.h"
#include "Terminal.h"
#include "PlayerRoute.h"
#include "ManageGateOutput.h"

class ManageGateTerminal 
	: public ManageTerminal 
	, public ISessionIn
	, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageGateTerminal();
	~ManageGateTerminal();
public:
	int svc();
public:
	virtual void ISessionIn_sync_read(Session * session, ACE_Message_Block & msg_block);

	virtual void output(Packet * packet);

	virtual void registerScene(uint64 scene_id, uint64 scene_type);

	//virtual void processOutputPacket(Packet * packet);
public:
	virtual Terminal * getTerminal(uint64 guid);

	Terminal * getTerminalByPointer(void * terminal);

	virtual void processPacketRoute(PacketRouteInfoQue_t * packet_route_info);

public:
	virtual int stop();

	virtual int finit();

protected:
	virtual int initing();

	int connectGate();

	bool addRoute(uint64 guid, void * terminal);

	void removeRoute(uint64 guid, void * terminal);

	void registerReactor(ACE_Reactor * reactor);

	void lostGateConnection();

	//Terminal * getTerminal(uint64 guid);
protected:
private:
	typedef vector<GateTerminal *> TerminalVec_t;

	typedef vector<ACE_Reactor *> ReactorVec_t;

	bool m_lost_gate_connection;

	TerminalVec_t m_terminal_vec;

	ACE_Thread_Mutex	m_terminal_map_mutex;

	TerminalMap_t m_terminal_map;

	ACE_Thread_Mutex m_reactor_vec_mutex;

	ReactorVec_t m_reactor_vec;

	PlayerRoute m_player_route;

	ManageGateOutput m_manage_gate_output;

	bool m_is_stop;
};
#endif

