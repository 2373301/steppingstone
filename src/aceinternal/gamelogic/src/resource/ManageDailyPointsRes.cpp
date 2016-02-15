#include "Logger.h"
#include "ManageDailyPointsRes.h"

bool ManageDailyPointsRes::loadDailyPointsEevent(Document * xml_doc)
{
	if (NULL == xml_doc) return false;

	Element * root_ele = xml_doc->get_root();
	if (NULL == root_ele) return false;

	bool result = true;
	Elements ele_list = root_ele->get_elements();
	for (Elements::iterator it = ele_list.begin(); it != ele_list.end(); ++it)
	{
		result = loadDailyPointsEeventInfo(*it) && result;
	}

	return result;
}

bool ManageDailyPointsRes::loadDailyPointsReward(Document * xml_doc)
{
	if (NULL == xml_doc) return false;

	Element * root_ele = xml_doc->get_root();
	if (NULL == root_ele) return false;

	bool result = true;
	Elements ele_list = root_ele->get_elements();
	for (Elements::iterator it = ele_list.begin(); it != ele_list.end(); ++it)
	{
		result = loadDailyPointsRewardInfo(*it) && result;
	}

	return result;
}

const DailyPointsEventsMap_t & ManageDailyPointsRes::getDailyPointsEeventMap()
{
	return m_dailypoints_events_map;
}

const DailyPointsRewardMap_t & ManageDailyPointsRes::getDailyPointsRewardMap()
{
	return m_dailypoints_reward_map;
}


bool ManageDailyPointsRes::loadDailyPointsEeventInfo(Element * element)
{
	if (NULL == element) return false;

	bool result = true;
	DailyPointsEvents * info = new DailyPointsEvents();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "eventid", info->event_id) && result;
	result = getAttrValue(element, "num", info->num) && result;
	result = getAttrValue(element, "unlock_min", info->min_lev) && result;
	result = getAttrValue(element, "unlock_max", info->max_lev) && result;
	result = getAttrValue(element, "point", info->point) && result;


	DailyPointsEventsMap_t::iterator it = m_dailypoints_events_map.find(info->id);
	if (it != m_dailypoints_events_map.end())
	{
		DEF_LOG_ERROR("failed to load dailypoints_event.xml, get reduplicate dailypoints_event.xml id <%d>\n", info->id);
		return false;
	}

	m_dailypoints_events_map.insert(std::make_pair(info->id, info));
	return result;
}

bool ManageDailyPointsRes::loadDailyPointsRewardInfo(Element * element)
{
	if (NULL == element) return false;

	bool result = true;
	DailyPointsReward * info = new DailyPointsReward();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "lv", info->lev) && result;
	result = getAttrValue(element, "point", info->vec_point) && result;
	result = getAttrValue(element, "bonus_id", info->vec_bonus_id) && result;
	result = getAttrValue(element, "confrater_exp", info->vec_guild_exp) && result;

	DailyPointsRewardMap_t::iterator it = m_dailypoints_reward_map.find(info->id);
	if (it != m_dailypoints_reward_map.end())
	{
		DEF_LOG_ERROR("failed to load dailypoints_reward.xml, get reduplicate dailypoints_reward.xml id <%d>\n", info->id);
		return false;
	}

	m_dailypoints_reward_map.insert(std::make_pair(info->id, info));
	return result;
}