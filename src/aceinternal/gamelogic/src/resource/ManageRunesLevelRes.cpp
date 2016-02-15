#include "Logger.h"
#include "ManageRunesLevelRes.h"
bool ManageRunesLevelRes::loadContent(Document * xml_doc)
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
const RunesLevelInfoMap_t & ManageRunesLevelRes::getRunesLevelInfo()
{
	return m_runes_level_res_map;
}
bool ManageRunesLevelRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	RunesLevelInfo * info = new RunesLevelInfo();
	result = getAttrValue(element,"class", info->classs) && result;
	result = getAttrValue(element,"exp", info->exp) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"money", info->money) && result;
	result = getAttrValue(element,"normal_value", info->normal_value) && result;
	result = getAttrValue(element,"p1", info->p1) && result;
	result = getAttrValue(element,"p2", info->p2) && result;
	result = getAttrValue(element,"p3", info->p3) && result;
	result = getAttrValue(element,"p4", info->p4) && result;
	result = getAttrValue(element,"p5", info->p5) && result;
	result = getAttrValue(element,"part", info->part) && result;
	result = getAttrValue(element,"runes_stone_num", info->runes_stone_num) && result;
	result = getAttrValue(element,"talk", info->talk) && result;
	result = getAttrValue(element,"value1", info->value1) && result;
	result = getAttrValue(element,"value2", info->value2) && result;
	result = getAttrValue(element,"value3", info->value3) && result;
	result = getAttrValue(element,"value4", info->value4) && result;
	result = getAttrValue(element,"value5", info->value5) && result;
	
	info->keys = info->id * 100000 + info->classs * 1000 + info->level;
	RunesLevelInfoMap_t::iterator it = m_runes_level_res_map.find(info->keys);
	if (it != m_runes_level_res_map.end())
	{
		DEF_LOG_ERROR("failed to load RUNESLEVEL, get reduplicate keys <%d>\n",info->keys);
		return false;
	}
	m_runes_level_res_map.insert(std::make_pair(info->keys, info));
	return result;
}
