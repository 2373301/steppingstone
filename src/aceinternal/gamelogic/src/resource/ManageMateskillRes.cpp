#include "Logger.h"
#include "ManageMateskillRes.h"
bool ManageMateskillRes::loadContent(Document * xml_doc)
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
const MateskillInfoMap_t & ManageMateskillRes::getMateskillInfo()
{
	return m_Mateskill__res_map;
}
bool ManageMateskillRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	MateskillInfo * info = new MateskillInfo();
	result = getAttrValue(element,"next_id", info->next_id) && result;
	result = getAttrValue(element,"skill_id", info->skill_id) && result;
	MateskillInfoMap_t::iterator it = m_Mateskill__res_map.find(info->skill_id);
	if (it != m_Mateskill__res_map.end())
	{
		DEF_LOG_ERROR("failed to load MATESKILL, get reduplicate id <%d>\n",info->skill_id);
		return false;
	}
	m_Mateskill__res_map.insert(std::make_pair(info->skill_id, info));
	return result;
}
