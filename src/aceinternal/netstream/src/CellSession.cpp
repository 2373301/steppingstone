
#include "CellSession.h"
#include "ManageNetEventNotify.h"

namespace netstream
{

CellSession::CellSession()
: m_guid(0)
{
	async_out_buf_.init(s_socket_buf_len_);
}

CellSession::CellSession(bool client)
	: m_guid(0)
	,Session(client)
{
	async_out_buf_.init(s_socket_buf_len_);
}


CellSession::~CellSession()
{

}

int CellSession::session_on_connected()
{
	ManageNetEventNotify::instance()->sessionOpenNotify(this);
	return 0;
}

int CellSession::session_on_read()
{
	int result = Session::session_on_read();
	if (-1 == result)
	{
		// net close
		ManageNetEventNotify::instance()->sessionCloseNotify(this, ACE_Event_Handler::READ_MASK);
	}

	return result;
}


int CellSession::syncWrite()
{
	int result = Session::syncWrite();
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
