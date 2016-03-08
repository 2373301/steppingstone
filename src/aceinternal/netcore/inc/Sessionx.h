#ifndef Sessionx_h__
#define Sessionx_h__

#include <fstream>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Message_Block.h>
#include "Packet.h"
#include "netcore_def.h"

using namespace std;

enum SessionStatex
{
	SS_NONE = 0,
	SS_OPEN,
	SS_CONNECTED,
	SS_CLOSE,
};

/*
HandleXXX
virtual XXX ,说明自己是handler
setHandleXXX, 设置 other 为 handler
*/

// cin << 进入的完整的packet
class NETCORE_EXOPRT HandleInputx
{
public:
	virtual void input(Packet * packet) = 0;
};

// cout >> 发送完整的packet, 有可能 非reactor线程
class NETCORE_EXOPRT HandleOutputx
{
public:
	virtual void output(Packet * packet) = 0;
};

class NETCORE_EXOPRT SavePackInfox
{
public:
	SavePackInfox();

	~SavePackInfox();

	bool init(bool is_save, const string & file_n);

	void save(char * stream_buffer, int stream_len);

	bool is_save_stream;
	fstream * file_stream;
	string file_name;
};

class NETCORE_EXOPRT Sessionx 
	:public HandleOutputx
	, public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	Sessionx();
	virtual ~Sessionx();

	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;
	typedef std::list<Packet *> PacketList;

	// ACE_Svc_Handler
	virtual int open(void * p = 0) override;
	virtual int close(ACE_Reactor_Mask close_mask) override;
	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE) override;
	virtual int handle_output(ACE_HANDLE  fd = ACE_INVALID_HANDLE) override;
	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask) override;

public:
	virtual int rd_stream(); // 0: normal, -1:close
	virtual int wt_stream(); // 0: normal, -1:close, 1:empty
	virtual void recvError(int recv_value, int last_error);
	virtual void initBufferError(int last_error);

	void setHandleInput(HandleInputx * a_input);
	void setSavePackInfo(bool is_save, const string & file_name);

protected:
	virtual int net_connected();
	virtual int net_closed();
	virtual void parseInputPacket();

protected:
	static int m_socket_buffer_length;

	SessionStatex m_session_state;

	HandleInputx * m_handle_input;

	SavePackInfox m_save_input_pack_info;

	ACE_Message_Block m_inputs;

	ACE_Message_Block m_outputs;
};

#endif // Sessionx_h__
