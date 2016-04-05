
#ifndef ROUTE_CACHE_PACKET_HPP
#define ROUTE_CACHE_PACKET_HPP

#include <ace/Task.h>
#include <boost/random.hpp>
#include "Packet.h"
//#include "Cache.h"
#include "SingletonInit.h"
#include "Singleton.h"
#include "ManageCacheSession.h"
#include "RouteGatePacket.h"

class RouteCachePacket : public ACE_Task<ACE_NULL_SYNCH>, public SingletonInit, public HandleInputx
{
public:
	RouteCachePacket();
	~RouteCachePacket();
public:
	static RouteCachePacket * instance()
	{
		return Singleton<RouteCachePacket>::instance();
	}
public:
	/// init
	virtual int init();

	/// fini, stop
	virtual int stop();

	/// wait for finish
	virtual int finit();
public:
	void input(Packet * packet);

public:
	typedef std::list<Packet *> PacketList_t;

	virtual int svc(void);

	int handleClientPacket(Packet * packet_stream);
protected:
	int doHandlePacket(Packet * packet_stream);

	int handleRoleMake(Packet * packet_stream);

	int handleRoleDelete(Packet * packet_stream);

	int handleLoadEntity(Packet * packet_stream);

	bool initPlayer(uint64 guid, const typed::protocol::cmsg_player_make & role_make, typed::entity::Player & player, uint64 first_role_guid);

	bool initRole(uint64 player_guid, const typed::protocol::cmsg_player_make & role_make, typed::entity::Role & role);

private:
	bool m_stop;

	///CacheSys m_node_manager;

	PacketList_t m_input_queue;

	ACE_Thread_Mutex m_input_mutex;

	boost::mt19937 m_rand_engineer;

	boost::uniform_int<>  m_rand_distribution;

	boost::variate_generator<boost::mt19937, boost::uniform_int<> > m_rand_generator;

	//CBuilder m_role_builder;

	ManageCacheSession m_manage_cache_session;

};

#endif