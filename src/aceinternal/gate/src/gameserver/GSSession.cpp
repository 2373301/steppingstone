
#include <ace/OS.h>
#include <ace/os_include/netinet/os_tcp.h>
#include "GSSession.h"
#include "GateLogger.h"
#include "ManageGSStream.h"
#include "RouteGSPacket.h"
#include "ManageStat.h"
#include "ManageGSSessionCooler.h"
#include "ManageConfig.h"
#include "GSStrategyCenter.h"
#include "ManageGSSession.h"

#define INPUT_BUFFER_SIZE 1024

GSSession::GSSession()
: m_shutdown(false)
, m_sock_disconnection(false)
, m_is_transcript(false)
{
	m_output_msg_block.init(sMsgCfg->getGSCfg().socket_buffer_length);
	m_input_msg_block.init(sMsgCfg->getGSCfg().socket_buffer_length);
}

GSSession::~GSSession()
{
	cleanup();
}

int GSSession::open(void * p)
{
	ManageStat::instance()->statGSConnection(1);

	if (super::open(p) == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call super::open in GSSession::open, param is <%d>, the last error is : <%d>\n"), p, ACE_OS::last_error());
		return -1;
	}
	ACE_TCHAR peer_name[MAXHOSTNAMELEN];
	ACE_INET_Addr peer_addr;
	if (this->peer().get_remote_addr(peer_addr) == 0 &&
		peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN) == 0)
	{
		GATE_LOG_INFO(ACE_TEXT("Get new connection from game server, the ip is : %s\n"), peer_name);
	}

	this->peer().enable(ACE_NONBLOCK);

	int flag = 1;
	this->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

	ManageGSSession::instance()->addGSSession(this);

	return 0;
}

int GSSession::handle_input(ACE_HANDLE  fd)
{
	if (rd_stream() == -1)
	{
		ManageGSSessionCooler::instance()->putGSSession(this);
		return -1;
	}
	else
	{
		return 0;
	}
}

int GSSession::handle_output(ACE_HANDLE  fd)
{
	// do nothing
	// return 0;
	int result = 0;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_transcript_output_mutex, -1);
	if (m_output_msg_block.length() > 0)
	{
		int r = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
		if (r > 0)
		{
			m_output_msg_block.rd_ptr(r);
			if (m_output_msg_block.length() == 0)
			{
				this->reactor()->remove_handler(this, ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
			}
		}
		else
		{
			if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
			{
				// ok
			}
			else
			{
				// error
				result = -1;
			}
		}
	}
	else
	{
		this->reactor()->remove_handler(this, ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
	}

	if (0 != result)
	{
		ManageGSSessionCooler::instance()->putGSSession(this);
		return -1;
	}
	else
	{
		return 0;
	}
}

int GSSession::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	if ((close_mask & ACE_Event_Handler::READ_MASK) || (close_mask & ACE_Event_Handler::WRITE_MASK))
	{
		return 0;
	}

	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_handle_close_mutex, 0);

	if (!m_shutdown)
	{
		m_shutdown = true;

		ManageGSSession::instance()->removeGSSession(this);

		if (m_map_id.size() > 0)
		{
			if (!ManageGSStream::instance()->isStop())
			{
				GSStrategyCenter::instance()->GSClosedNotifyClient(this);
			}
		}

		this->peer().close();

		m_handle_close_time = ACE_OS::gettimeofday();

		for (vector<uint64>::iterator it = m_map_id.begin(); it != m_map_id.end(); ++it)
		{
			RouteGSPacket::instance()->removeGSMapID(*it);
		}

		ManageStat::instance()->statGSDisconnection(1);

//		ManageGSSessionCooler::instance()->putGSSession(this);

	}
	return 0;
}

void GSSession::handleOutputStream(PacketStream * output_stream)
{
	if ((NULL == output_stream)
		|| m_sock_disconnection)
	{
		if (NULL != output_stream)
		{
			delete output_stream;
		}
		return;
	}

	if (m_is_transcript)
	{
		handleOutputStreamToTranscript(output_stream);
	}
	else
	{
		handleOutputStreamToGS(output_stream);
	}
}

void GSSession::handleOutputStreamToGS(PacketStream * output_stream)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_msg_mutex, );
	if (m_output_msg_block.space() < output_stream->stream_size())
	{
		if (m_output_msg_block.length() * 2 > m_output_msg_block.capacity())
		{
			ManageStat::instance()->statGSPacketLoss(1);
			delete output_stream;
			return ;
		}
		m_output_msg_block.crunch();
	}

	if ((output_stream->stream_size() > 4096) || (output_stream->body_size() > 4096))
	{
		return ;
	}

	m_output_msg_block.copy(output_stream->stream(), output_stream->stream_size());
	ManageStat::instance()->statGSOutputTraffic(output_stream->stream_size(), 1);
	delete output_stream;
}

void GSSession::handleOutputStreamToTranscript(PacketStream * output_stream)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_transcript_output_mutex, );
	if (m_output_msg_block.length() == 0)
	{
		int r = this->peer().send(output_stream->stream(), output_stream->stream_size());
		if (output_stream->stream_size() != r)
		{
			if (r > 0)
			{
				serializeToMsgBlock(output_stream, r);
				this->reactor()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
			}
			else
			{
				if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
				{
					serializeToMsgBlock(output_stream, 0);
					this->reactor()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
				}
				else
				{
					GATE_LOG_ERROR(ACE_TEXT("Failed to send data in GSSession::handleOutputStreamToTranscript, last error is <%d>\n"), ACE_OS::last_error());
					//removeHandler(0);
					//m_session_client_state.socket_closed = true;
					//ManageClientSessionCooler::instance()->putClientSession(this);
					// error
				}
			}
		}
		else
		{
			ManageStat::instance()->statGSOutputTraffic(output_stream->stream_size(), 1);
		}
	}
	else
	{
		serializeToMsgBlock(output_stream, 0);
	}

	delete output_stream;
}

