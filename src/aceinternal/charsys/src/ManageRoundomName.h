
#ifndef MANAGE_ROUNDOM_NAME_HPP
#define MANAGE_ROUNDOM_NAME_HPP

#include <boost/random.hpp>
#include "Singleton.h"
#include "SingletonInit.h"
#include "typedef.h"
#include "SimpleXML.h"

typedef vector<string> StrVec_t;

struct RandomNameInfo 
{
	RandomNameInfo()
	: m_rand_engineer(::time(NULL))
	, m_rand_distribution(0, 100000)
	, m_rand_generator(m_rand_engineer, m_rand_distribution)
	{
	}

	void getNewName(int sex, string & new_name)
	{
		int random_int = m_rand_generator();
		int index = random_int % surname.size();
		new_name = surname[index];

		if (0 == sex)
		{
			index = m_rand_generator() % female_name.size();
			new_name += female_name[index];
		}
		else
		{
			index = m_rand_generator() % male_name.size();
			new_name += male_name[index];
		}
	}

	boost::mt19937 m_rand_engineer;

	boost::uniform_int<>  m_rand_distribution;

	boost::variate_generator<boost::mt19937, boost::uniform_int<> > m_rand_generator;

	StrVec_t surname;
	StrVec_t male_name;
	StrVec_t female_name;
};

class ManageRoundomName : public SingletonInit
{
public:
	static ManageRoundomName * instance()
	{
		return Singleton<ManageRoundomName>::instance();
	}
public:
	virtual int init();

	bool loadConfig(const string & cfg_path);

	bool loadNameCfg(Element * root_ele);

	void getNewName(int sex, string & new_name);

protected:
private:
	RandomNameInfo m_random_name_info;
};

#endif