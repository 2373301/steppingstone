#ifndef FILE_NAME_GENERATOR_HPP
#define FILE_NAME_GENERATOR_HPP

#include <string>
#include <ace/Time_Value.h>

using namespace std;

/***
 * @class FileNameGenerator
 *
 * @brief 产生文件名称，cache每隔一段时间产生一个新的binary文件
 */
class FileNameGenerator
{
public:
	FileNameGenerator();

	~FileNameGenerator();
public:
	/// 判断时间是否过期，用于产生下一个binary文件
	bool   expire();

	/// 产生新的binary文件名称
	string getFileName();

protected:
private:
	int m_generate_interval;

	ACE_Time_Value m_last_generate_time;
};
#endif