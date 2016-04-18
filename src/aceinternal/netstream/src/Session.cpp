
#include <boost/lexical_cast.hpp>
#include <ace/OS_Errno.h>
#include <ace/OS.h>
#include <ace/Date_Time.h>
#include <ace/os_include/netinet/os_tcp.h>
#include <iostream>
#include "Session.h"
#include "Logger.h"

//#define LOG_INPUT_MSG_SIZE()	DEF_LOG_DEBUG("the ISessionIn_sync_read msg rd_ptr is <%x>, wt_ptr is <%x>, file line is <%d>\n", sync_in_buf_.rd_ptr(), sync_in_buf_.wr_ptr(), __LINE__)
#define LOG_INPUT_MSG_SIZE()
namespace netstream
{

int Session::s_socket_buf_len_ = 102400;

void parsePacketFromStream(netstream::Session_t session, ACE_Message_Block & msg_block, PacketVec_t & packet_vec)
{
	while (true)
	{
		if (msg_block.length() < Packet::head_size())
		{
			return;
		}

		Packet * ps = new Packet();

		memcpy(ps->ch_head(), msg_block.rd_ptr(), ps->head_size());

		if (msg_block.length() >= ps->head_size() + ps->body_size())
		{
			msg_block.rd_ptr(ps->head_size());

			memcpy(ps->ch_body(), msg_block.rd_ptr(), ps->body_size());

			msg_block.rd_ptr(ps->body_size());

			ps->setOwner(session);
			packet_vec.push_back(ps);
		}
		else
		{
			delete ps;
			break;
		}
	}
}
	



SavePackInfo::SavePackInfo()
: is_save_stream(false)
, file_stream(NULL)
{}

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
		sprintf(file_str, "%s_%d_%d_%dT%d_%d_%d"
			, file_name.c_str(), curr_time.year()
			, curr_time.month(), curr_time.day()
			, curr_time.hour(), curr_time.minute(), curr_time.second());
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
: session_state_(SS_NONE)
, handle_input_(NULL)
,client_side_(false)
{
	sync_in_buf_.init(s_socket_buf_len_);
	sync_out_buf_.init(s_socket_buf_len_);
}

Session::Session(bool client)
	: session_state_(SS_NONE)
	, handle_input_(NULL)
	,client_side_(client)
{
	sync_in_buf_.init(s_socket_buf_len_);
	sync_out_buf_.init(s_socket_buf_len_);
}

Session::~Session()
{

}

int Session::open(void * p)
{
	return session_on_connected();
}

int Session::handle_input(ACE_HANDLE  fd)
{
	return session_on_read();
}

int Session::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{	
	// ÏÈÈÃread reactor ÏÈ handle close
	if (ACE_Event_Handler::WRITE_MASK == close_mask)
	{
		return 0;
	}

	// READ_MASK
	if (SS_CLOSED != session_state_)
	{
		session_state_ = SS_CLOSED;
		session_on_closed();
	}
	
	return 0;
}

bool Session::session_async_write(char * buffer, int buff_size)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, async_out_mutex_, false);
	if (async_out_buf_.space() >= buff_size)
	{
		async_out_buf_.copy(buffer, buff_size);
		return true;
	}
	else
	{
		return false;
	}
}

void Session::setSocketBufferSize(int input_buf_size, int output_buf_size)
{
	if (input_buf_size > s_socket_buf_len_)
	{
		sync_in_buf_.init(input_buf_size);
	}

	if (output_buf_size > s_socket_buf_len_)
	{
		sync_out_buf_.init(output_buf_size);
	}
}

int Session::session_on_read()
{
	int result = 0;

	LOG_INPUT_MSG_SIZE();

	sync_in_buf_.crunch();

	LOG_INPUT_MSG_SIZE();

	int recv_n = (int)this->peer().recv(sync_in_buf_.wr_ptr(), sync_in_buf_.space());
	if (recv_n > 0)
	{
		LOG_INPUT_MSG_SIZE();

		sync_in_buf_.wr_ptr(recv_n);

		LOG_INPUT_MSG_SIZE();

		handle_input_->ISessionIn_sync_read(this, sync_in_buf_);
	}
	else if (0 == recv_n)
	{
		// remote normal close
		//DEF_LOG_ERROR("remote stream close normally, last error is <%d>\n", ACE_OS::last_error());
		//string debug_str = "remote stream close normally, last error is : " + boost::lexical_cast<string>(ACE_OS::last_error()) + " buffer length : " + boost::lexical_cast<string>(sync_in_buf_.space()) + "\n";
		//std::cout << debug_str;
		session_on_read_error(recv_n, ACE_OS::last_error());
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
			session_on_read_error(recv_n, ACE_OS::last_error());
			result = -1;
		}
	}

	return result;
}

int Session::session_sync_write()
{
	// todo
	int result = 0;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, async_out_mutex_, -1);
		if ((async_out_buf_.length() == 0) && (sync_out_buf_.length() == 0))
		{
			// no data to send
			return 1;
		}

		if ((sync_out_buf_.space() > 0) && (async_out_buf_.length() > 0))
		{
			auto copy_size = async_out_buf_.length() < sync_out_buf_.space() ? async_out_buf_.length() : sync_out_buf_.space();
			sync_out_buf_.copy(async_out_buf_.rd_ptr(), copy_size);
			async_out_buf_.rd_ptr(copy_size);
			async_out_buf_.crunch();
		}
	}


	if (sync_out_buf_.length() > 0)
	{
		auto send_n = this->peer().send(sync_out_buf_.rd_ptr(), sync_out_buf_.length());
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
			sync_out_buf_.rd_ptr(send_n);

			if (sync_out_buf_.length() == 0)
			{
				result = 0;
			}
			else
			{
				result = 2;
			}
		}
	}
	else
	{
		result = 1;
	}

	if (sync_out_buf_.length() < 500)
	{
		sync_out_buf_.crunch();
	}

	return result;
}

void Session::setHandleInput(ISessionIn * a_input)
{
	handle_input_ = a_input;
}

void Session::session_on_read_error(int recv_num, int last_err)
{

}

void Session::setSavePackInfo(bool is_save, const string & file_name)
{
	//m_save_input_pack_info.init(is_save, file_name);
	//m_save_input_pack_info.init(false, file_name);
}

int Session::session_on_connected()
{
	return 0;
}

int Session::session_on_closed()
{
	return 0;
}


int Session::regReadEvent()
{
	this->peer().enable(ACE_NONBLOCK);
	int flag = 1;
	this->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

	if (super::open() == -1)
		return -1;

	session_state_ = SS_CONNECTED;
	session_on_connected();
	return 0;
}

}