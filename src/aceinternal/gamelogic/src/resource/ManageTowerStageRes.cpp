#include "Logger.h"
#include "ManageTowerStageRes.h"

bool ManageTowerStageRes::loadContent(Document * xml_doc)
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
const TowerStageInfoMap_t & ManageTowerStageRes::getTowerStageInfo()
{
	return m_tower_stage_res_map;
}
bool ManageTowerStageRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TowerStageInfo * info = new TowerStageInfo();
	result = getAttrValue(element,"bonus", info->bonus) && result;
	result = getAttrValue(element,"enemy", info->enemy) && result;
	result = getAttrValue(element,"exp", info->exp) && result;
	result = getAttrValue(element,"level_group", info->level_group) && result;
	result = getAttrValue(element,"move_group", info->move_group) && result;
	result = getAttrValue(element, "floor", info->floor) && result;
	TowerStageInfoMap_t::iterator it = m_tower_stage_res_map.find(info->enemy);
	if (it != m_tower_stage_res_map.end())
	{
		DEF_LOG_ERROR("failed to load TOWERSTAGE, get reduplicate daily quest function id <%d>\n",info->enemy);
		return false;
	}
	m_tower_stage_res_map.insert(std::make_pair(info->floor, info));
	return result;
}
