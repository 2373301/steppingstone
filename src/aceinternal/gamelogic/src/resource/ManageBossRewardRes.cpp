
#include "ManageBossRewardRes.h"
#include "Logger.h"

bool ManageBossRewardRes::loadContent(Document * xml_doc)
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

const BossRewardResInfoMap_t & ManageBossRewardRes::getBossRewardInfo()
{
	return m_boss_reward_info_map;
}

bool ManageBossRewardRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	BossRewardResInfo * info = new BossRewardResInfo();

	result = getAttrValue(element, "enemy", info->enemy) && result;
	result = getAttrValue(element, "money_x1", info->money_x1) && result;
	result = getAttrValue(element, "money_a1", info->money_a1) && result;
	result = getAttrValue(element, "money_b1", info->money_b1) && result;
	result = getAttrValue(element, "frame_x2", info->frame_x2) && result;
	result = getAttrValue(element, "frame_a2", info->frame_a2) && result;
	result = getAttrValue(element, "frame_b2", info->frame_b2) && result;
	result = getAttrValue(element, "prestige_x3", info->prestige_x3) && result;
	result = getAttrValue(element, "prestige_a3", info->prestige_a3) && result;
	result = getAttrValue(element, "prestige_b3", info->prestige_b3) && result;
	result = getAttrValue(element, "substitute_money", info->substitute_money) && result;
	result = getAttrValue(element, "substitute_frame", info->substitute_frame) && result;
	result = getAttrValue(element, "substitute_prestige", info->substitute_prestige) && result;
	result = getAttrValue(element, "substitute_value", info->substitute_value) && result;
	result = getAttrValue(element, "money", info->money) && result;
	result = getAttrValue(element, "frame", info->frame) && result;
	result = getAttrValue(element, "prestige", info->prestige) && result;

	if (info->money.size() != 10)
	{
		return false;
	}
	if (info->frame.size() != 10)
	{
		return false;
	}
	if (info->prestige.size() != 10)
	{
		return false;
	}

	BossRewardResInfoMap_t::iterator it = m_boss_reward_info_map.find(info->enemy);
	if (it != m_boss_reward_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate boss enemy : <%d>\n", info->enemy);
		return false;
	}
	m_boss_reward_info_map.insert(std::make_pair(info->enemy, info));


	return result;
}