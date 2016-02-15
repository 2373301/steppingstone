
#include "ManageItemStrengRes.h"
#include "Logger.h"

bool ManageItemStrengRes::loadContent(Document * xml_doc)
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

const ItemStrengResInfoMap_t & ManageItemStrengRes::getItemStrengInfoMap()
{
	return m_item_streng_info_map;
}

bool ManageItemStrengRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	ItemStrengResInfo * info = new ItemStrengResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	//result = getAttrValue(element, "name", info->name) && result;
	result = getAttrValue(element, "strenglv", info->strenglv) && result;
	result = getAttrValue(element, "part", info->part) && result;
	result = getAttrValue(element, "color", info->color) && result;
	result = getAttrValue(element, "consumption", info->consumption) && result;
	result = getAttrValue(element, "profit", info->profit) && result;
	result = getAttrValue(element, "num", info->num) && result;
	result = getAttrValue(element, "money", info->money) && result;
	result = getAttrValue(element, "law", info->law) && result;
	result = getAttrValue(element, "profit1", info->profit1) && result;
	result = getAttrValue(element, "num1", info->num1) && result;
	result = getAttrValue(element, "first_strike", info->first_strike) && result;
	result = getAttrValue(element, "time", info->time) && result;

	ItemStrengResInfoMap_t::iterator it = m_item_streng_info_map.find(info->id);
	if (it != m_item_streng_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate ItemStreng id : <%d>\n", info->id);
		return false;
	}

	m_item_streng_info_map.insert(std::make_pair(info->id, info));

	return result;
}
