#include "Logger.h"
#include "ManageAchievementRes.h"


bool ManageAchievementRes::loadContentAchieveFun(Document * xml_doc)
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
		result = loadAchieveFunInfo(*it) && result;
	}

	return result;
}

bool ManageAchievementRes::loadContentAchievement(Document * xml_doc)
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
		result = loadAchievementInfo(*it) && result;
	}

	return result;
}

bool ManageAchievementRes::loadContentAchieveScore(Document * xml_doc)
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
		result = loadAchieveScoreInfo(*it) && result;
	}

	return result;
}
const AchievementInfoMap_t & ManageAchievementRes::getAchievementInfo()
{
	return m_achievement_res_map;
}

const AchievementRewardMap_t & ManageAchievementRes::getAchievementReward()
{
	return m_achievement_reward_map;
}

const AchievementUnlockMap_t & ManageAchievementRes::getAchievementUnlock()
{
	return m_achievement_unlock_map;
}

bool ManageAchievementRes::loadAchieveFunInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	AchievementUnlock * info = new AchievementUnlock();

	result = getAttrValue(element, "functionid", info->functionid) && result;
	result = getAttrValue(element, "gold_need", info->gold_need) && result;
	result = getAttrValue(element, "options", info->options) && result;


	AchievementUnlockMap_t::iterator it = m_achievement_unlock_map.find(info->functionid);
	if (it != m_achievement_unlock_map.end())
	{
		DEF_LOG_ERROR("failed to load achieve_fun.xml, get reduplicate achieve_fun.xml functionid <%d>\n", info->functionid);
		return false;
	}

	m_achievement_unlock_map.insert(std::make_pair(info->functionid, info));

	return result;
}


bool ManageAchievementRes::loadAchievementInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	AchievementInfo * info = new AchievementInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "type", info->type) && result;
	result = getAttrValue(element, "fun1", info->fun1) && result;
	result = getAttrValue(element, "fun2", info->fun2) && result;
	result = getAttrValue(element, "bouns", info->bouns) && result;
	result = getAttrValue(element, "point", info->point) && result;
	result = getAttrValue(element, "unlock", info->unlock) && result;


	AchievementInfoMap_t::iterator it = m_achievement_res_map.find(info->id);
	if (it != m_achievement_res_map.end())
	{
		DEF_LOG_ERROR("failed to load achivevment.xml, get reduplicate achivevment.xml id <%d>\n", info->id);
		return false;
	}

	m_achievement_res_map.insert(std::make_pair(info->id, info));

	return result;
}

bool ManageAchievementRes::loadAchieveScoreInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	AchievementReward * info = new AchievementReward();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "point", info->point) && result;
	result = getAttrValue(element, "bonus", info->bonus) && result;
	result = getAttrValue(element, "block", info->block) && result;
	result = getAttrValue(element, "dailybonus", info->dailybonus) && result;



	AchievementRewardMap_t::iterator it = m_achievement_reward_map.find(info->id);
	if (it != m_achievement_reward_map.end())
	{
		DEF_LOG_ERROR("failed to load achieve_score.xml, get reduplicate achieve_score.xml id <%d>\n", info->id);
		return false;
	}

	m_achievement_reward_map.insert(std::make_pair(info->id, info));

	return result;
}