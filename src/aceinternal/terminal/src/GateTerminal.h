
#ifndef GATE_TERMINAL_HPP
#define GATE_TERMINAL_HPP

#include <boost/unordered_map.hpp>
#include <ace/Thread_Mutex.h>
#include "Terminal.h"
#include "PacketSerializeAssistant.h"
#include "typedef.h"

class GateTerminal 
	: public Terminal
	, public PacketProducer
{
public:
	GateTerminal();
	~GateTerminal();
public:
	virtual int session_on_read() override;

public:
	virtual void outputBuffer(uint32 msg_index, MsgBlockQue_t & msg_block_que);

	virtual uint32 getMessageIndex(uint32 increase_no);

public:
	virtual void IOutput_output(Packet * packet);

	virtual void outputPacket(Packet * packet);

	int session_sync_write();

	virtual void recvError(int recv_value, int last_error);
public:
	virtual int finit();

protected:
	virtual int initing();

	uint32 increaseNumber(uint32 src, uint32 increase_no);

	void geteLostConnection();

protected:
private:
	bool m_is_writing;

	uint32 m_msg_block_index;

	uint32 m_msg_block_pos;

	ACE_Message_Block * m_current_msg_block;

	PacketSerializeAssistant m_packet_serialize_assistant;

	typedef define_unordered_map<uint32, MsgBlockQue_t> MsgBlockQueMap_t;

	MsgBlockQueMap_t m_msg_block_que_map;

	ACE_Thread_Mutex m_msg_block_que_map_mutex;

	
	//ACE_Thread_Mutex m_output_packet_mutex;

	//PacketQue_t m_output_packet;

};
#endif
