
#ifndef SINGLE_CONNECTION_HPP
#define SINGLE_CONNECTION_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "Packet.h"
#include "Session.h"
#include "netstream_def.h"

namespace netstream
{

class NETSTREAM_EXOPRT SingleConnection
	: public IStreamIn
	, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	SingleConnection();
	~SingleConnection();

	virtual int svc() override;

	virtual void IStreamIn_read(Session * session, ACE_Message_Block & msg_block) override;;

	virtual int init(const string & remote_addr);

	virtual int stop();

	virtual int IStreamOut_async_write(Packet * packet);

	virtual void setSocketBufferSize(int input_buf_size, int output_buf_size);
protected:
	virtual void handlePacket(Packet * packet) = 0;

	virtual void newConnection(PacketQue_t & output_packet_que);

	virtual void onSessionCloseNotify();
protected:
	bool connectToServer();

	int processOutputPacket(PacketQue_t & packet_que);

	void closeSession();
protected:
	int m_connect_status;

	netstream::Session * m_session;

	bool m_is_stop;

	int m_socket_input_buffer_size;

	int m_socket_output_buffer_size;

	string m_remote_addr;

	PacketQue_t m_packet_que;

	ACE_Recursive_Thread_Mutex m_packet_que_mutex;
};

}

#endif