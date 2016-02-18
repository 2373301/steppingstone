
#include <boost/lexical_cast.hpp>
#include <ace/OS_Errno.h>
#include <ace/OS.h>
#include <ace/Date_Time.h>
#include <ace/os_include/netinet/os_tcp.h>
#include <iostream>
#include "Session.h"
#include "Logger.h"

//#define LOG_INPUT_MSG_SIZE()	DEF_LOG_DEBUG("the input msg rd_ptr is <%x>, wt_ptr is <%x>, file line is <%d>\n", m_inputs.rd_ptr(), m_inputs.wr_ptr(), __LINE__)

#define LOG_INPUT_MSG_SIZE()	

int Session::m_socket_buffer_length = 102400;

SavePackInfo::SavePackInfo()
: is_save_stream(false)
, file_stream(NULL)
{}

SavePackInfo::~SavePackInfo()
{
	if (NULL != file_stream)
	{
		file_stream->close();
		delete file_stream;
	}
}

bool SavePackInfo::init(bool is_save, const string & file_n)
{
	is_save_stream = is_save;
	this->file_name = file_n;
	if (is_save_stream)
	{
		if (NULL != file_stream)
		{
			file_stream->close();
			delete file_stream;
			file_stream = NULL;
		}

		char file_str[100] = {0};
		ACE_Date_Time curr_time(ACE_OS::gettimeofday());
		sprintf(file_str, "%s_%d_%d_%dT%d_%d_%d", file_name.c_str(), curr_time.year(), curr_time.month(), curr_time.day(), curr_time.hour(), curr_time.minute(), curr_time.second());
		file_stream = new fstream();
		file_stream->open(file_str, ios_base::out | ios_base::app | ios_base::binary);
		if (file_stream->fail())
		{
			delete file_stream;
			file_stream = NULL;
			return false;
		}
	}

	return true;
}

void SavePackInfo::save(char * stream_buffer, int stream_len)
{
	if (is_save_stream && (NULL != file_stream))
	{
		file_stream->rdbuf()->sputn(stream_buffer, stream_len);
		file_stream->flush();
	}
}

Session::Session()
: m_session_state(SS_NONE)
, m_handle_input(NULL)
{
	m_inputs.init(m_socket_buffer_length);
	m_outputs.init(m_socket_buffer_length);
}

Session::~Session()
{

}

int Session::open(void * p)
{
	//// for test
	//int init_buf = m_inputs.init(m_socket_buffer_length);
	//if (-1 == init_buf)
	//{
	//	initBufferError(ACE_OS::last_error());
	//}

	//init_buf = m_outputs.init(m_socket_buffer_length);
	//if (-1 == init_buf)
	//{
	//	initBufferError(ACE_OS::last_error());
	//}

	m_session_state = SS_OPEN;

	if (super::open(p) == -1)
	{
		//GATELogp(LM_ERROR, ACE_TEXT("Failed to call super::open in GSSession::open, param is <%d>, the last error is : <%d>, msg is : %m\n"), p, ACE_OS::last_error());
		return -1;
	}

	this->peer().enable(ACE_NONBLOCK);

	int flag = 1;
	this->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

	return net_connected();
}

int Session::handle_input(ACE_HANDLE  fd)
{
	return rd_stream();
}

int Session::handle_output(ACE_HANDLE  fd)
{
	int result = wt_stream();
	if (-1 == result)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int Session::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	if (ACE_Event_Handler::WRITE_MASK == close_mask)
	{
		return 0;
	}

	return close(close_mask);
}

