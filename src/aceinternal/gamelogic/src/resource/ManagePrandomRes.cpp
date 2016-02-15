

#include "ManagePrandomRes.h"
#include "Logger.h"

bool ManagePrandomRes::loadContent(Document * xml_doc)
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

const PrandomResInfoMap_t & ManagePrandomRes::getPrandomInfoMap()
{
	return m_prandom_info_map;
}

bool ManagePrandomRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	PrandomResInfo * info = new PrandomResInfo();

	result = getAttrValue(element, "lv", info->level) && result;
	result = getAttrValue(element, "low", info->low) && result;
	result = getAttrValue(element, "high", info->high) && result;
	result = getAttrValue(element, "sum", info->sum) && result;

	PrandomResInfoMap_t::iterator it = m_prandom_info_map.find(info->level);
	if (it != m_prandom_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate Prandom id : <%d>\n", info->level);
		return false;
	}

	m_prandom_info_map.insert(std::make_pair(info->level, info));

	return result;
}