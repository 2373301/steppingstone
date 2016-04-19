
#include <boost/lexical_cast.hpp>
#include <ace/OS.h>
#include "ManageTimer.h"
#include "opcode.h"
#include "Logger.h"
#include "share_fun.h"

TimerEvent::TimerEvent(netstream::IInput * handle_input)
: m_timer_id(0)
, m_handle_input(handle_input)
{
}

int TimerEvent::handle_timeout (const ACE_Time_Value &current_time, const void *act)
{
	MAKE_NEW_PACKET(packet, SMSG_TIMER_OCCUR, m_timer_id, Packet::empty_stream);
	m_handle_input->IInput_input(packet);
	return 0;
}

void TimerEvent::set_timer_id (int timer_id)
{
	m_timer_id = timer_id;
}


ManageTimer::ManageTimer(netstream::IInput * handle_input)
: m_handle_input(handle_input)
, m_is_trigger(false)
, m_is_stop(false)
{
	m_timer_queue = new ACE_Timer_Heap();
}

ManageTimer::~ManageTimer()
{
	for (TimerEventMap_t::iterator it = m_timer_event_map.begin(); it != m_timer_event_map.end(); ++it)
	{
		m_timer_queue->cancel(it->first);
	}

	delete m_timer_queue;

	cleanMapSecondContent(m_timer_event_map);
	
}

int ManageTimer::svc()
{
	REPORT_THREAD_INFO();

	ACE_Time_Value sleep_time(0, 100 * 1000);
	ACE_Time_Value curr_time;
	ACE_Time_Value * curr_timeout;
	while (!m_is_stop)
	{
		if (!m_is_trigger)
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		if (m_timer_queue->is_empty())
		{
			ACE_OS::sleep(sleep_time);
			continue;
		}

		curr_time = m_timer_queue->gettimeofday ();

		curr_timeout = m_timer_queue->calculate_timeout(&curr_time);

		if (*curr_timeout == ACE_Time_Value::zero)
		{
			m_timer_queue->expire();
		}
		else
		{
//			DEF_LOG_DEBUG("sleep time , year<%s>, \n", boost::lexical_cast<string>(next_timeout_time.sec()).c_str());
			ACE_OS::sleep(*curr_timeout);
			m_timer_queue->expire();
		}
	}
	return 0;
}

int ManageTimer::init()
{
	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

int ManageTimer::stop()
{
	m_is_stop = true;
	return 0;
}

void ManageTimer::trigger()
{
	m_is_trigger = true;
}

long ManageTimer::ITimer_scheme(int interval_value)
{
	TimerEvent * timer_event = new TimerEvent(m_handle_input);
	ACE_Time_Value tv(interval_value / 1000, (interval_value % 1000) * 1000);
	long timer_id = m_timer_queue->schedule(timer_event, NULL, ACE_OS::gettimeofday() + tv, tv);
	if (-1 == timer_id)
	{
		// error
		DEF_LOG_FATAL("failed to scheme timer\n");
		delete timer_event;
	}
	else
	{
		timer_event->set_timer_id(timer_id);
		m_timer_event_map[timer_id] = timer_event;
	}

	return timer_id;
}

long ManageTimer::ITimer_cancel(long timer_id)
{
	int cr = m_timer_queue->cancel(timer_id);
	if (-1 == cr)
	{
		DEF_LOG_FATAL("failed to cancel timer, id is <%d>\n", timer_id);
		return -1;
	}
	else
	{
		TimerEventMap_t::iterator it = m_timer_event_map.find(timer_id);
		if (it != m_timer_event_map.end())
		{
			delete it->second;
			m_timer_event_map.erase(it);
		}
		else
		{
			DEF_LOG_ERROR("failed to find timer id in timer map, id is <%d>\n", timer_id);
		}

		return 0;
	}
}