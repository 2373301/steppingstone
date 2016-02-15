
#ifndef MANAGE_SINGLE_TERMINAL_HPP
#define MANAGE_SINGLE_TERMINAL_HPP

#include <ace/Task.h>
#include "typedef.h"
#include "ManageTerminal.h"
#include "SingleTerminal.h"

class ManageSingleTerminal : public ManageTerminal, public ACE_Task<ACE_NULL_SYNCH>, public HandleInputStream
{
public:
	ManageSingleTerminal();

public:
	virtual int stop();

	virtual int finit();

public:
	// svc of ACE_Task
	virtual int svc();

	virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act /* = 0 */);
public:
	virtual void input(Session * session, ACE_Message_Block & msg_block);

	virtual void output(Packet * packet);

	virtual void registerScene(uint64 scene_id, uint64 scend_type);
public:
	static ManageSingleTerminal * instance()
	{
		if (NULL == m_static_instance)
		{
			m_static_instance = new ManageSingleTerminal();
		}
		return m_static_instance;
	}

	void registerSingleTerminal(SingleTerminal * single_terminal);

	void removeSingleTerminal(SingleTerminal * single_terminal);

protected:
	virtual int initing();

	bool addRoute(uint64 guid, void * terminal);

	void removeRoute(uint64 guid);

private:
	static ManageSingleTerminal * m_static_instance;

private:
	typedef map<uint64, SingleTerminal *>	SingleTerminalMap_t;

	typedef vector<SingleTerminal *>		SingleTerminalVec_t;

	ACE_Reactor * m_reactor;

	int m_max_epoll_socket;

	int m_timer_id;

	ACE_Thread_Mutex	m_single_terminal_mutex;

	SingleTerminalMap_t m_single_terminal_map;

	ACE_Thread_Mutex	m_single_terminal_map_mutex;

	SingleTerminalVec_t m_single_terminal_vec;

	ACE_Thread_Mutex	m_deleting_single_terminal_vec_mutex;

	SingleTerminalVec_t m_deleting_single_terminal_vec;
};

#endif