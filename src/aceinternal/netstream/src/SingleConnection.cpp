
#include <ace/OS.h>
#include <ace/SOCK_Connector.h>
#include "SingleConnection.h"
#include "Logger.h"

namespace netstream
{

SingleConnection::SingleConnection()
: m_connect_status(0)
, m_session(NULL)
, m_is_stop(false)
, m_socket_input_buffer_size(0)
, m_socket_output_buffer_size(0)
{
}

SingleConnection::~SingleConnection()
{
}

int SingleConnection::svc()
{
	ACE_Time_Value sleep_time(0, 10000);

	PacketQue_t packet_que;

	int read_result = 0;

	int write_result = 0;

	while (!m_is_stop)
	{
		if (NULL == m_session)
		{
			if (!connectToServer())
			{
				ACE_OS::sleep(1);
				continue;
			}
			else
			{
				newConnection(packet_que);
			}
		}

		read_result = m_session->rd_stream();

		write_result = processOutputPacket(packet_que);

		if (1 == write_result)
		{
			ACE_OS::sleep(sleep_time);
		}
		
		if ((-1 == read_result) || (-1 == write_result))
		{
			closeSession();
		}
	}

	return 0;
}

void SingleConnection::input(Session * session, ACE_Message_Block & msg_block)
{
	PacketVec_t packet_vec;
	parsePacketFromStream(session, msg_block, packet_vec);

	for (PacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
	{
		Packet * packet = *it;

		handlePacket(packet);

		//delete packet;
	}
}

int SingleConnection::init(const string & remote_addr)
{
	m_remote_addr = remote_addr;

	if (this->activate() == -1)
	{
		DEF_LOG_ERROR("failed to active the SingleConnection, last error is <%d>\n", ACE_OS::last_error());
		return -1;
	}

	while (0 == m_connect_status)
	{
		ACE_OS::sleep(1);
	}

	if (1 != m_connect_status)
	{
		DEF_LOG_ERROR("SingleConnection failed to listen at addr <%s>\n", m_remote_addr.c_str());
		return -1;
	}

	return 0;
}

int SingleConnection::stop()
{
	return 0;
}

int SingleConnection::output(Packet * packet)
{
	ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, guard, m_packet_que_mutex, 1);
	m_packet_que.push(packet);
	return 0;
}

void SingleConnection::setSocketBufferSize(int input_buf_size, int output_buf_size)
{
	m_socket_input_buffer_size = input_buf_size;
	m_socket_output_buffer_size = output_buf_size;
}

void SingleConnection::newConnection(PacketQue_t & output_packet_que)
{
}

void SingleConnection::onSessionCloseNotify()
{
}

bool SingleConnection::connectToServer()
{
	ACE_INET_Addr addr(m_remote_addr.c_str());
	m_session = new netstream::Session();
	ACE_SOCK_Connector connector;
	if (connector.connect(m_session->peer(), addr) == -1)
	{
		m_session->peer().close();
		delete m_session;
		m_session = NULL;
		m_connect_status = 2;
		DEF_LOG_ERROR("failed to connect the remote server <%s>\n", m_remote_addr.c_str());
		return false;
	}
	else
	{
		m_connect_status = 1;
		DEF_LOG_INFO("success to connect the remote server <%s>\n", m_remote_addr.c_str());
		if (m_socket_input_buffer_size > 0)
		{
			m_session->setSocketBufferSize(m_socket_input_buffer_size, m_socket_output_buffer_size);
		}
		m_session->open();
		m_session->setHandleInput(this);
		return true;
	}
}

int SingleConnection::processOutputPacket(PacketQue_t & packet_que)
{
	{
		ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, guard, m_packet_que_mutex, 1);
		while (m_packet_que.size() > 0)
		{
			packet_que.push(m_packet_que.front());
			m_packet_que.pop();
		}
	}

	while (packet_que.size() > 0)
	{
		Packet * packet = packet_que.front();
		if (m_session->output(packet->stream(), packet->stream_size()))
		{
			delete packet;
			packet_que.pop();
		}
		else
		{
			break;
		}
	}

	return m_session->wt_stream();
}

void SingleConnection::closeSession()
{
	DEF_LOG_ERROR("the single socket closed <%s>\n", m_remote_addr.c_str());
	m_session->peer().close();
	delete m_session;
	m_session = NULL;
	m_connect_status = 2;

	onSessionCloseNotify();
}

}
