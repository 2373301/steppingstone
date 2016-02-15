#include "Logger.h"
#include "ManageTavernspebilityRes.h"
bool ManageTavernspebilityRes::loadContent(Document * xml_doc)
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
const TavernspebilityInfoMap_t & ManageTavernspebilityRes::getTavernspebilityInfo()
{
	return m_tavernspebility__res_map;
}
bool ManageTavernspebilityRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TavernspebilityInfo*  info  = new TavernspebilityInfo();
	result = getAttrValue(element,"hire_id", info->hire_id) && result;
	result = getAttrValue(element,"times1", info->times1) && result;
	result = getAttrValue(element,"times2", info->times2) && result;
	result = getAttrValue(element,"times3", info->times3) && result;
	TavernspebilityInfoMap_t::iterator it = m_tavernspebility__res_map.find(info->hire_id);
	if (it != m_tavernspebility__res_map.end())
	{
		DEF_LOG_ERROR("failed to load TAVERNSPEBILITY, get reduplicate id <%d>\n",info->hire_id);
		return false;
	}
	m_tavernspebility__res_map.insert(std::make_pair(info->hire_id, info));
	return result;
}
