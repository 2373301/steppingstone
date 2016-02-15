#include "Logger.h"
#include "ManageGoldCardRes.h"
bool ManageGoldCardRes::loadContent(Document * xml_doc)
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
const GoldCardInfoMap_t & ManageGoldCardRes::getGoldCardInfo()
{
	return m_gold_card_res_map;
}
bool ManageGoldCardRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	GoldCardInfo * info = new GoldCardInfo();
	result = getAttrValue(element,"bonus_rewards", info->bonus_rewards) && result;
	result = getAttrValue(element,"day", info->day) && result;
	result = getAttrValue(element,"first_rewards", info->first_rewards) && result;
	result = getAttrValue(element,"gold_rewards", info->gold_rewards) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"need_gold", info->need_gold) && result;
	result = getAttrValue(element,"p", info->p) && result;
	result = getAttrValue(element,"talk", info->talk) && result;
	result = getAttrValue(element,"target", info->target) && result;
	result = getAttrValue(element,"unlock_gold", info->unlock_gold) && result;
	result = getAttrValue(element,"v", info->v) && result;
	result = getAttrValue(element,"name", info->card_name) && result;
	result = getAttrValue(element,"first_bonus", info->first_bonus) && result;
	GoldCardInfoMap_t::iterator it = m_gold_card_res_map.find(info->id);
	if (it != m_gold_card_res_map.end())
	{
		DEF_LOG_ERROR("failed to load GOLDCARD, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_gold_card_res_map.insert(std::make_pair(info->id, info));
	return result;
}
