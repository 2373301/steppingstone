
#include "GSStrategyCenter.h"
#include "GateLogger.h"
#include "ClientStrategyCenter.h"
#include "ManageClientValidation.h"

GSStrategyCenter::GSStrategyCenter()
: m_stop(false)
, m_wait(true)
{
	m_wait = false;
}

GSStrategyCenter::~GSStrategyCenter()
{
}

int GSStrategyCenter::svc(void)
{
	FUNCTIONTRACE(GSStrategyCenter::svc);
	REPORT_THREAD_INFO("GSStrategyCenter::svc", ACE_OS::thr_self());
	return 0;
	while (!m_stop)
	{
		//ACE_OS::sleep(1);
	}

	m_wait = false;
	return 0;
}

int GSStrategyCenter::init(int argc, ACE_TCHAR * argv[])
{
	if (this->activate() == -1)
	{
		GATE_LOG_INFO(ACE_TEXT("Failed to init GSStrategyCenter, last error is <%d>\n"), ACE_OS::last_error());
		return -1;
	}
	else
	{
		GATE_LOG_INFO(ACE_TEXT("Success to init GSStrategyCenter\n"));
		return 0;
	}
}

int GSStrategyCenter::fini()
{
	m_stop = true;
	return 0;
}

int GSStrategyCenter::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for GSStrategyCenter\n");
		ACE_OS::sleep(1);
	}
	return 0;
}

void GSStrategyCenter::GSClosedNotifyClient(GSSession * gs_session)
{
	if (NULL == gs_session)
	{
		return;
	}
	vector<uint64> map_id_vec = gs_session->getMapID();
	for (vector<uint64>::iterator it = map_id_vec.begin(); it != map_id_vec.end(); ++it)
	{
		uint64 map_id = *it;
		ClientStrategyCenter::instance()->closeClientByMapID(map_id);
		ManageClientValidation::instance()->gsClosed(map_id);
	}
}