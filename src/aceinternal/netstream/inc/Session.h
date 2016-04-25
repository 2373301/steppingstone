
#ifndef SESSION_HPP
#define SESSION_HPP

#include <fstream>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Message_Block.h>
#include "Packet.h"
#include "netstream_def.h"

namespace netstream
{

class Session;
typedef void * Session_t;

extern "C" void NETSTREAM_EXOPRT parsePacketFromStream(Session_t session, ACE_Message_Block & msg_block, PacketVec_t & packet_vec);

enum SessionState
{
	SS_NONE			= 0,
	SS_CONNECTED,
	SS_CLOSED,
};

class NETSTREAM_EXOPRT ISessionIn
{
public:
	virtual void ISessionIn_syncRead(Session * session, ACE_Message_Block & msg_block) = 0;
};

class NETSTREAM_EXOPRT IInput
{
public:
	virtual void IInput_inputPacket(Packet * packet) = 0;
};


class NETSTREAM_EXOPRT IOutput
{
public:
	virtual void IOutput_outputPacket(Packet * packet) = 0;
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

// 外静内动
class NETSTREAM_EXOPRT Session
	: public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	Session();
	Session(bool client);
	virtual ~Session();

	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> super;
	typedef std::list<Packet *> PacketList;

	void	setBufSize(int in_size, int out_size);
	void	setHandleInput(ISessionIn * a_input);
	bool	isClientSide() { return client_side_; }
	int		setReadEvent();
	void	setSavePackInfo(bool is_save, const string & file_name);
	bool	asyncWrite(char * buffer, int buff_size);
	int		syncWrite();	

protected:
	friend class SingleConnection;

	/* 这三个callback 由 reactor 驱动 */
	virtual int  session_on_connected();
	virtual int  session_on_closed();
	virtual int  session_on_read(); // 有问题, 只reset flag
	virtual void session_on_read_error(int recv_num, int last_err);

	
									
public: // ace 的 callback, 隔离, 不用
	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE) final;
	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask) final;
	virtual int open(void * p = 0) final;

protected:
	static int s_socket_buf_len_;
	SessionState session_state_;
	netstream::ISessionIn * handle_input_;
	//SavePackInfo m_save_input_pack_info;
	ACE_Message_Block sync_in_buf_;
	ACE_Message_Block sync_out_buf_;
	bool client_side_;

	ACE_Message_Block async_out_buf_;
	ACE_Thread_Mutex async_out_mutex_;
};

}

#endif