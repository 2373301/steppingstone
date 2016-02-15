
#ifndef PACKET_CONVERT_HPP
#define PACKET_CONVERT_HPP

#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#include "Packet.h"

class SceneImp;

class PacketConvert : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	PacketConvert(SceneImp * scene_imp);

	~PacketConvert();

public:
	virtual int svc();

	int stop();
public:
	void processPacket(Packet * packet);

protected:
private:
	SceneImp * m_scene_imp;

	PacketVec_t m_packet_vec;

	ACE_Thread_Mutex m_packet_vec_mutex;

	bool m_is_stop;
};
#endif