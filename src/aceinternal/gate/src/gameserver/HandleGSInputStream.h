#ifndef _HANDLE_GS_INPUT_STREAM_HPP
#define _HANDLE_GS_INPUT_STREAM_HPP

#include "HandleGSStream.h"
#include "GSSession.h"

class HandleGSInputStream : public HandleGSStream
{
public:
	HandleGSInputStream(GSSession * gs_session);

	~HandleGSInputStream();

	virtual int doing();
private:
	GSSession * m_gs_session;
};

#endif