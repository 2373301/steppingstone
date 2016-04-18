
#include <ace/OS.h>
#include "OutputSessionPool.h"
#include "Logger.h"

namespace netcore 
{

OutputSessionPool::OutputSessionPool()
: m_session_thread_info_index(0)
, m_stop(false)
, m_wait(true)
, m_actived(false)
, m_session_pool(NULL)
{

}

OutputSessionPool::~OutputSessionPool()
{

}

int OutputSessionPool::init(int thread_no, SessionPoolx * session_pool)
{
	if (this->activate(THR_NEW_LWP, thread_no) == -1)
	{
		return -1;
	}

	if (!m_actived)
	{
		ACE_OS::sleep(1);
	}

	m_session_pool = session_pool;

	return 0;
}

void OutputSessionPool::stop()
{
	m_stop = true;
}

void OutputSessionPool::finit()
{
	while(m_wait)
	{
		//std::cout << "wait OutputSessionPool" << std::endl;
		ACE_OS::sleep(1);
	}

	ACE_OS::sleep(2);

	//std::cout << "exit OutputSessionPool" << std::endl;
}

int OutputSessionPool::svc()
{
	REPORT_THREAD_INFO();

	OutputSessionThreadInfo output_session_thread_info;

	registerOutputSessionThreadinfo(&output_session_thread_info);

	m_actived = true;

	CellSessionSet_t cell_session_set;

	int write_result = 1;

	ACE_Time_Value sleep_time(0, 1000);

	while (!m_stop)
	{
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, output_session_thread_info.mutex, -1);
			for (CellSessionSet_t::iterator it = output_session_thread_info.remove_cell_session_set.begin(); it != output_session_thread_info.remove_cell_session_set.end(); ++it)
			{
				CellSessionSet_t::iterator find_it = cell_session_set.find(*it);
				if (find_it != cell_session_set.end())
				{
					CellSessionx * cell = *find_it;
					cell_session_set.erase(find_it);
					m_session_pool->removeSession(cell);
				}
				else
				{
					// error
				}
			}
			output_session_thread_info.remove_cell_session_set.clear();

			for (CellSessionSet_t::iterator it = output_session_thread_info.add_cell_session_set.begin(); it != output_session_thread_info.add_cell_session_set.end(); ++it)
			{
				cell_session_set.insert(*it);
			}
			output_session_thread_info.add_cell_session_set.clear();
		}

		write_result = 1;
		for (CellSessionSet_t::iterator it = cell_session_set.begin(); it != cell_session_set.end(); )
		{
			CellSessionx * cell_session = *it;
			if (NULL == cell_session)
			{
				continue;
			}
			int wr = cell_session->wt_stream();
			if (-1 == wr)
			{
				cell_session_set.erase(it++);
			}
			else if (1 == wr)
			{
				// empty write
				++it;
			}
			else
			{
				write_result = 0;
				++it;
			}

		}

		if (1 == write_result)
		{
			// all empty write
			ACE_OS::sleep(sleep_time);
		}
	}

	m_wait = false;

	return 0;
}

void OutputSessionPool::handleSession(CellSessionx * cell_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_session_thread_info_vec_mutex, );
	if (m_output_session_thread_info_vec.size() == 0)
	{
		// error

		return ;
	}

	int index = m_session_thread_info_index++ % m_output_session_thread_info_vec.size();

	OutputSessionThreadInfo * session_thread_info = m_output_session_thread_info_vec[index];

	session_thread_info->addCellSession(cell_session);

	m_cell_session_map[cell_session] = session_thread_info;
}

void OutputSessionPool::removeSession(CellSessionx * cell_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_session_thread_info_vec_mutex, );
	CellSessionMap_t::iterator it = m_cell_session_map.find(cell_session);
	if (it != m_cell_session_map.end())
	{
		OutputSessionThreadInfo * session_thread_info = it->second;
		session_thread_info->removeCellSession(cell_session);

		m_cell_session_map.erase(it);
	}
	else
	{
		// error
	}
}

void OutputSessionPool::registerOutputSessionThreadinfo(OutputSessionThreadInfo * output_session_thread_info)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_session_thread_info_vec_mutex, );
	m_output_session_thread_info_vec.push_back(output_session_thread_info);
}


}
