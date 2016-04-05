
#ifndef _MANAGEFINI_HPP
#define _MANAGEFINI_HPP

#include <ace/Event_Handler.h>

int startGate(int argc, char * argv[]);

class ManageFini : public ACE_Event_Handler
{
public:
	ManageFini();
	~ManageFini();
public:
	int init(int argc, ACE_TCHAR * argv[]);

	void setTimerID(long timer_id);
public:
	/// ACE_Event_Handler handle_timeout
	virtual int  handle_timeout (const ACE_Time_Value &current_time, const void *act=0);
private:
	void stop();

private:
	long m_timer_id;
};

#endif