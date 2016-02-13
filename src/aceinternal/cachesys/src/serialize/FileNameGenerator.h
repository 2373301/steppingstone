#ifndef FILE_NAME_GENERATOR_HPP
#define FILE_NAME_GENERATOR_HPP

#include <string>
#include <ace/Time_Value.h>

using namespace std;

/***
 * @class FileNameGenerator
 *
 * @brief �����ļ����ƣ�cacheÿ��һ��ʱ�����һ���µ�binary�ļ�
 */
class FileNameGenerator
{
public:
	FileNameGenerator();

	~FileNameGenerator();
public:
	/// �ж�ʱ���Ƿ���ڣ����ڲ�����һ��binary�ļ�
	bool   expire();

	/// �����µ�binary�ļ�����
	string getFileName();

protected:
private:
	int m_generate_interval;

	ACE_Time_Value m_last_generate_time;
};
#endif