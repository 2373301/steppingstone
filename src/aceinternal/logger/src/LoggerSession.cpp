
#include <ace/os_include/netinet/os_tcp.h>
#include "LoggerSession.h"

LoggerSession::LoggerSession()
{
}

LoggerSession::~LoggerSession()
{
	this->peer().close();
}

int LoggerSession::open(void * p)
{
	if (super::open(p) == -1)
	{
		return -1;
	}

	this->peer().enable(ACE_NONBLOCK);

	int flag = 1;
	this->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

	m_output_block.init(1024 * 1000);

	return 0;
}

int LoggerSession::handle_input(ACE_HANDLE  fd)
{
	return 0;
}

int LoggerSession::handle_output(ACE_HANDLE  fd)
{
	return 0;
}

int LoggerSession::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	return 0;
}

int LoggerSession::handleLogBuffer(vector<char *> & log_buffer_vec)
{
	int result = 0;
	for (vector<char *>::iterator it = log_buffer_vec.begin(); it != log_buffer_vec.end(); ++it)
	{
		int str_size = strlen(*it);
		if (m_output_block.space() > str_size)
		{
			m_output_block.copy(*it, str_size);
		}
		else
		{
			int send_r = sendLogBuffer();
			if (-1 == send_r)
			{
				result = -1;
				break;
			}
			else
			{
				m_output_block.copy(*it, str_size + 1);
			}
		}
	}

	int send_r = sendLogBuffer();
	if (-1 == send_r)
	{
		result = -1;
	}

	return result;
}

int LoggerSession::recv_data(ACE_Message_Block & msg_block)
{
	int result = 0;
	int r = this->peer().recv(msg_block.wr_ptr(), msg_block.space());
	if (r > 0)
	{
		msg_block.wr_ptr(r);
		result = 1;
	}
	else
	{
		int last_error = ACE_OS::last_error();
		if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error))
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

int LoggerSession::sendLogBuffer()
{
	int result = 0;
	while (true)
	{
		int send_r = this->peer().send(m_output_block.rd_ptr(), m_output_block.length());
		if (send_r > 0)
		{
			m_output_block.rd_ptr(send_r);
		}
		else
		{
			int last_error = ACE_OS::last_error();
			if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error))
			{
				// do nothing
			}
			else
			{
				result = -1;
				break;
			}
		}

		if (m_output_block.length() == 0)
		{
			m_output_block.crunch();
			break;
		}
	}

	return result;
}