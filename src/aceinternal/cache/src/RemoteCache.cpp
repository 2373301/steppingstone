
#include "RemoteCache.h"
#include "Logger.h"

RemoteCache::RemoteCache()
{

}

RemoteCache::~RemoteCache()
{
}

int RemoteCache::session_on_read()
{
	int result = Session::session_on_read();
	if (-1 == result)
	{
		DEF_LOG_ERROR("occur error in RemoteCache::handle_input, rd_stream return value is -1, last error is <%d>\n", ACE_OS::last_error());
		return -1;
	}
	else
	{
		return 0;
	}
}

void RemoteCache::output(Packet * ps)
{
	Session::IStreamOut_async_write(ps->stream(), ps->stream_size());
	delete ps;
}


void RemoteCache::session_on_read_error(int recv_value, int last_error)
{
	DEF_LOG_ERROR("RemoteCache recv error, recv value is <%d>, last error is <%d>\n", recv_value, last_error);
}

int RemoteCache::finit()
{
	return 0;
}

int RemoteCache::initing()
{
	return 0;
}
