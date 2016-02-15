
#include "ManageItemRes.h"
#include "Logger.h"

bool ManageItemRes::loadContent(Document * xml_doc)
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

const ItemResInfoMap_t & ManageItemRes::getItemInfoMap()
{
	return m_item_info_map;
}

bool ManageItemRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	ItemResInfo * info = new ItemResInfo();

	result = getAttrValue(element, "id", info->item_id) && result;
	result = getAttrValue(element, "level", info->level) && result;
	result = getAttrValue(element, "job", info->job) && result;
	result = getAttrValue(element, "sex", info->sex) && result;
	result = getAttrValue(element, "martial", info->martial) && result;
	result = getAttrValue(element, "part", info->part) && result;
	result = getAttrValue(element, "color", info->color) && result;
	result = getAttrValue(element, "equipment", info->equipment) && result;
	result = getAttrValue(element, "type", info->type) && result;
	result = getAttrValue(element, "subtype", info->subtype) && result;
	result = getAttrValue(element, "pub1", info->pub1) && result;
	result = getAttrValue(element, "pub2", info->pub2) && result;
	result = getAttrValue(element, "pub3", info->pub3) && result;
	result = getAttrValue(element, "pub4", info->pub4) && result;
	result = getAttrValue(element, "pub5", info->pub5) && result;
	result = getAttrValue(element, "link", info->link) && result;
	result = getAttrValue(element, "price", info->price) && result;
	result = getAttrValue(element, "pricenum", info->pricenum) && result;
	result = getAttrValue(element, "stack", info->stack) && result;
	result = getAttrValue(element, "talk", info->talk) && result;
	result = getAttrValue(element, "value", info->coin_value) && result;
	result = getAttrValue(element, "group", info->group) && result;
	result = getAttrValue(element, "holefree", info->holefree) && result;
	result = getAttrValue(element, "autosale", info->auto_sale) && result;
	result = getAttrValue(element, "holelimit", info->holelimit) && result;
	result = getAttrValue(element, "cost", info->cost) && result;
	result = getAttrValue(element, "options", info->options) && result;
	result = getAttrValue(element, "options_num", info->options_num) && result;
	result = getAttrValue(element, "result", info->result) && result;
	result = getAttrValue(element, "vip_limit", info->vip_limit) && result;
	result = getAttrValue(element, "resolve_bonus", info->resolve_bonus) && result;

	ItemResInfoMap_t::iterator it = m_item_info_map.find(info->item_id);
	if (it != m_item_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate item id : <%d>\n", info->item_id);
		return false;
	}

	m_item_info_map.insert(std::make_pair(info->item_id, info));

	return result;
}