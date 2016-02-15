#include "Logger.h"
#include "ManageSoulSetRes.h"
bool ManageSoulSetRes::loadContent(Document * xml_doc)
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
const SoulSetInfoMap_t & ManageSoulSetRes::getSoulSetInfo()
{
	return m_soul_set_res_map;
}
bool ManageSoulSetRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	SoulSetInfo * info = new SoulSetInfo();
	result = getAttrValue(element,"default_open", info->default_open) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"unlock", info->unlock) && result;
	result = getAttrValue(element,"unlock_cost", info->unlock_cost) && result;
	SoulSetInfoMap_t::iterator it = m_soul_set_res_map.find(info->id);
	if (it != m_soul_set_res_map.end())
	{
		DEF_LOG_ERROR("failed to load SOULSET, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_soul_set_res_map.insert(std::make_pair(info->id, info));
	return result;
}
