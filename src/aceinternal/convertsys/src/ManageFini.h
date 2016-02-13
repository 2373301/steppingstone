
#ifndef MANAGE_FINI_HPP
#define MANAGE_FINI_HPP

#include <ace/Event_Handler.h>

int startConvert(int argc, char * argv[]);

/***
 * @class SerializeFactory
 *
 * @brief 管理convert的初始化操作以及反初始化操作
 */
class ManageFini : public ACE_Event_Handler
{
public:
	ManageFini();
	~ManageFini();
public:
	/// 初始化操作
	int init(int argc, ACE_TCHAR * argv[]);

	/// 设置注册timer的ID
	void setTimerID(long timer_id);
public:
	/// ACE_Event_Handler handle_timeout，用于侦测推出事件，当在控制台输入stop convert时，程序退出
	virtual int  handle_timeout (const ACE_Time_Value &current_time, const void *act=0);

private:
	/// 反初始化函数
	void stop();

private:
	long m_timer_id;
};

#endif