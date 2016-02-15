#include "Logger.h"
#include "ManageGvgrewardsRes.h"
bool ManageGvgrewardsRes::loadContent(Document * xml_doc)
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
const GvgrewardsInfoMap_t & ManageGvgrewardsRes::getGvgrewardsInfo()
{
	return m_gvgrewards_res_map;
}
bool ManageGvgrewardsRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	GvgrewardsInfo * info = new GvgrewardsInfo();
	result = getAttrValue(element,"bonusid", info->bonusid) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"reward", info->reward) && result;
	GvgrewardsInfoMap_t::iterator it = m_gvgrewards_res_map.find(info->reward);
	if (it != m_gvgrewards_res_map.end())
	{
		DEF_LOG_ERROR("failed to load GVGREWARDS, get reduplicate daily quest function id <%d>\n",info->reward);
		return false;
	}
	m_gvgrewards_res_map.insert(std::make_pair(info->reward, info));
	return result;
}
