
#ifndef MANAGE_JOB_INIT_HPP
#define MANAGE_JOB_INIT_HPP

#include "Singleton.h"
#include "SingletonInit.h"
#include "typedef.h"
#include "SimpleXML.h"

struct JobInitCfg 
{
	JobInitCfg()
	: job(0)
	, sex(0)
	, role_id(0)
	{}

	int job;
	int sex;
	int role_id;
};

class ManageJobInit : public SingletonInit
{
public:
	static ManageJobInit * instance()
	{
		return Singleton<ManageJobInit>::instance();
	}
public:
	virtual int init();

	bool getJobInitCfg(int job, JobInitCfg & job_init_cfg);

	int getRoleId(int job, int sex);
protected:
	bool loadConfig(const string & cfg_path);

	bool loadInitCfg(Element * root_ele);

	bool loadCharCfg(Element * char_ele);

	bool getAttr(Element * element, const string & attr_name, int & attr_value);

	bool getAttr(Element * element, const string & attr_name, string & attr_value);

	int getMixId(int job, int sex);

private:
	typedef map<int, JobInitCfg> JobInitCfgMap_t;

	JobInitCfgMap_t m_job_init_cfg_map;
};
#endif