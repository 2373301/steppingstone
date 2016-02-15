#include "Logger.h"
#include "ManageDodgeRes.h"
bool ManageDodgeRes::loadContent(Document * xml_doc)
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
const DodgeInfoMap_t & ManageDodgeRes::getDodgeInfo()
{
	return m_dodge__res_map;
}
bool ManageDodgeRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	DodgeInfo * info = new DodgeInfo();
	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"item_unlock", info->item_unlock) && result;
	result = getAttrValue(element,"level_unlock", info->level_unlock) && result;
	result = getAttrValue(element,"need_dodge_num", info->need_dodge_num) && result;
	result = getAttrValue(element,"needlevel", info->need_level) && result;
	uint32 part_group = 0;
	uint32 part_level = 0;

	result = getAttrValue(element,"need_dodgemini_group1", part_group) && result;
	result = getAttrValue(element,"need_level1", part_level) && result;
	if (part_group != 0)
	{
		info->need_part_group.push_back(part_group);
		info->need_part_level.push_back(part_level);
	}
	result = getAttrValue(element,"need_dodgemini_group2", part_group) && result;
	result = getAttrValue(element,"need_level2", part_level) && result;
	if (part_group != 0)
	{
		info->need_part_group.push_back(part_group);
		info->need_part_level.push_back(part_level);
	}
	result = getAttrValue(element,"need_dodgemini_group3", part_group) && result;
	result = getAttrValue(element,"need_level3", part_level) && result;
	if (part_group != 0)
	{
		info->need_part_group.push_back(part_group);
		info->need_part_level.push_back(part_level);
	}
	result = getAttrValue(element,"need_dodgemini_group4", part_group) && result;
	result = getAttrValue(element,"need_level4", part_level) && result;
	if (part_group != 0)
	{
		info->need_part_group.push_back(part_group);
		info->need_part_level.push_back(part_level);
	}
	result = getAttrValue(element,"need_dodgemini_group5", part_group) && result;
	result = getAttrValue(element,"need_level5", part_level) && result;
	if (part_group != 0)
	{
		info->need_part_group.push_back(part_group);
		info->need_part_level.push_back(part_level);
	}
	result = getAttrValue(element,"need_dodgemini_group6", part_group) && result;
	result = getAttrValue(element,"need_level6", part_level) && result;
	if (part_group != 0)
	{
		info->need_part_group.push_back(part_group);
		info->need_part_level.push_back(part_level);
	}
	result = getAttrValue(element,"skill_id1", info->skill_id1) && result;
	result = getAttrValue(element,"skill_id2", info->skill_id2) && result;
	result = getAttrValue(element,"skill_level", info->skill_level) && result;
	result = getAttrValue(element,"talk", info->talk) && result;
	result = getAttrValue(element, "max", info->max) && result;
	result = getAttrValue(element, "need_id", info->need_id) && result;
	result = getAttrValue(element, "first_strike", info->first_strike) && result;
	result = getAttrValue(element,"type", info->_type) && result;
	result = getAttrValue(element, "level", info->level) && result;
//	result = getAttrValue(element, "isjar", info->isjar) && result;
	DodgeInfoMap_t::iterator it = m_dodge__res_map.find(info->id);
	if (it != m_dodge__res_map.end())
	{
		DEF_LOG_ERROR("failed to load DODGE, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_dodge__res_map.insert(std::make_pair(info->id, info));
	return result;
}
