#pragma once

#include <string>
#include <deque>
#include <list>
#include <boost/bind.hpp>
#include <boost/cstdint.hpp>
#include <boost/function.hpp>
#include <WinSock2.h>
#include <Windows.h>
#include <google/protobuf/message.h>

#define BUFFER_SIZE 10240

using namespace std;

class PacketStream
{
	struct Header {
		Header (void) : opcode (0) {}
		Header (boost::uint16_t o, boost::uint16_t s) 
			: opcode (o), size (s) {}

		boost::uint16_t opcode;  /// message code
		boost::uint16_t size;    /// message body length
	};

public:		
	PacketStream (void) : m_buffer (NULL), m_total_size (0), m_total_buffer(NULL) {}

	/// 构造一个完整的协议包
	PacketStream (boost::uint32_t opcode, const std::string &msg)
		: m_header (opcode, msg.length ())
	{
		m_total_size = head_size() + msg.length();
		m_total_buffer = new char[m_total_size];
		memcpy (m_total_buffer, &m_header, sizeof m_header);
		memcpy (m_total_buffer + head_size (), msg.c_str (), msg.length ());
		m_buffer = m_total_buffer + head_size();
	}

	~PacketStream (void) 
	{
		delete []m_total_buffer;
		m_total_buffer = NULL;
		m_buffer = NULL;
	}

	boost::uint32_t opcode (void) const {return m_header.opcode;}

	boost::uint32_t 
		head_size (void) const {return sizeof (Header);}

	boost::uint32_t
		body_size (void) const {return m_header.size;}

	boost::uint32_t
		stream_size (void) const {return m_total_size;}

	/// 当从流中接收一个头部结构时调用此方法
	char * re_head (void) {return reinterpret_cast<char*>(&m_header);}

	/// 调用此方法从套接口接收一个正文流
	char * re_body (void) 
	{
		if (m_total_buffer == NULL)
		{
			m_total_buffer = new char[m_header.size + head_size()];
		}

		memcpy(m_total_buffer, &m_header, head_size());
		m_buffer = m_total_buffer + head_size();
		return m_buffer;
	}	


	/// 提取接收到的消息体
	const char * pe_body (void) const {return m_buffer;}

	/// 发送时应该调用此方法来将组装好的协议一块发送
	const char * stream (void) const {return m_total_buffer;}

	/// 解析协议
	bool parse_protocol (google::protobuf::Message &message) const;


	void resetBody(const char * body_buffer, size_t body_len);

protected:
	Header m_header;
	char * m_buffer;
	boost::uint32_t m_total_size;
	char *m_total_buffer;
};

class ClientTCP;

typedef boost::function<void(ClientTCP *, bool)> ConnectCallback;
typedef boost::function<void(PacketStream*)> MessageHandler;

class ClientTCP;

class HandleStream
{
public:
	virtual void handle_stream() = 0;
};

class ClientTCP
{
public:
	ClientTCP();

	virtual ~ClientTCP (void);

public:
	bool open(const std::string &host, short port);

	int read(std::deque<PacketStream*> & read_queue);

	void write (boost::uint32_t opcode, const std::string &message);

	void rewrite();

	void close (void);

	void msg_handler (HWND wnd, UINT msg_id)
	{
		m_wnd = wnd;
		m_message_id = msg_id;
	}

protected:
	int handle_input();

	void handle_output();

private:
	void cleanPacketStream();

	void parseInputPacket(char * buffer, int buf_len);
protected:
	bool   m_close;

	int	   m_recv_tail_position;

	char m_recv_buffer[BUFFER_SIZE];

	SOCKET m_socket;

	HWND m_wnd;

	UINT m_message_id;

	int m_send_pos;

	PacketStream * m_send_packet_stream;

	std::deque<PacketStream*> m_read_queue;

	std::deque<PacketStream*> m_write_queue;
};

class StreamConnector
{
public:
	StreamConnector (void);
	virtual ~StreamConnector (void);

	/// 打开一个客户端TCP连接
	ClientTCP * build (); 

	static StreamConnector * instance (void)
	{
		if (_singleton == NULL)
			_singleton = new StreamConnector ();

		return _singleton;
	}

	void update (void);

protected:
	static StreamConnector *_singleton;
};

#define sConnector (*StreamConnector::instance())