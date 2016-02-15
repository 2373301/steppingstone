

#include "ManageSpecialShopRes.h"
#include "Logger.h"

bool ManageSpecialShopRes::loadContent(Document * xml_doc)
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

const SpecialShopResInfoMap_t & ManageSpecialShopRes::getSpecialShopInfoMap()
{
	return m_special_shop_info_map;
}

bool ManageSpecialShopRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	SpecialShopResInfo * info = new SpecialShopResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "item_id", info->item_id) && result;
	result = getAttrValue(element, "num", info->item_num) && result;
	result = getAttrValue(element, "price", info->pricenum) && result;
	result = getAttrValue(element, "currency", info->currency) && result;
	result = getAttrValue(element, "prompt", info->prompt) && result;
	result = getAttrValue(element, "weight", info->_weight) && result;
	result = getAttrValue(element, "playerlv", info->_player_level) && result;

	SpecialShopResInfoMap_t::iterator it = m_special_shop_info_map.find(info->id);
	if (it != m_special_shop_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate special shop id <%d>", info->id);
		result = false;
	}
	else
	{
		m_special_shop_info_map.insert(std::make_pair(info->id, info));
	}

	return result;
}