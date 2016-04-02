
#include "CellSession.h"
#include "ManageNetEventNotify.h"

namespace netstream
{

CellSession::CellSession()
: m_guid(0)
{
	cell_out_buf_.init(s_socket_buf_len_);
}

CellSession::CellSession(bool client)
	: m_guid(0)
	,Session(client)
{
	cell_out_buf_.init(s_socket_buf_len_);
}


CellSession::~CellSession()
{

}

int CellSession::open(void * p)
{
	int result = Session::open(p);
	if (-1 != result)
	{
		ManageNetEventNotify::instance()->sessionOpenNotify(this);
	}

	return result;
}

int CellSession::rd_stream()
{
	int result = Session::rd_stream();
	if (-1 == result)
	{
		// net close
		ManageNetEventNotify::instance()->sessionCloseNotify(this, ACE_Event_Handler::READ_MASK);
	}

	return result;
}

bool CellSession::output(char * buffer, int buff_size)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, cell_out_mutex_, false);
	if (cell_out_buf_.space() >= buff_size)
	{
		cell_out_buf_.copy(buffer, buff_size);
		return true;
	}
	else
	{
		return false;
	}
}

int CellSession::wt_stream()
{
	int result = 0;

	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, cell_out_mutex_, -1);
		if ((cell_out_buf_.length() == 0) && (out_buf_.length() == 0))
		{
			// no data to send
			return 1;
		}

		if ((out_buf_.space() > 0) && (cell_out_buf_.length() > 0))
		{
			auto copy_size = cell_out_buf_.length() < out_buf_.space() ? cell_out_buf_.length() : out_buf_.space();
			out_buf_.copy(cell_out_buf_.rd_ptr(), copy_size);
			cell_out_buf_.rd_ptr(copy_size);
			cell_out_buf_.crunch();
		}
	}

	if (out_buf_.length() > 0)
	{
		auto send_n = this->peer().send(out_buf_.rd_ptr(), out_buf_.length());
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
			out_buf_.rd_ptr(send_n);

			if (out_buf_.length() == 0)
			{
				result = 0;
			}
			else
			{
				result = 2;
			}
		}
	}

	if (out_buf_.length() < 500)
	{
		out_buf_.crunch();
	}

	if (-1 == result)
	{
		ManageNetEventNotify::instance()->sessionCloseNotify(this, ACE_Event_Handler::WRITE_MASK);
	}

	return result;
}

uint64 CellSession::getGUID()
{
	return m_guid;
}

void CellSession::setGUID(uint64 guid)
{
	m_guid = guid;
}

}
