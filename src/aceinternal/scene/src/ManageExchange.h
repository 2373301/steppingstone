
#ifndef MANAGE_EXCHANGE_HPP
#define MANAGE_EXCHANGE_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "typedef.h"
#include "Packet.h"
#include "Scene.h"

class ManageExchange : public HandleInputStream, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageExchange();
	~ManageExchange();
public:
	int svc();
public:
	int init(const ExchangeCfg & exchange_cfg);

	int stop();

	virtual void input(Session * session, ACE_Message_Block & msg_block);

	int handleOutputPacket(Packet * packet);

protected:
	void getOutputPacket(PacketQue_t & output_packet_que);

private:
	ExchangeCfg m_exchange_cfg;

	// default : 0, 1: connect, 2: connect failed
	int m_connect_exchange_status;

	PacketQue_t m_output_packet_que;

	ACE_Thread_Mutex m_output_packet_que_mutex;

	bool m_is_stop;
};
#endif