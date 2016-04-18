
#ifndef MANAGE_TIMER_HPP
#define MANAGE_TIMER_HPP

#include <ace/Task.h>
#include <ace/Timer_Queue.h>
#include <ace/Timer_Heap.h>
#include "Session.h"

class TimerEvent 
	: public ACE_Event_Handler
{
public:
	TimerEvent(netstream::HandleInput * handle_input);

	virtual int handle_timeout (const ACE_Time_Value &current_time, const void *act);

	void set_timer_id (int timer_id);
private:
	long m_timer_id;

	netstream::HandleInput * m_handle_input;
};

class ManageTimer 
	: public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageTimer(netstream::HandleInput * handle_input);

	~ManageTimer();

	virtual int svc();


	int init();

	int stop();

	void trigger();

	long schemeTimer(int interval_value);

	long cancelTimer(long timer_id);


private:
	typedef map<long, TimerEvent *> TimerEventMap_t;

	netstream::HandleInput * m_handle_input;

	ACE_Timer_Queue * m_timer_queue;

	TimerEventMap_t m_timer_event_map;

	bool m_is_trigger;

	bool m_is_stop;
};

#endif