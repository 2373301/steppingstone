#include "Logger.h"
#include "ManageTalentgroupRes.h"
bool ManageTalentgroupRes::loadContent(Document * xml_doc)
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
const TalentgroupInfoMap_t & ManageTalentgroupRes::getTalentgroupInfo()
{
	return m_TalentGroup__res_map;
}
bool ManageTalentgroupRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TalentgroupInfo * info = new TalentgroupInfo();
	result = getAttrValue(element,"gold_rate1", info->gold_rate1) && result;
	result = getAttrValue(element,"group_id", info->group_id) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"item_rate1", info->item_rate1) && result;
	TalentgroupInfoMap_t::iterator it = m_TalentGroup__res_map.find(info->group_id);
	if (it != m_TalentGroup__res_map.end())
	{
		DEF_LOG_ERROR("failed to load TALENTGROUP, get reduplicate id <%d>\n",info->group_id);
		return false;
	}
	m_TalentGroup__res_map.insert(std::make_pair(info->group_id, info));
	return result;
}
