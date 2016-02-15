
#ifndef PLAYER_ROUTE_HPP
#define PLAYER_ROUTE_HPP

#include <ace/Thread_Mutex.h>
#include <ace/Task.h>
#include <boost/unordered_map.hpp>
#include "Packet.h"
#include "Terminal.h"

typedef map<uint64, Terminal *>	 TerminalMap_t;


class ManageGateTerminal;

class TerminalRoute
{
public:
	void addRoute(uint64 guid, void * terminal);

	void removeRoute(uint64 guid, void * terminal);

	Terminal * getRoute(uint64 guid);
private:
	TerminalMap_t m_terminal_map;

	ACE_Thread_Mutex m_terminal_map_mutex;
};

struct PacketRouteInfo 
{
	PacketRouteInfo(Packet * ps, Terminal * ter)
	: packet(ps)
	, terminal(ter)
	{}

	Packet * packet;
	Terminal * terminal;
};

typedef deque<PacketRouteInfo> PacketRouteInfoQue_t;



class PlayerRouteDispatch : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	PlayerRouteDispatch(ManageGateTerminal * manage_gt);
	~PlayerRouteDispatch();
public:
	int stop();

	virtual int svc();
public:
	virtual uint32 getRouteIndex();

	virtual void processPacketRoute(uint32 route_index, PacketRouteInfoQue_t * packet_route_info);


protected:
	uint32 increaseNumber(uint32 src, uint32 increase_no);
private:
	ManageGateTerminal * m_manage_gate_terminal;

	uint32 m_packet_route_index;

	uint32 m_packet_route_pos;

	typedef define_unordered_map<uint32, PacketRouteInfoQue_t *>	PacketRouteInfoQueMap_t;

	PacketRouteInfoQueMap_t m_packet_route_info_map;

	ACE_Thread_Mutex m_packet_route_info_map_mutex;

	bool m_is_stop;
};



class PlayerRoute : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	PlayerRoute(ManageGateTerminal * manage_gt);
	~PlayerRoute();
public:
	int init(int thread_no);

	int stop();
public:
	void input(Packet * packet);


	void addRoute(uint64 guid, void * terminal);

	void removeRoute(uint64 guid, void * terminal);
public:
	virtual int svc();
protected:
	void registerTerminalRoute(TerminalRoute * ter_route);

	Terminal * getRouteByOwner(void * owner);

private:
	ManageGateTerminal * m_manage_gate_terminal;

	PacketVec_t m_packet_vec;

	ACE_Thread_Mutex m_packet_vec_mutex;

	PlayerRouteDispatch m_player_route_dispatch;

	typedef deque<TerminalRoute *> TerminalRouteQue_t;

	TerminalRouteQue_t m_terminal_route_que;

	ACE_Thread_Mutex m_terminal_route_que_mutex;

	bool m_is_stop;
};
#endif
