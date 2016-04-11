
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
	CellSessionInfo(){}

	int reference_no;
};

class CellSession : public Session
{
public:
	CellSession();
	CellSession(bool client);
	~CellSession();

	uint64 getGUID();
	void setGUID(uint64 guid);

public:
	virtual bool IStream_output(char * buffer, int buff_size) override;
	virtual int session_on_connected() override;
	virtual int session_on_read() override;
	virtual int session_write() override;

private:
	uint64 m_guid;
	ACE_Message_Block cell_out_buf_;
	ACE_Thread_Mutex cell_out_mutex_;
};


}

#endif