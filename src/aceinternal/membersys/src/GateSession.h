

#ifndef GATE_SESSION_HPP
#define GATE_SESSION_HPP

#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Message_Block.h>
#include "Packet.h"

class GateHandleInput
{
public:
	virtual void input(Packet * packet) = 0;
};


class GateSession : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	GateSession();
	~GateSession();
public:
	virtual int open(void * p=0);

	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_output(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);
public:
	void setHandleInput(GateHandleInput * handle_input);

	int processRead();

	int processWrite(PacketQue_t & output_packet_que);
protected:
	int parseInputPacket();

private:
	GateHandleInput * m_handle_input;

	ACE_Time_Value m_read_timeout_value;

	ACE_Message_Block m_input_msg_block;

	ACE_Message_Block m_output_msg_block;
};

#endif