#ifndef _HANDLE_GS_OUTPUT_STREAM_HPP
#define _HANDLE_GS_OUTPUT_STREAM_HPP

#include "HandleGSStream.h"
#include "GSSession.h"

class HandleGSOutputStream : public HandleGSStream
{
public:
	HandleGSOutputStream(GSSession * gs_session);

	~HandleGSOutputStream();

	virtual int doing();
private:
	GSSession * m_gs_session;
};
#endif