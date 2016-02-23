#ifndef _HANDLE_GS_STREAM_HPP
#define _HANDLE_GS_STREAM_HPP

enum GSStreamType
{
	GSST_Base = 0,
	GSST_Input,
	GSST_Output
};

class HandleGSStream
{
public:
	HandleGSStream();

	virtual ~HandleGSStream();

	virtual int doing() = 0;
protected:
	GSStreamType m_type;
};

#endif
