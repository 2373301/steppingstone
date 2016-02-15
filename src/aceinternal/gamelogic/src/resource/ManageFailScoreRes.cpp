#include "Logger.h"
#include "ManageFailScoreRes.h"
bool ManageFailScoreRes::loadContent(Document * xml_doc)
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
const FailScoreInfoMap_t & ManageFailScoreRes::getFailScoreInfo()
{
	return m_fail_score_res_map;
}
bool ManageFailScoreRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	FailScoreInfo * info = new FailScoreInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"star1", info->star1) && result;
	result = getAttrValue(element,"star2", info->star2) && result;
	result = getAttrValue(element,"star3", info->star3) && result;
	result = getAttrValue(element,"star4", info->star4) && result;
	result = getAttrValue(element,"star5", info->star5) && result;
	FailScoreInfoMap_t::iterator it = m_fail_score_res_map.find(info->id);
	if (it != m_fail_score_res_map.end())
	{
		DEF_LOG_ERROR("failed to load FAILSCORE, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_fail_score_res_map.insert(std::make_pair(info->id, info));
	return result;
}
