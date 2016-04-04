
#ifndef MANAGE_MEMBER_SESSION_OUTPUT_HPP
#define MANAGE_MEMBER_SESSION_OUTPUT_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include "typedef.h"
#include "Singleton.h"
#include "MemberSession.h"
#include "Packet.h"
#include "MemberProtocol.h"

class ManageMemberSessionOutput : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageMemberSessionOutput();
	~ManageMemberSessionOutput();
public:
	static ManageMemberSessionOutput * instance()
	{
		return Singleton<ManageMemberSessionOutput>::instance();
	}
public:
	int init();

	int svc();

	void handleOutputPacket(Packet * packet);

protected:
	void processPacket(Packet * packet);

private:
	PacketVec_t	m_output_packet_vec;

	ACE_Thread_Mutex	m_output_packet_vec_mutex;

	


};
#endif
