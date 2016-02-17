
#include "stdafx.h"

#include <ostream>
//#include <google/protobuf/io/zero_copy_stream_impl.h>
//#include <google/protobuf/io/gzip_stream.h>
#include "ClientTCP.h"

bool PacketStream::parse_protocol (google::protobuf::Message & message) const
{
	return message.ParseFromString(string(m_buffer, m_header.size));
}

void PacketStream::resetBody( const char * body_buffer, size_t body_len )
{
	if (body_len > body_size())
	{
		delete [] m_total_buffer;

		m_header.size = body_len;
		m_total_size = head_size() + body_len;

		m_total_buffer = new char[m_total_size];
		m_buffer = m_total_buffer + head_size();
	}
	else
	{
		m_header.size = body_len;
		m_total_size = head_size() + body_len;
	}

	memcpy (m_total_buffer, &m_header, head_size());
	memcpy (m_total_buffer + head_size(), body_buffer, body_len);
	m_buffer = m_total_buffer + head_size();
}

ClientTCP::ClientTCP()
: m_close(true)
, m_recv_tail_position(0)
, m_socket(0)
, m_wnd(0)
, m_message_id(0)
, m_send_pos(0)
, m_send_packet_stream(NULL)
{
}

ClientTCP::~ClientTCP(void)
{
	close();
}

bool ClientTCP::open(const std::string &host, short port)
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socket)
	{
		return false;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(host.c_str());
	addr.sin_port = htons(port);

	int   nRecvBuf = 1024 * 1024; 

	if (SOCKET_ERROR == setsockopt(m_socket , SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)))
	{
		return false;
	}


	if ( connect( m_socket, (SOCKADDR*) &addr, sizeof(addr) ) == SOCKET_ERROR) 
	{
		printf( "Failed to connect.\n" );
		return false;
	}

	m_close = false;

	WSAAsyncSelect(m_socket, m_wnd, m_message_id, FD_READ | FD_CLOSE);
	return true;
}

int ClientTCP::read(std::deque<PacketStream*> & read_queue)
{
	if (m_close)
	{
		return false;
	}
	m_read_queue.clear();
	int result = handle_input();
	read_queue = m_read_queue;
	return result;
}

void ClientTCP::write (boost::uint32_t opcode, const std::string &message)
{
	if (m_close)
	{
		return;
	}

	PacketStream *stream = new PacketStream (opcode, message);

	m_write_queue.push_back(stream);
	rewrite();
}

void ClientTCP::rewrite()
{
	if (m_close)
	{
		return;
	}

	if (m_write_queue.size() == 0)
	{
		//cancelWriteEvent();
		return;
	}

	while (true)
	{
		if (NULL == m_send_packet_stream)
		{
			if (m_write_queue.size() == 0)
			{
				return;
			}

			m_send_pos = 0;
			m_send_packet_stream = m_write_queue.front();
			m_write_queue.pop_front();
		}
		
		int send_number = send(m_socket, m_send_packet_stream->stream() + m_send_pos, m_send_packet_stream->stream_size() - m_send_pos, 0);
		if (m_send_packet_stream->stream_size() == send_number + m_send_pos)
		{
			delete m_send_packet_stream;
			m_send_packet_stream = NULL;
			m_send_pos = 0;
		}
		else
		{
			m_send_pos += send_number;
			//registerWriteEvent();
			break;
		}
	}
}

void ClientTCP::close (void)
{
	if (!m_close)
	{
		m_close = true;
		closesocket(m_socket);
		cleanPacketStream();
		m_send_pos = 0;
	}
}

int ClientTCP::handle_input()
{
	int result = 0;
	int recv_number = 0;
	recv_number = recv(m_socket, m_recv_buffer + m_recv_tail_position, BUFFER_SIZE - m_recv_tail_position, 0);
	//TRACE1("number %d", recv_number);

	if (recv_number > 0)
	{
		parseInputPacket(m_recv_buffer, m_recv_tail_position + recv_number);
		result = 0;
	}
	else
	{
		if (GetLastError() == WSAEWOULDBLOCK)
		{
			result = 0;
		}
		else
		{
			result = -1;
		}
	}
	return result;
}

void ClientTCP::handle_output()
{
	std::deque<PacketStream*> write_queue;
	int send_number = 0;
	int pos = 0;
	while (!m_close)
	{
		if (m_write_queue.size() > 0)
		{
			write_queue.clear();
			write_queue = m_write_queue;
			m_write_queue.clear();
		}
		if (write_queue.size() == 0)
		{
			Sleep(1);
			continue;
		}
		for (std::deque<PacketStream*>::iterator it = write_queue.begin(); it != write_queue.end(); ++it)
		{
			PacketStream * ps = *it;
			while (true)
			{
				send_number = send(m_socket, ps->stream() + pos, ps->stream_size() - pos, 0);
				if (send_number == SOCKET_ERROR)
				{
					return;
				}
				else if (send_number + pos == ps->stream_size())
				{
					break;
				}
				else
				{
					pos += send_number;
				}
			}
			delete ps;
		}
		write_queue.clear();
	}
}

void ClientTCP::cleanPacketStream()
{
	for (std::deque<PacketStream*>::iterator it = m_write_queue.begin(); it != m_write_queue.end(); ++it)
	{
		delete *it;
	}
	m_write_queue.clear();
}

void ClientTCP::parseInputPacket(char * buffer, int buf_len)
{
	int current_buf_len = 0;
	PacketStream * ps = NULL;
	while (true)
	{
		ps = new PacketStream();
		if ((buf_len - current_buf_len) < ps->head_size())
		{
			delete ps;
			break;
		}
		memcpy(ps->re_head(), buffer + current_buf_len, ps->head_size());
		if ((buf_len - current_buf_len) >= ps->head_size() + ps->body_size())
		{
			current_buf_len += ps->head_size();

			if (ps->body_size() > 0)
			{
				memcpy(ps->re_body(), buffer + current_buf_len, ps->body_size());
				current_buf_len += ps->body_size();
			}

			//int loginGS = ::GetPrivateProfileInt("system","loginGS",1,".\\game.cfg");
			//if (loginGS)
			//{
			//	if (ps->opcode() == Opcodes::SMSG_LOGINREQUEST_REPLAY
			//		|| ps->opcode() == Opcodes::SMSG_ACCOUNT_RESULT
			//		|| ps->opcode() == Opcodes::SMSG_AUTH_SUCCESS
			//		|| ps->opcode() == Opcodes::SMSG_PIC
			//		|| ps->opcode() == Opcodes::SMSG_ENCRYPTION_KEY)
			//	{
			//		// do nothing
			//	}
			//	else
			//	{
			//		decode_packet(m_encryption, ps);
			//	}
			//}

			m_read_queue.push_back(ps);
		}
		else
		{
			delete ps;
			break;
		}
	}

	if (current_buf_len < buf_len)
	{
		memcpy(buffer, buffer + current_buf_len, buf_len - current_buf_len);
		m_recv_tail_position = buf_len - current_buf_len;
	}
	else
	{
		m_recv_tail_position = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
//      StreamConnector  
//////////////////////////////////////////////////////////////////////////

StreamConnector * StreamConnector::_singleton = NULL;

StreamConnector::StreamConnector (void)
{
}

StreamConnector::~StreamConnector (void)
{
}

void StreamConnector::update (void)
{
}

ClientTCP * StreamConnector::build (/*const std::string &host, short port, ConnectCallback acc*/)
{
	ClientTCP * client = new ClientTCP ();
	return client;
}