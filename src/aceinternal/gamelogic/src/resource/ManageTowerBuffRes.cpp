#include "Logger.h"
#include "ManageTowerBuffRes.h"

bool ManageTowerBuffRes::loadContent(Document * xml_doc)
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
const TowerBuffInfoMap_t & ManageTowerBuffRes::getTowerBuffInfo()
{
	return m_tower_Buff_res_map;
}
bool ManageTowerBuffRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TowerBuffInfo * info = new TowerBuffInfo();
	result = getAttrValue(element,"goleblessme_id", info->goleblessme_id) && result;
	result = getAttrValue(element,"probability", info->probability) && result;
	result = getAttrValue(element,"type", info->type) && result;
	TowerBuffInfoMap_t::iterator it = m_tower_Buff_res_map.find(info->type);
	if (it != m_tower_Buff_res_map.end())
	{
		DEF_LOG_ERROR("failed to load TOWERBUFF, get reduplicate daily quest function id <%d>\n",info->type);
		return false;
	}
	m_tower_Buff_res_map.insert(std::make_pair(info->type, info));
	return result;
}
