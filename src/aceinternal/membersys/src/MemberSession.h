
#ifndef MEMBER_SESSION_HPP
#define MEMBER_SESSION_HPP

#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Message_Block.h>
#include "Packet.h"

class MemberHandleInput
{
public:
	virtual void input(Packet * packet) = 0;
};

#pragma pack(1)

struct MemberHead
{
	MemberHead()
		: opcode(0)
		, size(0)
	{}

	MemberHead(uint16 op, uint16 si)
		: opcode(op)
		, size(si)
	{}

	uint16	opcode;
	uint16	size;
};

#pragma pack(8)

class MemberSession : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	MemberSession();
	~MemberSession();
public:
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;

	virtual int open(void * p=0);

	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_output(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);
public:
	void setHandleInput(MemberHandleInput * handle_input);

	// no data return 1, lost connection return 2
	int processRead();

	// no data return 1, lost connection return 2
	int processWrite(PacketQue_t & output_packet_que);

	int processWrite(Packet * packet);
protected:
	int parseInputPacket();

private:
	MemberHandleInput * m_handle_input;

	ACE_Time_Value m_read_timeout_value;

	ACE_Message_Block m_input_msg_block;

	ACE_Message_Block m_output_msg_block;

	string m_remote_ip;
};
#endif