void GSSession::setMapID(uint64 map_id)
{
	m_map_id.push_back(map_id);
}

vector<uint64> GSSession::getMapID()
{
	return m_map_id;
}

void GSSession::setTranscript(bool is_transcript)
{
	m_is_transcript = is_transcript;
}

int GSSession::rd_stream()
{
	int result = 0;
	//if (m_recv_buffer_length > m_input_msg_block.space())
	//{
		//int msg_length = m_input_msg_block.length();
		//memcpy(m_input_msg_block.base(), m_input_msg_block.rd_ptr(), msg_length);
		//m_input_msg_block.rd_ptr(m_input_msg_block.base());
		//m_input_msg_block.wr_ptr(m_input_msg_block.base() + msg_length);
	//}
	m_input_msg_block.crunch();
	int recv_n = this->peer().recv(m_input_msg_block.wr_ptr(), m_input_msg_block.space());
	if (recv_n > 0)
	{
		m_input_msg_block.wr_ptr(recv_n);
		parseInputPacket();
	}
	else if (0 == recv_n)
	{
		// close
		GATE_LOG_ERROR(ACE_TEXT("Remote connection closed in GSSession::rd_stream, space is <%d>, recv return 0, last error is <%d>\n"), m_input_msg_block.space(), ACE_OS::last_error());
		m_sock_disconnection = true;
		ManageGSSessionCooler::instance()->putGSSession(this);
		result = -1;
	}
	else
	{
		if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
		{
			result = 2;
			ACE_OS::sleep(ACE_Time_Value(0, 1000));
		}
		else
		{
			GATE_LOG_ERROR(ACE_TEXT("Remote connection closed in GSSession::rd_stream, space is <%d>, last error is <%d>\n"), m_input_msg_block.space(), ACE_OS::last_error());
			m_sock_disconnection = true;
			ManageGSSessionCooler::instance()->putGSSession(this);
			result = -1;
		}
	}
	return result;
}

int GSSession::wt_stream()
{
	int result = 0;
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_msg_mutex, -1);
	if (m_output_msg_block.length() == 0)
	{
		if (m_sock_disconnection)
		{
			return -1;
		}
		return 2;
	}

	int send_n = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
	//GATELogp(LM_INFO, ACE_TEXT("Start to send data, length is <%d>\n"), m_output_msg_block.length());
	if (send_n <= 0)
	{
		GATE_LOG_ERROR(ACE_TEXT("error to send data in GSSession::wt_stream, send return value is <%d>, last error is <%d>\n"), send_n, ACE_OS::last_error());
		if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
		{
			result = 0;
			ACE_OS::sleep(ACE_Time_Value(0, 100 * 1000));
		}
		else
		{
			result = -1;
			GATE_LOG_ERROR(ACE_TEXT("Failed to send data, last error is <%d>\n"), ACE_OS::last_error());
		}
	}
	else
	{
		//GATELogp(LM_INFO, ACE_TEXT("success to send data, length is <%d>\n"), send_n);
		m_output_msg_block.rd_ptr(send_n);
		result = 0;
	}
	return result;
}

const ACE_Time_Value & GSSession::getHandleCloseTime()
{
	return m_handle_close_time;
}

bool GSSession::isShutdown()
{
	return m_shutdown;
}

void GSSession::cleanup()
{
}

void GSSession::parseInputPacket()
{
	vector<int> q;
	while (true)
	{
		if (m_input_msg_block.length() < sizeof(Header))
		{
			return;
		}

		PacketStream * ps = new PacketStream();
		ps->setEventHandler(this);
		memcpy(ps->re_head(), m_input_msg_block.rd_ptr(), ps->head_size());
		if (m_input_msg_block.length() >= ps->head_size() + ps->body_size())
		{


			if (ps->body_size() > 2220)
			{
				int a = 1;
				a = a + 1;
			}
			q.push_back(ps->body_size());



			m_input_msg_block.rd_ptr(ps->head_size());
			memcpy(ps->re_body(), m_input_msg_block.rd_ptr(), ps->body_size());
			m_input_msg_block.rd_ptr(ps->body_size());
			ManageStat::instance()->statGSInputTraffic(ps->stream_size(), 1);
			RouteGSPacket::instance()->putPackage(ps);
		}
		else
		{
			delete ps;
			break;
		}
	}
}

void GSSession::serializeToMsgBlock(PacketStream * output_stream, int already_read_byte)
{
	int msg_length = m_output_msg_block.length();
	if (m_output_msg_block.space() < output_stream->stream_size())
	{
		if (msg_length * 2 > m_output_msg_block.size())
		{
			return;
		}
		memcpy(m_output_msg_block.base(), m_output_msg_block.rd_ptr(), msg_length);
		m_output_msg_block.rd_ptr(m_output_msg_block.base());
		m_output_msg_block.wr_ptr(m_output_msg_block.base() + msg_length);
	}
	m_output_msg_block.copy(output_stream->stream() + already_read_byte, output_stream->stream_size() - already_read_byte);
}