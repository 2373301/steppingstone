#ifndef _HANDLE_CLIENT_STREAM_HPP
#define _HANDLE_CLIENT_STREAM_HPP

enum ClientStreamType
{
	CST_Base = 0,
	CST_Input,
	CST_Output
};

class HandleClientStream
{
public:
	HandleClientStream();

	virtual ~HandleClientStream();

	virtual int doing() = 0;
protected:
	ClientStreamType m_type;
};

#endif
