
#ifndef CELL_SESSION_HPP
#define CELL_SESSION_HPP

#include <ace/Reactor.h>
#include "Sessionx.h"
#include "Packet.h"

namespace netcore
{

class CellSessionx;

struct CellSessionInfo
{
	CellSessionInfo()
	{}

	int reference_no;
};

class CellSessionx : public Sessionx
{
public:
	CellSessionx();
	~CellSessionx();

	virtual int open(void * p=0) override;
	virtual void output(Packet * packet) override; // 有可能 非reactor线程,所以写入缓存,待callback

	virtual int rd_stream() override;
	virtual int wt_stream() override; // copy m_deferred_outputs to outputs

	uint64 getGUID();
	void setGUID(uint64 guid);


private:
	uint64 m_guid;

	ACE_Thread_Mutex m_deferred_outputs_mutex;
	PacketQue_t m_deferred_outputs;

	//SavePackInfox m_save_output_pack_info;
};

}

#endif