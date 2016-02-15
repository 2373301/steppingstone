
#include "ManageWarJobEnrageRes.h"
#include "Logger.h"

bool ManageWarJobEnrageRes::loadWarJobEnrageContent(Document * xml_doc)
{
	if (NULL == xml_doc)
	{
		return false;
	}

	Element * root_ele = xml_doc->get_root();

	if (NULL == root_ele)
	{
		return false;
	}

	bool result = true;
	Elements warjob_ele_list = root_ele->get_elements();
	for (Elements::iterator it = warjob_ele_list.begin(); it != warjob_ele_list.end(); ++it)
	{
		result = loadWarJobInfo(*it) && result;
	}

	return result;

}

const WarJobEnrageResInfoMap_t & ManageWarJobEnrageRes::getWarJobEnrageResInfo()
{
	return m_war_job_enrage_res_info;
}

bool ManageWarJobEnrageRes::loadWarJobInfo(Element * warjob_element)
{
	if (NULL == warjob_element)
	{
		return false;
	}

	bool result = true;

	WarJobEnrageRes * warjob_info = new WarJobEnrageRes();

	result = getAttrValue(warjob_element, "job", warjob_info->job_id) && result;
	result = getAttrValue(warjob_element, "mana_range", warjob_info->mana_range) && result;
	string attr_name;
	int32 attr_value;
	for (int i = 1; i <= 3; ++i)
	{
		attr_name = "hit_type" + boost::lexical_cast<string>(i);
		if (getAttrValue(warjob_element, attr_name, attr_value))
		{
			warjob_info->hit.push_back(attr_value);
		}
		else
		{
			result = false;
			break;
		}
	}

	for (int i = 1; i <= 3; ++i)
	{
		attr_name = "uhit_type" + boost::lexical_cast<string>(i);
		if (getAttrValue(warjob_element, attr_name, attr_value))
		{
			warjob_info->un_hit.push_back(attr_value);
		}
		else
		{
			result = false;
			break;
		}
	}

	for (int i = 1; i <= 3; ++i)
	{
		attr_name = "fend_type" + boost::lexical_cast<string>(i);
		if (getAttrValue(warjob_element, attr_name, attr_value))
		{
			warjob_info->dodge.push_back(attr_value);
		}
		else
		{
			result = false;
			break;
		}
	}

	for (int i = 1; i <= 3; ++i)
	{
		attr_name = "ufend_type" + boost::lexical_cast<string>(i);
		if (getAttrValue(warjob_element, attr_name, attr_value))
		{
			warjob_info->un_dodge.push_back(attr_value);
		}
		else
		{
			result = false;
			break;
		}
	}

	for (int i = 1; i <= 3; ++i)
	{
		attr_name = "block_type" + boost::lexical_cast<string>(i);
		if (getAttrValue(warjob_element, attr_name, attr_value))
		{
			warjob_info->block.push_back(attr_value);
		}
		else
		{
			result = false;
			break;
		}
	}

	for (int i = 1; i <= 3; ++i)
	{
		attr_name = "ublock_type" + boost::lexical_cast<string>(i);
		if (getAttrValue(warjob_element, attr_name, attr_value))
		{
			warjob_info->un_block.push_back(attr_value);
		}
		else
		{
			result = false;
			break;
		}
	}

	for (int i = 1; i <= 3; ++i)
	{
		attr_name = "critical_type" + boost::lexical_cast<string>(i);
		if (getAttrValue(warjob_element, attr_name, attr_value))
		{
			warjob_info->critical.push_back(attr_value);
		}
		else
		{
			result = false;
			break;
		}
	}

	for (int i = 1; i <= 3; ++i)
	{
		attr_name = "ucritical_type" + boost::lexical_cast<string>(i);
		if (getAttrValue(warjob_element, attr_name, attr_value))
		{
			warjob_info->un_critical.push_back(attr_value);
		}
		else
		{
			result = false;
			break;
		}
	}

	WarJobEnrageResInfoMap_t::iterator it = m_war_job_enrage_res_info.find(warjob_info->job_id);
	if (m_war_job_enrage_res_info.end() != it)
	{
		DEF_LOG_ERROR("get reduplicate warjob id : <%d>\n", warjob_info->job_id);
		delete warjob_info;
		return false;
	}

	m_war_job_enrage_res_info.insert(std::make_pair(warjob_info->job_id, warjob_info));

	return result;
}
