

#include "ManageRandomSkillRes.h"
#include "Logger.h"

bool ManageRandomSkillRes::loadContent(Document * xml_doc)
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
	Elements ele_list = root_ele->get_elements();
	for (Elements::iterator it = ele_list.begin(); it != ele_list.end(); ++it)
	{
		result = loadInfo(*it) && result;
	}

	return result;
}

const RandomSkillResInfoMap_t & ManageRandomSkillRes::getRandomSkillInfoMap()
{
	return m_randomskill_info_map;
}

bool ManageRandomSkillRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	RandomSkillResInfo * info = new RandomSkillResInfo();

	result = getAttrValue(element, "skillid", info->skill_id) && result;
	result = getAttrValue(element, "unlock_lv", info->unlock_lv) && result;
	result = getAttrValue(element, "weight", info->weight) && result;

	RandomSkillResInfoMap_t::iterator it = m_randomskill_info_map.find(info->skill_id);
	if (it != m_randomskill_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate RandomSkill id : <%d>\n", info->skill_id);
		return false;
	}

	m_randomskill_info_map.insert(std::make_pair(info->skill_id, info));

	return result;
}