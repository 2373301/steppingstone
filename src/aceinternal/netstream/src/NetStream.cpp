//
//#include "TraceLogger.h"
//#include "NetStream.h"
//
//int NetStream::m_socket_buffer_length = 102400;
//
//NetStream::NetStream()
//: m_handle_net_packet(NULL)
//, m_cleaned(false)
//{
//	m_input_msg_block.init(m_socket_buffer_length);
//	m_output_msg_block.init(m_socket_buffer_length);
//}
//
//NetStream::~NetStream()
//{
//
//}
//
//int NetStream::open(void * p)
//{
//	if (super::open(p) == -1)
//	{
//		TRACELogp(LM_ERROR, ACE_TEXT("Failed to call super::open in NetStream::open, param is <%d>, the last error is : <%d>, msg is : %m\n"), p, ACE_OS::last_error());
//		return -1;
//	}
//	this->peer().enable(ACE_NONBLOCK);
//
//	int flag = 1;
//	this->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));
//
//	return netConnected();
//}
//
//int NetStream::handle_input(ACE_HANDLE  fd)
//{
//	if (rd_stream() == -1)
//	{
//		return -1;
//	}
//	else
//	{
//		return 0;
//	}
//}
//
//int NetStream::handle_output(ACE_HANDLE  fd)
//{
//	if (wt_stream() == -1)
//	{
//		return -1;
//	}
//	else
//	{
//		return 0;
//	}
//}
//
//int NetStream::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
//{
//	netClosed(close_mask);
//	return 0;
//}
//
//void NetStream::handleOutputStream(PacketStream * output_stream)
//{
//	if (NULL == output_stream)
//	{
//		return;
//	}
//	if (m_net_state.closed)
//	{
//		return ;
//	}
//
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_mutex, );
//	if (m_output_msg_block.length() == 0)
//	{
//		int r = this->peer().send(output_stream->stream(), output_stream->stream_size());
//		if (output_stream->stream_size() != r)
//		{
//			if (r > 0)
//			{
//				serializeToMsgBlock(output_stream, r);
//				this->reactor()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
//			}
//			else
//			{
//				if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
//				{
//					serializeToMsgBlock(output_stream, 0);
//					this->reactor()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
//				}
//				else
//				{
//					TRACELogp(LM_ERROR, ACE_TEXT("Failed to send data in NetStream::handleOutputStream, last error is <%d>, msg is : %m\n"), ACE_OS::last_error());
//				}
//			}
//		}
//		else
//		{
//			// success to send data
//		}
//	}
//	else
//	{
//		serializeToMsgBlock(output_stream, 0);
//	}
//
//	delete output_stream;
//}
//
//void NetStream::setPacketHandler(HandleNetPacket * handle_net_packet)
//{
//	m_handle_net_packet = handle_net_packet;
//}
//
//void NetStream::initSocketBufferLength(int buffer_length)
//{
//	m_socket_buffer_length = buffer_length;
//}
//
//int NetStream::netConnected()
//{
//	m_net_state.closed = false;
//	return 0;
//}
//
//void NetStream::netClosed(ACE_Reactor_Mask close_mask)
//{
//	m_net_state.closed = true;
//}
//
//void NetStream::parseInputPacket()
//{
//	PacketStream * ps = NULL;
//	int msg_length = 0;
//	try
//	{
//		msg_length = m_input_msg_block.length();
//		while(true)
//		{
//			if (m_input_msg_block.length() <= sizeof(Header))
//			{
//				break;
//			}
//
//			ps = new PacketStream();
//			memcpy(ps->re_head(), m_input_msg_block.rd_ptr(), ps->head_size());
//			if (m_input_msg_block.length() >= ps->head_size() + ps->body_size())
//			{
//				m_input_msg_block.rd_ptr(ps->head_size());
//				if (ps->body_size() > 0)
//				{
//					memcpy(ps->re_body(), m_input_msg_block.rd_ptr(), ps->body_size());
//					m_input_msg_block.rd_ptr(ps->body_size());
//				}
//				else
//				{
//					// do nothing
//				}
//
//				ps->setEventHandler(this);
//				m_handle_net_packet->handlePacket(ps);
//				msg_length = m_input_msg_block.length();
//			}
//			else
//			{
//				delete ps;
//				break;
//			}
//		}
//
//		if (m_input_msg_block.rd_ptr() != m_input_msg_block.base())
//		{
//			msg_length = m_input_msg_block.length();
//			if (msg_length > 0)
//			{
//				memcpy(m_input_msg_block.base(), m_input_msg_block.rd_ptr(), msg_length);
//			}
//			m_input_msg_block.rd_ptr(m_input_msg_block.base());
//			m_input_msg_block.wr_ptr(m_input_msg_block.base() + msg_length);
//		}
//	}
//	catch (...)
//	{
//		TRACELogp(LM_ERROR, ACE_TEXT("Raise unknown exception in ClientSession::parseInputPacket, last error is <%d>\n"), ACE_OS::last_error());
//	}
//}
//
//int NetStream::rd_stream()
//{
//	int result = 0;
//	int recv_n = this->peer().recv(m_input_msg_block.wr_ptr(), m_input_msg_block.space());
//	if (recv_n > 0)
//	{
//		m_input_msg_block.wr_ptr(recv_n);
//		parseInputPacket();
//	}
//	else if (0 == recv_n)
//	{
//		// normal close by client
//		TRACELogp(LM_ERROR, ACE_TEXT("Remote connection closed in ClientSession::rd_stream, recv number is 0, last error is <%d>, msg is : %m\n"), ACE_OS::last_error());
//		result = -1;
//	}
//	else if (recv_n < 0)
//	{
//		if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
//		{
//			// it's ok
//		}
//		else
//		{
//			//// socket error
//			//if (!m_role_logout)
//			//{
//			//	ClientStrategyCenter::instance()->clientExceptionClosed(this);
//			TRACELogp(LM_ERROR, ACE_TEXT("Remote connection closed in ClientSession::rd_stream, last error is <%d>, msg is : %m\n"), ACE_OS::last_error());
//			result = -1;
//			//			}
//			//closeHandle();
//		}
//	}
//	return result;
//}
//
//int NetStream::wt_stream()
//{
//	int result = 0;
//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_mutex, -1);
//	if (m_output_msg_block.length() > 0)
//	{
//		int r = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());
//		if (r > 0)
//		{
//			m_output_msg_block.rd_ptr(r);
//			if (m_output_msg_block.length() == 0)
//			{
//				this->reactor()->remove_handler(this, ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
//			}
//		}
//		else
//		{
//			if ((ACE_OS::last_error() == EWOULDBLOCK) || (ACE_OS::last_error() == EINTR) || (ACE_OS::last_error() == EAGAIN))
//			{
//				// ok
//			}
//			else
//			{
//				// error
//				result = -1;
//			}
//		}
//	}
//	else
//	{
//		this->reactor()->remove_handler(this, ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
//	}
//
//	if (0 != result)
//	{
//		return -1;
//	}
//	else
//	{
//		return 0;
//	}
//}
//
//void NetStream::serializeToMsgBlock(PacketStream * output_stream, int already_read_byte)
//{
//	int msg_length = m_output_msg_block.length();
//	if (m_output_msg_block.space() < output_stream->stream_size())
//	{
//		if (msg_length * 2 > m_output_msg_block.size())
//		{
//			return;
//		}
//		memcpy(m_output_msg_block.base(), m_output_msg_block.rd_ptr(), msg_length);
//		m_output_msg_block.rd_ptr(m_output_msg_block.base());
//		m_output_msg_block.wr_ptr(m_output_msg_block.base() + msg_length);
//	}
//	m_output_msg_block.copy(output_stream->stream() + already_read_byte, output_stream->stream_size() - already_read_byte);
//}