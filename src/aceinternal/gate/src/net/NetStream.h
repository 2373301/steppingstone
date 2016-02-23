
#ifndef NET_STREAM_HPP
#define NET_STREAM_HPP

#include <list>
#include <ace/Svc_Handler.h>
#include <ace/Thread_Mutex.h>
#include <ace/Message_Block.h>
#include "PacketStream.h"

class HandleNetPacket
{
public:
	virtual int handlePacket(PacketStream * packet_stream) = 0;
};

struct NetState
{
	NetState()
	: connected(false)
	, closed(true)
	{}

	bool connected;
	bool closed;
};

class NetStream : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	NetStream();
	virtual ~NetStream();
public:
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;

	typedef std::list<PacketStream *> PacketStreamList;

	virtual int open(void * p=0);

	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_output(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);
public:
	void handleOutputStream(PacketStream * output_stream);

public:
	static void initSocketBufferLength(int buffer_length);

	virtual int netConnected();
protected:

	virtual void netClosed(ACE_Reactor_Mask close_mask);

protected:
	void parseInputPacket();

	int rd_stream();

	int wt_stream();

	void serializeToMsgBlock(PacketStream * output_stream, int already_read_byte);

protected:
	static int m_socket_buffer_length;

protected:
	HandleNetPacket * m_handle_net_packet;

	NetState m_net_state;

	bool m_cleaned;

	ACE_Message_Block m_input_msg_block;

	ACE_Message_Block m_output_msg_block;

	ACE_Thread_Mutex m_output_mutex;
};

#endif