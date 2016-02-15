#include "Logger.h"
#include "ManageTalentRes.h"
bool ManageTalentRes::loadContent(Document * xml_doc)
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
const TalentInfoMap_t & ManageTalentRes::getTalentInfo()
{
	return m_Talent__res_map;
}
bool ManageTalentRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TalentInfo * info = new TalentInfo();
	result = getAttrValue(element,"gold_rate1", info->gold_rate1) && result;
	result = getAttrValue(element,"gold_rate2", info->gold_rate2) && result;
	result = getAttrValue(element,"group_id", info->group_id) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"is_show", info->is_show) && result;
	result = getAttrValue(element,"item_rate1", info->item_rate1) && result;
	result = getAttrValue(element,"item_rate2", info->item_rate2) && result;
	result = getAttrValue(element,"skill_id", info->skill_id) && result;
	TalentInfoMap_t::iterator it = m_Talent__res_map.find(info->id);
	if (it != m_Talent__res_map.end())
	{
		DEF_LOG_ERROR("failed to load TALENT, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Talent__res_map.insert(std::make_pair(info->id, info));
	return result;
}
