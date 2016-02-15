#include "Logger.h"
#include "ManageSoulStageRes.h"
bool ManageSoulStageRes::loadContent(Document * xml_doc)
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
const SoulStageInfoMap_t & ManageSoulStageRes::getSoulStageInfo()
{
	return m_Soul_Stage_res_map;
}
bool ManageSoulStageRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	SoulStageInfo * info = new SoulStageInfo();
	result = getAttrValue(element,"bonus", info->bonus) && result;
	result = getAttrValue(element,"enemy_id", info->enemy_id) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	SoulStageInfoMap_t::iterator it = m_Soul_Stage_res_map.find(info->id);
	if (it != m_Soul_Stage_res_map.end())
	{
		DEF_LOG_ERROR("failed to load SOULSTAGE, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Soul_Stage_res_map.insert(std::make_pair(info->id, info));
	return result;
}
