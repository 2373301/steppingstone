
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
	SS_CONNECTED,
	SS_CLOSED,
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

// handle_xxx �Ǳ� reactor��ܵ���
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

	virtual bool output(char * buffer, int buff_size) override;

	void setSocketBufferSize(int input_buf_size, int output_buf_size);

	void setHandleInput(HandleInputStream * handle_input);
	bool isClientSide() { return client_side_; }
protected:
	friend class SingleConnection;
	virtual int rd_stream(); // ������, ֻreset flag
	// 0 : normal, -1: socket closed, 1:empty buffer, 2:call again, still have data in buffer
	virtual int wt_stream(); // �������� shutdown
	virtual void recvError(int recv_value, int last_error);	// recv num, err
	void setSavePackInfo(bool is_save, const string & file_name);
	
	virtual int net_connected();	// ������, �Զ���ĵط�
	virtual int net_closed();		// �ѹر�, �Զ���ĵط�

protected:
	static int s_socket_buf_len_;
	SessionState session_state_;
	netstream::HandleInputStream * handle_input_;
	//SavePackInfo m_save_input_pack_info;
	ACE_Message_Block in_buf_;
	ACE_Message_Block out_buf_;
	bool client_side_;
};

}

#endif