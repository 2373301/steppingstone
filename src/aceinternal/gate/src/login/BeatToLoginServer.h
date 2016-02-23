
#ifndef _BEAT_TO_LOGIN_SERVER_HPP
#define _BEAT_TO_LOGIN_SERVER_HPP

#include <ace/Event_Handler.h>

class BeatToLoginServer : public ACE_Event_Handler
{
public:
	BeatToLoginServer();
	~BeatToLoginServer();
public:
	virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act=0);

private:
	void beatToLogin();
};

#endif