
#ifndef CELL_SESSION_HPP
#define CELL_SESSION_HPP

#include <ace/Reactor.h>
#include "Session.h"
#include "Packet.h"

namespace netstream
{

class CellSession;

struct CellSessionInfo
{
	CellSessionInfo()
	{}

	int reference_no;
};

class CellSession : public Session
{
public:
	CellSession();

	~CellSession();
public:
	virtual int open(void * p=0);

	virtual int rd_stream();

public:
	virtual bool output(char * buffer, int buff_size);

	virtual int wt_stream();
public:
	uint64 getGUID();

	void setGUID(uint64 guid);

protected:
private:
	uint64 m_guid;

	//ACE_Thread_Mutex m_output_packet_mutex;

	//PacketQue_t m_output_packet;

	ACE_Message_Block m_outpu_buffer;

	ACE_Thread_Mutex m_outpu_buffer_mutex;

	//SavePackInfo m_save_output_pack_info;

};

}

#endif