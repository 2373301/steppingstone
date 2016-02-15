
#ifndef PACKET_SERIALIZE_ASSISTANT_HPP
#define PACKET_SERIALIZE_ASSISTANT_HPP

#include <ace/Task.h>
#include "typedef.h"
#include "Packet.h"

typedef deque<ACE_Message_Block *> MsgBlockQue_t;

class PacketProducer
{
public:
	virtual void outputBuffer(uint32 msg_index, MsgBlockQue_t & msg_block_que) = 0;

	virtual uint32 getMessageIndex(uint32 increase_no) = 0;
};

class PacketSerializeAssistant : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	PacketSerializeAssistant();
	~PacketSerializeAssistant();
public:
	int init(int thread_no, PacketProducer * packet_producer, int msg_length = 102400);

	int stop();
public:
	int svc ();
public:
	void handlePacket(Packet * packet);

	void returnMsgBlock(ACE_Message_Block * msg_block);

protected:
	void serializePacket(uint32 msg_index, PacketVec_t & packet_vec);

	ACE_Message_Block * getMessageBlock();
private:
	int m_message_length;

	int m_message_no;

	PacketProducer * m_packet_producer;

	PacketVec_t m_packet_vec;

	ACE_Thread_Mutex m_packet_vec_mutex;

	MsgBlockQue_t m_message_block_que;

	ACE_Thread_Mutex m_message_block_que_mutex;

	bool m_is_stop;
};
#endif