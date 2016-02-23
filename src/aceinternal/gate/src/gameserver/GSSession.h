#ifndef _GS_SESSION_HPP
#define _GS_SESSION_HPP

#include <list>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include "PacketStream.h"

class GSSession : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	GSSession();
	virtual ~GSSession();
public:
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;

	typedef std::list<PacketStream *> PacketStreamList;

	virtual int open(void * p=0);

	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_output(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

public:
	void handleOutputStream(PacketStream * output_stream);

	void handleOutputStreamToGS(PacketStream * output_stream);

	void handleOutputStreamToTranscript(PacketStream * output_stream);

	void setMapID(uint64 map_id);

	vector<uint64> getMapID();

	void setTranscript(bool is_transcript);
public:
	int rd_stream();

	int wt_stream();

	const ACE_Time_Value & getHandleCloseTime();

	bool isShutdown();

	void cleanup();
protected:
	void parseInputPacket();

	void serializeToMsgBlock(PacketStream * output_stream, int already_read_byte);
protected:
	vector<uint64> m_map_id;

	//////////////////////////////////////////////////////////////////////////

	///是否发生错误，已关闭socket
	bool m_shutdown;

	bool m_sock_disconnection;

	ACE_Thread_Mutex m_handle_close_mutex;

	ACE_Time_Value m_handle_close_time;

	// not used
	//int m_recv_buffer_length;

	ACE_Message_Block m_input_msg_block;

	ACE_Thread_Mutex  m_input_msg_mutex;

	ACE_Message_Block m_output_msg_block;

	ACE_Thread_Mutex  m_output_msg_mutex;

	bool m_is_transcript;

	ACE_Thread_Mutex m_transcript_output_mutex;

};

#endif