
#include <ace/OS.h>
#include "ManageGateOutput.h"
#include "Logger.h"

ManageGateOutput::ManageGateOutput()
: m_is_stop(false)
{}

ManageGateOutput::~ManageGateOutput()
{}

int ManageGateOutput::init(int thread_no)
{
	if (this->activate(THR_NEW_LWP, thread_no) == -1)
	{
		DEF_LOG_ERROR("error to active ManageGateOutput, thread no is <%d>, last error is <%d>\n", thread_no, ACE_OS::last_error());
		return -1;
	}

	return 0;
}

int ManageGateOutput::stop()
{
	m_is_stop = true;
	return 0;
}

int ManageGateOutput::svc()
{
	REPORT_THREAD_INFO();
	GateTerminal * gate_terminal = NULL;

	while(!m_is_stop)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gate_terminal_qeque_mutex, -1);
			if (m_gate_terminal_qeque.size() > 0)
			{
				gate_terminal = m_gate_terminal_qeque.front();
				m_gate_terminal_qeque.pop_front();
				break;
			}

		}
		ACE_OS::sleep(1);
	}

	ACE_Time_Value sleep_time(0, 1 * 1000);

	int process_result = 0;
	while (!m_is_stop)
	{
		process_result = gate_terminal->syncWrite();

		if (1 == process_result)
		{
			ACE_OS::sleep(sleep_time);
		}
		else if (-1 == process_result)
		{
			break;
		}
	}
	return 0;
}

void ManageGateOutput::handleGateTerminal(GateTerminal * gate_terminal)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_gate_terminal_qeque_mutex, );
	m_gate_terminal_qeque.push_back(gate_terminal);
}
