
#ifndef REMOTE_CACHE_HPP
#define REMOTE_CACHE_HPP

#include "typedef.h"
#include "Cache.h"

class RemoteCache : public Cache
{
public:
	RemoteCache();
	~RemoteCache();
public:
	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

public:
	virtual void output(Packet * packet);

	virtual int wt_stream();

	virtual void recvError(int recv_value, int last_error);
public:
	virtual int finit();

protected:
	virtual int initing();
protected:
private:
	bool m_is_writing;

	ACE_Thread_Mutex m_output_packet_mutex;

	PacketQue_t m_output_packet;
};
#endif