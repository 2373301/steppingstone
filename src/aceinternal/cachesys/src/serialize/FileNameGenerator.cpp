
#include "FileNameGenerator.h"
#include "config.h"

#define MAX_PATH 128

FileNameGenerator::FileNameGenerator()
: m_generate_interval(5 * 30)
{
	string time_interval = CONTAINER->lookup(SERIALIZE_INTERVAL);
	int t = atoi(time_interval.c_str());
	m_generate_interval = t > 0 ? t : m_generate_interval;
}

FileNameGenerator::~FileNameGenerator()
{

}

bool FileNameGenerator::expire()
{
	ACE_Time_Value diff = ACE_OS::gettimeofday();
	diff = diff - m_last_generate_time;
	return diff.sec() >= m_generate_interval ? true : false;
}

string FileNameGenerator::getFileName()
{
	m_last_generate_time = ACE_OS::gettimeofday();
	ACE_Date_Time ct(m_last_generate_time);
	char file_name[MAX_PATH] = {0};
	sprintf(file_name, "%d%0.2d%0.2d_%0.2d%0.2d%0.2d.sqlite", ct.year(), ct.month(), ct.day(), ct.hour(), ct.minute(), ct.second());
	return string(file_name);
}