
#include "Logger.h"
#include "ManageDailyQuestRes.h"

bool ManageDailyQuestRes::loadContent(Document * xml_doc)
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

const DailyQuestInfoMap_t & ManageDailyQuestRes::getDailyQuestInfo()
{
	return m_daily_quest_res_map;
}

bool ManageDailyQuestRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	DailyQuestInfo * info = new DailyQuestInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "correct_money", info->correct_money) && result;
	result = getAttrValue(element, "correct_seni", info->correct_seni) && result;
	result = getAttrValue(element, "correct_exp", info->correct_exp) && result;
	result = getAttrValue(element, "correct_coin", info->correct_coin) && result;
	result = getAttrValue(element, "correct_frame", info->correct_frame) && result;
	result = getAttrValue(element, "pro", info->pro) && result;
	result = getAttrValue(element, "senicost", info->senicost) && result;
	result = getAttrValue(element, "rate", info->rate) && result;

	if (info->pro < 0 || info->pro > 10000)
	{
		DEF_LOG_ERROR("daily quest pro error!");
		return -1;
	}

	DailyQuestInfoMap_t::iterator it = m_daily_quest_res_map.find(info->id);
	if (it != m_daily_quest_res_map.end())
	{
		DEF_LOG_ERROR("failed to load dailyquest, get reduplicate daily quest function id <%d>\n", info->id);
		return false;
	}

	m_daily_quest_res_map.insert(std::make_pair(info->id, info));

	return result;
}
