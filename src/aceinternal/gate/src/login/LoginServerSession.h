#ifndef _LOGIN_SERVER_SESSION_HPP
#define _LOGIN_SERVER_SESSION_HPP

#include <ace/Event_Handler.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Svc_Handler.h>
#include <ace/Thread_Mutex.h>
#include "GateLogger.h"
#include "ManageClientBeat.h"
#include "protocol/msg_gate.pb.h"
#include "opcode.h"

class LoginServerSession;

class LogoutAccount;

typedef ACE_Acceptor<LoginServerSession, ACE_SOCK_ACCEPTOR> LoginAcceptor;

class LoginServerSession : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	LoginServerSession();
	~LoginServerSession();
public:
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;

	virtual int open(void * p=0);

	virtual int handle_input(ACE_HANDLE fd /* = ACE_INVALID_HANDLE */);

	virtual int handle_output(ACE_HANDLE fd /* = ACE_INVALID_HANDLE */);

	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

	typedef queue<PacketStream *> PacketStreamQue_t;

	int rd_stream();

	int processWrite(PacketStreamQue_t & output_packet_que);

protected:

	void parseInputPacket();

	void reportOnlineClient(ManageClientBeat::ClientSessionList & online_client, std::list<LogoutAccount> & logout_wait_from_gs);
private:

	ACE_Message_Block m_input_msg_block;

	ACE_Message_Block m_output_msg_block;

	//std::list<PacketStream *> m_output_packet_list;
};

#endif