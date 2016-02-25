
#ifndef SESSION_HPP
#define SESSION_HPP

#include <fstream>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Message_Block.h>
#include "Packet.h"
#include "netstream_def.h"

class NETSTREAM_EXOPRT HandleInput
{
public:
	virtual void input(Packet * packet) = 0;
};

namespace netstream
{

using namespace std;

class Session;

typedef void * Session_t;

extern "C" void NETSTREAM_EXOPRT parsePacketFromStream(Session_t session, ACE_Message_Block & msg_block, PacketVec_t & packet_vec);

enum SessionState
{
	SS_NONE			= 0,
	SS_OPEN,
	SS_CONNECTED,
	SS_CLOSE,
};

class NETSTREAM_EXOPRT HandleInputStream
{
public:
	virtual void input(Session * session, ACE_Message_Block & msg_block) = 0;
};

class NETSTREAM_EXOPRT HandleOutput
{
public:
	virtual void output(Packet * packet) = 0;
};

class NETSTREAM_EXOPRT HandleOutputStream
{
public:
	virtual bool output(char * buffer, int buff_size) = 0;
};

class NETSTREAM_EXOPRT SavePackInfo
{
public:
	SavePackInfo();

	bool init(bool is_save, const string & file_n);

	void save(char * stream_buffer, int stream_len);

	bool is_save_stream;
	fstream * file_stream;
	string file_name;
};

class NETSTREAM_EXOPRT Session
	: public HandleOutputStream
	, public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	Session();
	Session(bool client);
	virtual ~Session();

	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;
	typedef std::list<Packet *> PacketList;

	virtual int open(void * p=0) override;

	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE) override;

	virtual int handle_output(ACE_HANDLE  fd = ACE_INVALID_HANDLE) override;

	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask) override;

public:
	virtual bool output(char * buffer, int buff_size) override;

	void setSocketBufferSize(int input_buf_size, int output_buf_size);

public:
	virtual int rd_stream();

	// 0 : normal, -1: socket closed, 1:empty buffer, 2:call again, still have data in buffer
	virtual int wt_stream();

	virtual int close(uint32 close_mask);

	void setHandleInput(HandleInputStream * handle_input);

	virtual void recvError(int recv_value, int last_error);

	virtual void initBufferError(int last_error);

	void setSavePackInfo(bool is_save, const string & file_name);

	bool isClientSide() { return m_clientSide;  }
protected:
	virtual int net_connected();

	virtual int net_closed();
protected:
	virtual void parseInputPacket();

protected:
	static int m_socket_buffer_length;

	SessionState m_session_state;

	HandleInputStream * m_handle_input;

	//SavePackInfo m_save_input_pack_info;

	ACE_Message_Block m_input_msg_block;

	ACE_Message_Block m_output_msg_block;

	bool m_clientSide;
};

}

#endif