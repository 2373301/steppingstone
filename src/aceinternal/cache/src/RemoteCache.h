
#ifndef REMOTE_CACHE_HPP
#define REMOTE_CACHE_HPP

#include "typedef.h"
#include "Cache.h"

class RemoteCache 
	: public Cache
{
public:
	RemoteCache();
	~RemoteCache();

	// 增加log 而已
	virtual int session_on_read() override;

public:
	virtual void output(Packet * packet);

	virtual int session_write() override;

	virtual void session_recvError(int recv_value, int last_error) override;
public:
	virtual int finit() override;

protected:
	virtual int initing() override;

private:
	ACE_Thread_Mutex m_output_packet_mutex;

	PacketQue_t m_remote_session_output;
};
#endif