int Session::rd_stream()
{
	int result = 0;

	LOG_INPUT_MSG_SIZE();

	m_inputs.crunch();

	LOG_INPUT_MSG_SIZE();

	int recv_n = (int)this->peer().recv(m_inputs.wr_ptr(), m_inputs.space());
	if (recv_n > 0)
	{
		m_save_input_pack_info.save(m_inputs.wr_ptr(), recv_n);

		LOG_INPUT_MSG_SIZE();

		m_inputs.wr_ptr(recv_n);

		LOG_INPUT_MSG_SIZE();

		parseInputPacket();
	}
	else if (0 == recv_n)
	{
		// remote normal close
		//DEF_LOG_ERROR("remote stream close normally, last error is <%d>\n", ACE_OS::last_error());
		//string debug_str = "remote stream close normally, last error is : " + boost::lexical_cast<string>(ACE_OS::last_error()) + " buffer length : " + boost::lexical_cast<string>(m_inputs.space()) + "\n";
		//std::cout << debug_str;
		recvError(recv_n, ACE_OS::last_error());
		result = -1;
	}
	else
	{
		int last_error = ACE_OS::last_error();
		if ((last_error == EWOULDBLOCK) || (last_error == EINTR) || (last_error == EAGAIN))
		{
			// ok, no data in session
		}
		else
		{
			//DEF_LOG_ERROR("occur error while send data, return value is <%d>, last error is <%d>\n", recv_n, last_error);
			//string debug_str = "occur error while send data, return value is : " + boost::lexical_cast<string>(recv_n) + " last error is : " + boost::lexical_cast<string>(ACE_OS::last_error()) + "\n";
			//std::cout << debug_str;
			recvError(recv_n, ACE_OS::last_error());
			result = -1;
		}
	}

	return result;
}

int Session::wt_stream()
{
	// todo
	int result = 0;
	if (m_outputs.length() > 0)
	{
		ssize_t send_n = this->peer().send(m_outputs.rd_ptr(), m_outputs.length());
		if (send_n <= 0)
		{
			int last_error = ACE_OS::last_error();
			if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error))
			{
				result = 0;
			}
			else
			{
				this->shutdown();
				result = -1;
			}
		}
		else
		{
			m_outputs.rd_ptr(send_n);
		}
	}
	else
	{
		result = 1;
	}

	if (m_outputs.length() < 500)
	{
		m_outputs.crunch();
	}

	//if (m_outputs.length() == 0)
	//{
	//	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_deferred_outputs_mutex, -1);
	//	if (m_deferred_outputs.size() == 0)
	//	{
	//		//this->reactor()->remove_handler(this, ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL);
	//		result = -1;
	//	}
	//}

	return result;
}

int Session::close(ACE_Reactor_Mask close_mask)
{
	if (SS_CLOSE != m_session_state)
	{
		m_session_state = SS_CLOSE;

		net_closed();
	}

	if ((close_mask & ACE_Event_Handler::READ_MASK) || (close_mask & ACE_Event_Handler::WRITE_MASK))
	{
		return 0;
	}

	return 0;
}

void Session::setHandleInput(HandleInput * a_input)
{
	m_handle_input = a_input;
}

void Session::recvError(int recv_value, int last_error)
{

}

void Session::initBufferError(int last_error)
{

}

void Session::setSavePackInfo(bool is_save, const string & file_name)
{
	//m_save_input_pack_info.init(is_save, file_name);
	m_save_input_pack_info.init(false, file_name);
}

int Session::net_connected()
{
	return 0;
}

int Session::net_closed()
{
	return 0;
}

void Session::parseInputPacket()
{
	while (true)
	{
		if (m_inputs.length() < Packet::head_size())
		{
			return;
		}

		Packet * ps = new Packet();
		
		memcpy(ps->ch_head(), m_inputs.rd_ptr(), ps->head_size());

		if (m_inputs.length() >= ps->head_size() + ps->body_size())
		{
			m_inputs.rd_ptr(ps->head_size());

			memcpy(ps->ch_body(), m_inputs.rd_ptr(), ps->body_size());

			m_inputs.rd_ptr(ps->body_size());

			if (NULL != m_handle_input)
			{
				ps->setOwner(this);
				m_handle_input->input(ps);
			}
			else
			{
				// fatal error
				delete ps;
			}
		}
		else
		{
			//std::cout << "not full package, opcode is : " << ps->opcode() << " body size is : " << ps->body_size() << std::endl;
			delete ps;
			break;
		}
	}
}