#include "Logger.h"
#include "ManageTowerRewardsRes.h"

bool ManageTowerRewardsRes::loadContent(Document * xml_doc)
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
const TowerRewardsInfoMap_t & ManageTowerRewardsRes::getTowerRewardsInfo()
{
	return m_tower_rewards_res_map;
}
bool ManageTowerRewardsRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TowerRewardsInfo * info = new TowerRewardsInfo();
	result = getAttrValue(element,"bonus1", info->bonus1) && result;
	result = getAttrValue(element,"bonus2", info->bonus2) && result;
	result = getAttrValue(element,"bonus3", info->bonus3) && result;
	result = getAttrValue(element,"bonus4", info->bonus4) && result;
	result = getAttrValue(element,"bonus5", info->bonus5) && result;
	result = getAttrValue(element,"exp1", info->exp1) && result;
	result = getAttrValue(element,"exp2", info->exp2) && result;
	result = getAttrValue(element,"level_group", info->level_group) && result;
	TowerRewardsInfoMap_t::iterator it = m_tower_rewards_res_map.find(info->level_group);
	if (it != m_tower_rewards_res_map.end())
	{
		DEF_LOG_ERROR("failed to load TOWERREWARDS, get reduplicate daily quest function id <%d>\n",info->level_group);
		return false;
	}
	m_tower_rewards_res_map.insert(std::make_pair(info->level_group, info));
	return result;
}
