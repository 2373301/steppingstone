

#include "ManageSectRes.h"
#include "Logger.h"

bool ManageSectRes::loadContent(Document * xml_doc)
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

const SectResInfoMap_t & ManageSectRes::getSectInfoMap()
{
	return m_sect_info_map;
}

bool ManageSectRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	SectResInfo * info = new SectResInfo();

	result = getAttrValue(element, "id", info->id) && result;
	result = getAttrValue(element, "limit", info->limit) && result;

	SectResInfoMap_t::iterator it = m_sect_info_map.find(info->id);
	if (it != m_sect_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate sect id : <%d>\n", info->id);
		return false;
	}

	m_sect_info_map.insert(std::make_pair(info->id, info));

	return result;
}