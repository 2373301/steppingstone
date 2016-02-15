
#include "ManageDailyBonusRes.h"
#include "Logger.h"

bool ManageDailyBonusRes::loadContent(Document * xml_doc)
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

const DailyBonusResInfoMap_t & ManageDailyBonusRes::getDailyBonusInfoMap()
{
	return m_daily_bonus_info_map;
}

bool ManageDailyBonusRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	DailyBonusResInfo * info = new DailyBonusResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "bonustype", info->bonustype) && result;
	result = getAttrValue(element, "a", info->a) && result;
	result = getAttrValue(element, "b", info->b) && result;
	result = getAttrValue(element, "lv", info->lv) && result;
	result = getAttrValue(element, "limit", info->limit_time) && result;
	result = getAttrValue(element, "opentime", info->enable_level) && result;

    result = getAttrValue(element, "bonus_id", info->bonus_id) && result;
    result = getAttrValue(element, "boom", info->boom) && result;

    result = getAttrValue(element, "free_a", info->free_a) && result;
    result = getAttrValue(element, "free_b", info->free_b) && result;
    result = getAttrValue(element, "critical_num", info->critial_num_vec) && result;
    result = getAttrValue(element, "critical_value", info->critial_value_vec) && result;
    result = getAttrValue(element, "boom_num", info->boom_num) && result;
    result = getAttrValue(element, "talk", info->talk) && result;


	DailyBonusResInfoMap_t::iterator it = m_daily_bonus_info_map.find(info->id);
	if (it != m_daily_bonus_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate DailyBonus id : <%d>\n", info->id);
		return false;
	}

	m_daily_bonus_info_map.insert(std::make_pair(info->id, info));

	return result;
}