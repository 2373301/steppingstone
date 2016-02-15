#include "Logger.h"
#include "ManageTavern_groupRes.h"
bool ManageTavern_groupRes::loadContent(Document * xml_doc)
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
const Tavern_groupInfoMap_t & ManageTavern_groupRes::getTavern_groupInfo()
{
	return m_tavern_group__res_map;
}
bool ManageTavern_groupRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	Tavern_groupInfo * info = new Tavern_groupInfo();
	result = getAttrValue(element,"hire_id", info->hire_id) && result;
	result = getAttrValue(element,"produce", info->produce) && result;
	Tavern_groupInfoMap_t::iterator it = m_tavern_group__res_map.find(info->hire_id);
	if (it != m_tavern_group__res_map.end())
	{
		DEF_LOG_ERROR("failed to load TAVERN_GROUP, get reduplicate id <%d>\n",info->hire_id);
		return false;
	}
	m_tavern_group__res_map.insert(std::make_pair(info->hire_id, info));
	return result;
}
