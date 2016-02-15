
#include "ManageHammerRes.h"
#include "Logger.h"

bool ManageHammerRes::loadContent(Document * xml_doc)
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

const HammerResInfoMap_t & ManageHammerRes::getHammerInfoMap()
{
	return m_hammer_info_map;
}

bool ManageHammerRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	HammerResInfo * info = new HammerResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "probability", info->probability) && result;
	result = getAttrValue(element, "critical", info->critical) && result;
	result = getAttrValue(element, "pay", info->pay) && result;
	result = getAttrValue(element, "num", info->num) && result;

	HammerResInfoMap_t::iterator it = m_hammer_info_map.find(info->id);
	if (it != m_hammer_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate Hammer id : <%d>\n", info->id);
		return false;
	}

	m_hammer_info_map.insert(std::make_pair(info->id, info));

	return result;
}