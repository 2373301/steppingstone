
#ifndef _APPNOTIFYINTERFACE_HPP
#define _APPNOTIFYINTERFACE_HPP

#include <vector>
#include <ace/ace_wchar.h>
#include "Singleton.h"

using std::vector;

/***
 * @class AppNotifyInterface
 *
 * @brief 全局单实例类抽象接口，主要用于初始化和反初始化操作
 */
class AppNotifyInterface
{
public:
	AppNotifyInterface();

	/// init
	virtual int init(int argc, ACE_TCHAR * argv[]) = 0;

	/// fini, stop
	virtual int fini() = 0;

	/// wait for finish
	virtual int wait() = 0;
};

/***
 * @class AppNotifyInterface
 *
 * @brief 全局单实例类抽象接口实例的管理类
 */
class ManageAppNotifyInterface
{
public:
	typedef vector<AppNotifyInterface * > AppNotifyVec_t;
	
	ManageAppNotifyInterface();

	~ManageAppNotifyInterface();
public:
	static ManageAppNotifyInterface * instance()
	{
		return Singleton<ManageAppNotifyInterface>::instance();
	}
public:
	///添加一个全局实例对象
	void addApp(AppNotifyInterface * app_notify);

	/// 初始化所有全局实例对象
	int initApp(int argc, ACE_TCHAR * argv[]);

	/// 反初始化全局实例对象
	void finiApp();

	/// 等待所有全局实例对象反初始化结束
	void waitApp();

	/// 清理
	void clear();
private:
	AppNotifyVec_t m_app_notify_vec;
};

#endif