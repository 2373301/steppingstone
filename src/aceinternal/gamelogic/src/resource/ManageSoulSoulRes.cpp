#include "Logger.h"
#include "ManageSoulSoulRes.h"
bool ManageSoulSoulRes::loadContent(Document * xml_doc)
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
const SoulSoulInfoMap_t & ManageSoulSoulRes::getSoulSoulInfo()
{
	return m_soul_soul_res_map;
}
bool ManageSoulSoulRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	SoulSoulInfo * info = new SoulSoulInfo();
	result = getAttrValue(element,"color", info->color) && result;
	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"group_lv", info->group_lv) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"property_id", info->property_id) && result;
	result = getAttrValue(element,"rate", info->rate) && result;
	result = getAttrValue(element,"talk_id", info->talk_id) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"value", info->value) && result;
	result = getAttrValue(element,"name", info->name) && result;
	SoulSoulInfoMap_t::iterator it = m_soul_soul_res_map.find(info->id);
	if (it != m_soul_soul_res_map.end())
	{
		DEF_LOG_ERROR("failed to load SOULSOUL, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_soul_soul_res_map.insert(std::make_pair(info->id, info));
	return result;
}
