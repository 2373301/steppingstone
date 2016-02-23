
#ifndef _APPNOTIFYINTERFACE_HPP
#define _APPNOTIFYINTERFACE_HPP

#include <vector>
#include <ace/ace_wchar.h>
#include "Singleton.h"

using std::vector;

/***
 * @class AppNotifyInterface
 *
 * @brief ȫ�ֵ�ʵ�������ӿڣ���Ҫ���ڳ�ʼ���ͷ���ʼ������
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
 * @brief ȫ�ֵ�ʵ�������ӿ�ʵ���Ĺ�����
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
	///���һ��ȫ��ʵ������
	void addApp(AppNotifyInterface * app_notify);

	/// ��ʼ������ȫ��ʵ������
	int initApp(int argc, ACE_TCHAR * argv[]);

	/// ����ʼ��ȫ��ʵ������
	void finiApp();

	/// �ȴ�����ȫ��ʵ�����󷴳�ʼ������
	void waitApp();

	/// ����
	void clear();
private:
	AppNotifyVec_t m_app_notify_vec;
};

#endif