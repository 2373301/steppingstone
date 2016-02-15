
#include "ManageCardAndJokerRes.h"
#include "Logger.h"

bool ManageCardAndJokerRes::loadContent(Document * xml_doc)
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

const CardAndJokerResInfoMap_t & ManageCardAndJokerRes::getCardAndJokerInfoMap()
{
	return m_card_and_joker_info_map;
}

bool ManageCardAndJokerRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	CardAndJokerResInfo * info = new CardAndJokerResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "type", info->type) && result;
	result = getAttrValue(element, "subtype", info->subtype) && result;
	result = getAttrValue(element, "a", info->a) && result;
	result = getAttrValue(element, "b", info->b) && result;
	result = getAttrValue(element, "probability", info->probability) && result;
	result = getAttrValue(element, "critical", info->critical) && result;
	result = getAttrValue(element, "weight", info->weight) && result;


	CardAndJokerResInfoMap_t::iterator it = m_card_and_joker_info_map.find(info->id);
	if (it != m_card_and_joker_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate CardAndJoker id : <%d>\n", info->id);
		return false;
	}

	m_card_and_joker_info_map.insert(std::make_pair(info->id, info));

	return result;
}