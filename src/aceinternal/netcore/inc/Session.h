
#ifndef SESSION_HPP
#define SESSION_HPP

#include <fstream>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Message_Block.h>
#include "Packet.h"
#include "netcore_def.h"

using namespace std;

enum SessionState
{
	SS_NONE			= 0,
	SS_OPEN,
	SS_CONNECTED,
	SS_CLOSE,
};

class NETCORE_EXOPRT HandleInput
{
public:
	virtual void input(Packet * packet) = 0;
};

class NETCORE_EXOPRT HandleOutput
{
public:
	virtual void output(Packet * packet) = 0;
};

class NETCORE_EXOPRT SavePackInfo
{
public:
	SavePackInfo();

	~SavePackInfo();

	bool init(bool is_save, const string & file_n);

	void save(char * stream_buffer, int stream_len);

	bool is_save_stream;
	fstream * file_stream;
	string file_name;
};

class NETCORE_EXOPRT Session : public HandleOutput, public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	Session();

	virtual ~Session();
public:
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;

	typedef std::list<Packet *> PacketList;

	virtual int open(void * p=0);

	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_output(ACE_HANDLE  fd = ACE_INVALID_HANDLE);

	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

public:
	virtual void output(Packet * packet);

	bool handleOutputPacket(Packet * packet);

public:
	// 0: normal
	virtual int rd_stream();

	// 0: normal, -1:close, 1:empty
	virtual int wt_stream();

	virtual int close(uint32 close_mask);

	void setHandleInput(HandleInput * handle_input);

	virtual void recvError(int recv_value, int last_error);

	virtual void initBufferError(int last_error);

	void setSavePackInfo(bool is_save, const string & file_name);

protected:
	virtual int net_connected();

	virtual int net_closed();
protected:
	virtual void parseInputPacket();

protected:
	static int m_socket_buffer_length;

	SessionState m_session_state;

	HandleInput * m_handle_input;

	SavePackInfo m_save_input_pack_info;

	ACE_Message_Block m_input_msg_block;

	ACE_Message_Block m_output_msg_block;
};

#endif