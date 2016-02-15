#include "Logger.h"
#include "ManageSuitRes.h"
bool ManageSuitRes::loadContent(Document * xml_doc)
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
const SuitInfoMap_t & ManageSuitRes::getSuitInfo()
{
	return m_suit__res_map;
}
bool ManageSuitRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	SuitInfo * info = new SuitInfo();
	result = getAttrValue(element,"group", info->group) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"num", info->num) && result;
	result = getAttrValue(element,"property_id", info->property_id) && result;
	result = getAttrValue(element,"value", info->value) && result;

	uint32 keys = SuitInfo::make_key(info->group, info->num);
	SuitInfoMap_t::iterator it = m_suit__res_map.find(keys);
	if (it != m_suit__res_map.end())
	{
		DEF_LOG_ERROR("failed to load SUIT, get reduplicate id <%d>\n", keys);
		return false;
	}
	m_suit__res_map.insert(std::make_pair(keys, info));
	return result;
}
