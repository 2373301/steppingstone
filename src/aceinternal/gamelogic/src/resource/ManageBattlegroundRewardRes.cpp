#include "Logger.h"
#include "ManageBattlegroundRewardRes.h"
bool ManageBattlegroundRewardRes::loadContent(Document * xml_doc)
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
const BattlegroundRewardInfoMap_t & ManageBattlegroundRewardRes::getBattlegroundRewardInfo()
{
	return m_Battleground_Reward_res_map;
}
bool ManageBattlegroundRewardRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	BattlegroundRewardInfo * info = new BattlegroundRewardInfo();
	result = getAttrValue(element,"bonus_id", info->bonus_id) && result;
	result = getAttrValue(element,"faction_id", info->faction_id) && result;
	result = getAttrValue(element,"frame_a1", info->frame_a1) && result;
	result = getAttrValue(element,"frame_a2", info->frame_a2) && result;
	result = getAttrValue(element,"frame_a3", info->frame_a3) && result;
	result = getAttrValue(element,"frame_a4", info->frame_a4) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"money_b1", info->money_b1) && result;
	result = getAttrValue(element,"money_b2", info->money_b2) && result;
	result = getAttrValue(element,"money_b3", info->money_b3) && result;
	result = getAttrValue(element,"money_b4", info->money_b4) && result;
	result = getAttrValue(element,"weiwang_c1", info->weiwang_c1) && result;
	result = getAttrValue(element,"weiwang_c2", info->weiwang_c2) && result;
	result = getAttrValue(element,"weiwang_c3", info->weiwang_c3) && result;
	result = getAttrValue(element,"weiwang_c4", info->weiwang_c4) && result;

    result = getAttrValue(element,"frame_limit", info->frame_limit) && result;
    result = getAttrValue(element,"money_limit", info->money_limit) && result;
    result = getAttrValue(element,"weiwang_limit", info->weiwang_limit) && result;
    
    if(info->frame_limit.size() != info->frame_a3.size())
    {
        DEF_LOG_ERROR("failed to load BATTLEGROUNDREWARD, <%d> frame not the same\n",info->id);
        return false;
    }

    if( info->money_b3.size() != info->money_limit.size())
    {
        DEF_LOG_ERROR("failed to load BATTLEGROUNDREWARD, <%d> money not the same\n",info->id);
        return false;
    }

    if( info->weiwang_c3.size() != info->weiwang_limit.size())
    {
        DEF_LOG_ERROR("failed to load BATTLEGROUNDREWARD, <%d> weiwang not the same\n",info->id);
        return false;
    }


	BattlegroundRewardInfoMap_t::iterator it = m_Battleground_Reward_res_map.find(info->id);
	if (it != m_Battleground_Reward_res_map.end())
	{
		DEF_LOG_ERROR("failed to load BATTLEGROUNDREWARD, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Battleground_Reward_res_map.insert(std::make_pair(info->id, info));
	return result;
}
