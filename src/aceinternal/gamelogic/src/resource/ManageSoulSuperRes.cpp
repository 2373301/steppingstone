#include "Logger.h"
#include "ManageSoulSuperRes.h"
bool ManageSoulSuperRes::loadContent(Document * xml_doc)
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
const SoulSuperInfoMap_t & ManageSoulSuperRes::getSoulSuperInfo()
{
	return m_soul_super_res_map;
}
bool ManageSoulSuperRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	SoulSuperInfo * info = new SoulSuperInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"range", info->range) && result;
	result = getAttrValue(element,"property_id", info->property_id) && result;
	result = getAttrValue(element,"type", info->type) && result;
	SoulSuperInfoMap_t::iterator it = m_soul_super_res_map.find(info->id);
	if (it != m_soul_super_res_map.end())
	{
		DEF_LOG_ERROR("failed to load SOULSUPER, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_soul_super_res_map.insert(std::make_pair(info->id, info));	
	return result;
}
