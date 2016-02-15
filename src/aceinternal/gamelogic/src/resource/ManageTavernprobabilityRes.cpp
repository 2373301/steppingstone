#include "Logger.h"
#include "ManageTavernprobabilityRes.h"
#include <boost/foreach.hpp>

bool ManageTavernprobabilityRes::loadContent(Document * xml_doc)
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

const TavernprobabilityInfoMap_t & ManageTavernprobabilityRes::getTavernprobabilityInfo()
{
	return m_tavernprobability_res_map;
}

bool ManageTavernprobabilityRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	TavernprobabilityInfo * info = new TavernprobabilityInfo();
	result = getAttrValue(element,"hire_id", info->hire_id) && result;
	result = getAttrValue(element,"probability1", info->probability1) && result;
	result = getAttrValue(element,"probability2", info->probability2) && result;
	result = getAttrValue(element,"spe_probability1", info->spe_probability1) && result;
	result = getAttrValue(element,"spe_probability2", info->spe_probability2) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"level_group", info->level) && result;
	result = getAttrValue(element,"card_probability1", info->card_probability1) && result;
	result = getAttrValue(element,"card_probability2", info->card_probability2) && result;
	result = getAttrValue(element,"minimum", info->minimum) && result;
	result = getAttrValue(element,"judge", info->judge) && result;

	uint32 probability1_weight = 0;
	BOOST_FOREACH(uint32 weight, info->probability1)
	{
		probability1_weight += weight;
	}
	info->max_random_weight = probability1_weight + info->probability2;

	uint32 spe_probability1_weight = 0;
	BOOST_FOREACH(uint32 weight, info->spe_probability1)
	{
		spe_probability1_weight += weight;
	}
	info->max_change_weight = spe_probability1_weight + info->spe_probability2;

	uint32 max_level_in_map = 0;
	uint32 map_key = TavernprobabilityInfo::make_key(info->type, info->level);
	TavernprobabilityInfoMap_t::reverse_iterator rbegins = m_tavernprobability_res_map.rbegin();
	if (rbegins != m_tavernprobability_res_map.rend() && map_key - info->level < rbegins->first)
		max_level_in_map = rbegins->second->level; 
	else
		max_level_in_map = info->level;

	TavernprobabilityInfoMap_t::iterator it = m_tavernprobability_res_map.find(map_key);
	if (it != m_tavernprobability_res_map.end())
	{
		DEF_LOG_ERROR("Failed to load tavernprobability.xml, card_level_group_key<%u,%u> is repeat.\n", info->type, info->level);
		return false;
	}
	
	if (max_level_in_map == info->level)
		m_tavernprobability_res_map.insert(std::make_pair(map_key, info));
	else
	{
		for (uint32 i = max_level_in_map + 1; i < info->level; ++i)
		{
			uint32 keys = TavernprobabilityInfo::make_key(info->type, i);
			m_tavernprobability_res_map.insert(std::make_pair(keys, rbegins->second));
		}
		m_tavernprobability_res_map.insert(std::make_pair(map_key, info));
	}
	return result;
}