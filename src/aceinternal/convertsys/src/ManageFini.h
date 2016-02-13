
#ifndef MANAGE_FINI_HPP
#define MANAGE_FINI_HPP

#include <ace/Event_Handler.h>

int startConvert(int argc, char * argv[]);

/***
 * @class SerializeFactory
 *
 * @brief ����convert�ĳ�ʼ�������Լ�����ʼ������
 */
class ManageFini : public ACE_Event_Handler
{
public:
	ManageFini();
	~ManageFini();
public:
	/// ��ʼ������
	int init(int argc, ACE_TCHAR * argv[]);

	/// ����ע��timer��ID
	void setTimerID(long timer_id);
public:
	/// ACE_Event_Handler handle_timeout����������Ƴ��¼������ڿ���̨����stop convertʱ�������˳�
	virtual int  handle_timeout (const ACE_Time_Value &current_time, const void *act=0);

private:
	/// ����ʼ������
	void stop();

private:
	long m_timer_id;
};

#endif