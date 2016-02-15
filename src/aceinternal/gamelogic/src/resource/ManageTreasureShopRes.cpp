#include "Logger.h"
#include "ManageTreasureShopRes.h"
bool ManageTreasureShopRes::loadContent(Document * xml_doc)
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
const TreasureShopInfoMap_t & ManageTreasureShopRes::getTreasureShopInfo()
{
	return m_treasure_shop_res_map;
}
bool ManageTreasureShopRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TreasureShopInfo * info = new TreasureShopInfo();
	result = getAttrValue(element,"exp_item", info->exp_item) && result;
	result = getAttrValue(element,"label", info->label) && result;
	result = getAttrValue(element,"level", info->level) && result;
	TreasureShopInfoMap_t::iterator it = m_treasure_shop_res_map.find(info->label);
	if (it != m_treasure_shop_res_map.end())
	{
		DEF_LOG_ERROR("failed to load TREASURESHOP, get reduplicate id <%d>\n",info->label);
		return false;
	}
	m_treasure_shop_res_map.insert(std::make_pair(info->label, info));
	return result;
}
