#include "Logger.h"
#include "ManageRankingRuleRes.h"
bool ManageRankingRuleRes::loadContent(Document * xml_doc)
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
const RankingRuleInfoMap_t & ManageRankingRuleRes::getRankingRuleInfo()
{
	return m_Ranking_Rule_res_map;
}
bool ManageRankingRuleRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	RankingRuleInfo * info = new RankingRuleInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"list", info->list) && result;
	result = getAttrValue(element,"mail_id", info->mail_id) && result;
	result = getAttrValue(element,"num", info->num) && result;
	result = getAttrValue(element,"update_time", info->update_time) && result;
	RankingRuleInfoMap_t::iterator it = m_Ranking_Rule_res_map.find(info->id);
	if (it != m_Ranking_Rule_res_map.end())
	{
		DEF_LOG_ERROR("failed to load RANKINGRULE, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Ranking_Rule_res_map.insert(std::make_pair(info->id, info));
	return result;
}
