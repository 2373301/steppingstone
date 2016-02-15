
#include "ManageSkillRes.h"
#include "Logger.h"
#include "share_fun.h"

ManageSkillRes::~ManageSkillRes()
{
	cleanMapSecondContent(m_skill_info_map);
}

bool ManageSkillRes::loadSkillContent(Document * xml_doc)
{
	if (NULL == xml_doc)
	{
		return true;
	}

	Element * root_ele = xml_doc->get_root();

	if (NULL == root_ele)
	{
		return false;
	}

	bool result = true;
	Elements skill_ele_list = root_ele->get_elements();
	for (Elements::iterator it = skill_ele_list.begin(); it != skill_ele_list.end(); ++it)
	{
		result = loadSkillInfo(*it) && result;
	}

	return result;
}

const SkillResInfoMap_t & ManageSkillRes::getSkillInfoMap()
{
	return m_skill_info_map;
}

bool ManageSkillRes::loadSkillInfo(Element * skill_element)
{
	if (NULL == skill_element)
	{
		return false;
	}

	bool result = true;

	SkillResInfo * skill_info = new SkillResInfo();

	result = getAttrValue(skill_element, "id", skill_info->skill_id) && result;
	int32 tv = 0;
	result = getAttrValue(skill_element, "type", tv) && result;
	skill_info->type = (SkillType)tv;
	result = getAttrValue(skill_element, "target", tv) && result;
	skill_info->target = (SkillTargetType)tv;
	result = getAttrValue(skill_element, "manaleft", skill_info->manaleft) && result;
	result = getAttrValue(skill_element, "iscritical", skill_info->iscritical) && result;
	result = getAttrValue(skill_element, "isfengding", skill_info->isfengding) && result;
	result = getAttrValue(skill_element, "isblock", skill_info->isblock) && result;
	result = getAttrValue(skill_element, "isantiattack", skill_info->isantiattack) && result;
	result = getAttrValue(skill_element, "hit", skill_info->hit) && result;
	result = getAttrValue(skill_element, "critical", skill_info->critical) && result;
	result = getAttrValue(skill_element, "others", skill_info->others) && result;
	result = getAttrValue(skill_element, "buff_flag", skill_info->buff_flag) && result;
	result = getAttrValue(skill_element, "boss", skill_info->boss) && result;

	//result = getAttrValue(skill_element, "resouce", skill_info->resouce) && result;
	//result = getAttrValue(skill_element, "icon", skill_info->icon) && result;

	Attribute * attr = NULL;
	string fun_name;
	string rate_name;
	string fa_name;
	string fb_name;
	string fc_name;
	string fd_name;
	string fe_name;
	for (int i = 1; i <= 5; ++i)
	{
		fun_name = "function" + boost::lexical_cast<string>(i);
		attr = skill_element->get_attribute(fun_name);
		if (NULL == attr)
		{
			continue;
		}

		if (strlen(attr->as_cstr()) == 0)
		{
			continue;
		}

		if (string("0") == attr->as_cstr() )
		{
			continue;
		}

		rate_name = "rate" + boost::lexical_cast<string>(i);
		fa_name = "fa" + boost::lexical_cast<string>(i);
		fb_name = "fb" + boost::lexical_cast<string>(i);
		fc_name = "fc" + boost::lexical_cast<string>(i);
		fd_name = "fd" + boost::lexical_cast<string>(i);
		fe_name = "fe" + boost::lexical_cast<string>(i);

		SkillFunResInfo skill_fun_info;

		skill_fun_info.fun_id = attr->as_int();
		result = getAttrValue(skill_element, rate_name, skill_fun_info.rate) && result;
		result = getAttrValue(skill_element, fa_name, skill_fun_info.a) && result;
		result = getAttrValue(skill_element, fb_name, skill_fun_info.b) && result;
		result = getAttrValue(skill_element, fc_name, skill_fun_info.c) && result;
		result = getAttrValue(skill_element, fd_name, skill_fun_info.d) && result;
		result = getAttrValue(skill_element, fe_name, skill_fun_info.e) && result;

		skill_info->skill_fun_vec.push_back(skill_fun_info);
	}

	bool is_finish = false;
	for (int i = 0; i < skill_info->skill_fun_vec.size(); ++i)
	{
		SkillFunResInfo & skill_fun_info = skill_info->skill_fun_vec[i];
		if (3 == skill_fun_info.fun_id)
		{
			// move to the last position
			for (int j = skill_info->skill_fun_vec.size() - 1; j >= 0; --j)
			{
				if (i == j)
				{
					// finish
					is_finish = true;
					break;
				}
				
				SkillFunResInfo & last_info = skill_info->skill_fun_vec[j];
				if (last_info.fun_id != 3)
				{
					// exchange
					SkillFunResInfo temp = skill_info->skill_fun_vec[i];
					skill_info->skill_fun_vec[i] = skill_info->skill_fun_vec[j];
					skill_info->skill_fun_vec[j] = temp;
				}

				break;
			}

			if (is_finish)
			{
				break;
			}
		}
	}

	SkillResInfoMap_t::iterator it = m_skill_info_map.find(skill_info->skill_id);
	if (it != m_skill_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate skill id : <%d>\n", skill_info->skill_id);
		delete skill_info;
		return false;
	}

	m_skill_info_map.insert(std::make_pair(skill_info->skill_id, skill_info));

	return result;
}