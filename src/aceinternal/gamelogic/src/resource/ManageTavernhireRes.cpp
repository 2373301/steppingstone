#include "Logger.h"
#include "ManageTavernhireRes.h"
bool ManageTavernhireRes::loadContent(Document * xml_doc)
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
const TavernhireInfoMap_t & ManageTavernhireRes::getTavernhireInfo()
{
	return m_tavernhire_res_map;
}

bool ManageTavernhireRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TavernhireInfo * info = new TavernhireInfo();
	result = getAttrValue(element,"level_group", info->level) && result;
	result = getAttrValue(element,"resource_num", info->resource_num) && result;
	result = getAttrValue(element,"spe_probability", info->spe_probability) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"spe_hire", info->spe_hire) && result;

	uint32 max_level_in_map = 0;
	uint32 map_key = TavernhireInfo::make_key(info->type, info->level);
	TavernhireInfoMap_t::reverse_iterator rbegins = m_tavernhire_res_map.rbegin();
	if (rbegins != m_tavernhire_res_map.rend() && map_key - info->level < rbegins->first)
		max_level_in_map = rbegins->second->level; 
	else
		max_level_in_map = info->level;

	
	TavernhireInfoMap_t::iterator it = m_tavernhire_res_map.find(map_key);
	if (it != m_tavernhire_res_map.end())
	{
		DEF_LOG_ERROR("Failed to load tavernhire.xml, card_level_group_key<%u,%u> is repeat.\n", info->type, info->level);
		return false;
	}

	if (max_level_in_map == info->level)
		m_tavernhire_res_map.insert(std::make_pair(map_key, info));
	else
	{
		for (uint32 i = max_level_in_map + 1; i < info->level; ++i)
		{
			uint32 keys  = TavernhireInfo::make_key(info->type, i);
			m_tavernhire_res_map.insert(std::make_pair(keys, rbegins->second));
		}
		m_tavernhire_res_map.insert(std::make_pair(map_key, info));
	}
	return result;
}