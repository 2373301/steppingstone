
#include "GateLogger.h"
#include "AppNotifyInterface.h"

AppNotifyInterface::AppNotifyInterface()
{
	ManageAppNotifyInterface::instance()->addApp(this);
}

//////////////////////////////////////////////////////////////////////////

ManageAppNotifyInterface::ManageAppNotifyInterface()
{
}

ManageAppNotifyInterface::~ManageAppNotifyInterface()
{
}

void ManageAppNotifyInterface::addApp(AppNotifyInterface * app_notify)
{
	m_app_notify_vec.push_back(app_notify);
}

int ManageAppNotifyInterface::initApp(int argc, ACE_TCHAR * argv[])
{
	GATE_LOG_INFO(ACE_TEXT("Start to init all object\n"));
	int result = 0;

	///按照先后顺序初始化
	for (AppNotifyVec_t::iterator it = m_app_notify_vec.begin(); it != m_app_notify_vec.end(); ++it)
	{
		if ((*it)->init(argc, argv) == -1)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to call ManageAppNotifyInterface::doInit\n"));
			result = 1;
			break;
		}
	}
	return result;
}

void ManageAppNotifyInterface::finiApp()
{
	GATE_LOG_INFO(ACE_TEXT("Start to fini all object\n"));
	///按照后初始化先反初始化的顺序
	for (AppNotifyVec_t::reverse_iterator it = m_app_notify_vec.rbegin(); it != m_app_notify_vec.rend(); ++it)
	{
		(*it)->fini();
	}
}

void ManageAppNotifyInterface::waitApp()
{
	GATE_LOG_INFO(ACE_TEXT("Start to wait all object\n"));
	///按照后初始化先反初始化的顺序
	for (AppNotifyVec_t::reverse_iterator it = m_app_notify_vec.rbegin(); it != m_app_notify_vec.rend(); ++it)
	{
		(*it)->wait();
	}
}

void ManageAppNotifyInterface::clear()
{
	m_app_notify_vec.clear();
}