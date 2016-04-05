
#include "ManageJobInit.h"
#include "ManageConfig.h"

int ManageJobInit::init()
{
	if (loadConfig(ManageConfig::instance()->getInitJobCfg()))
	{
		return 0;
	}

	return -1;
}

bool ManageJobInit::getJobInitCfg(int job, JobInitCfg & job_init_cfg)
{
	JobInitCfgMap_t::iterator it = m_job_init_cfg_map.find(job);
	if (it != m_job_init_cfg_map.end())
	{
		job_init_cfg = it->second;
		return true;
	}

	return false;
}

int ManageJobInit::getRoleId(int job, int sex)
{
	int mix_id = getMixId(job, sex);
	JobInitCfgMap_t::iterator it = m_job_init_cfg_map.find(mix_id);
	if (m_job_init_cfg_map.end() != it)
	{
		return it->second.role_id;
	}

	return -1;
}

bool ManageJobInit::loadConfig(const string & cfg_path)
{
	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (doc.get() == NULL)
	{
		return -1;
	}

	if (doc->load_file(cfg_path))
	{
		Element * root_ele = doc->get_root();
		return loadInitCfg(root_ele);
	}

	return false;
}

bool ManageJobInit::loadInitCfg(Element * root_ele)
{
	if (NULL == root_ele)
	{
		return false;
	}

	Elements init_eles = root_ele->get_elements("char");

	bool result = true;

	for (Elements::iterator it = init_eles.begin(); it != init_eles.end(); ++it)
	{
		Element * init_ele = *it;
		result = loadCharCfg(init_ele) && result;
	}

	return result;
}

bool ManageJobInit::loadCharCfg(Element * char_ele)
{
	if (NULL == char_ele)
	{
		return false;
	}

	JobInitCfg job_init_cfg;	

	bool result = true;

	result = getAttr(char_ele, "job", job_init_cfg.job);
	result = getAttr(char_ele, "sex", job_init_cfg.sex);
	result = getAttr(char_ele, "roleid", job_init_cfg.role_id);

	int mix_id = getMixId(job_init_cfg.job, job_init_cfg.sex);
	JobInitCfgMap_t::iterator it = m_job_init_cfg_map.find(mix_id);
	if (it == m_job_init_cfg_map.end())
	{
		m_job_init_cfg_map.insert(std::make_pair(mix_id, job_init_cfg));
	}
	else
	{
		return false;
	}

	return result;
}

bool ManageJobInit::getAttr(Element * element, const string & attr_name, int & attr_value)
{
	Attribute * attr = element->get_attribute(attr_name);
	if (NULL ==  attr)
	{
		return false;
	}

	attr_value = attr->as_int();
	return true;
}

bool ManageJobInit::getAttr(Element * element, const string & attr_name, string & attr_value)
{
	Attribute * attr = element->get_attribute(attr_name);
	if (NULL ==  attr)
	{
		return false;
	}

	attr_value = attr->as_cstr();
	return true;
}

int ManageJobInit::getMixId(int job, int sex)
{
	return job * 2 + sex;
}