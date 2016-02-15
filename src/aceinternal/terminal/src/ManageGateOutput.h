
#ifndef MANAGE_GATE_OUTPUT_HPP
#define MANAGE_GATE_OUTPUT_HPP

#include <ace/Task.h>
#include "typedef.h"
#include "GateTerminal.h"

class ManageGateOutput : public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageGateOutput();
	~ManageGateOutput();
public:
	int init(int thread_no);

	int stop();

public:
	int svc();

public:
	void handleGateTerminal(GateTerminal * gate_terminal);

protected:
private:
	deque<GateTerminal *> m_gate_terminal_qeque;

	ACE_Thread_Mutex m_gate_terminal_qeque_mutex;

	bool m_is_stop;
};
#endif