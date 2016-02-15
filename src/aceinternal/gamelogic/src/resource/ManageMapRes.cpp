
#include "ManageMapRes.h"
#include "Logger.h"

ManageMapRes::~ManageMapRes()
{
	cleanMapSecondContent(m_map_info_map);
}

bool ManageMapRes::loadMapContent(Document * xml_doc)
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
	Elements map_ele_list = root_ele->get_elements();
	for (Elements::iterator it = map_ele_list.begin(); it != map_ele_list.end(); ++it)
	{
		result = loadMapInfo(*it) && result;
	}
	
	MapResInfo * map_info = new MapResInfo();
	map_info->map_id = 0;
	m_map_info_map.insert(std::make_pair(map_info->map_id, map_info));

	map_info = new MapResInfo();
	map_info->map_id = 99900;
	m_map_info_map.insert(std::make_pair(map_info->map_id, map_info));

	map_info = new MapResInfo();
	map_info->map_id = 99901;
	map_info->type = 100;	// do not need to unlock
	m_map_info_map.insert(std::make_pair(map_info->map_id, map_info));


	return result;
}

const MapResInfoMap_t & ManageMapRes::getMapInfoMap()
{
	return m_map_info_map;
}

bool ManageMapRes::loadMapInfo(Element * map_element)
{
	if (NULL == map_element)
	{
		return false;
	}

	bool result = true;

	MapResInfo * map_info = new MapResInfo();

	result = getAttrValue(map_element, "map_id", map_info->map_id) && result;
	result = getAttrValue(map_element, "type", map_info->type) && result;
	result = getAttrValue(map_element, "subtype", map_info->sub_type) && result;
	result = getAttrValue(map_element, "born_point_x", map_info->born_x) && result;
	result = getAttrValue(map_element, "born_point_y", map_info->born_y) && result;
	result = getAttrValue(map_element, "enter_level", map_info->enter_level) && result;
	result = getAttrValue(map_element, "unlock_level", map_info->unlock_level) && result;
	result = getAttrValue(map_element, "enter_quest", map_info->enter_quest) && result;
	result = getAttrValue(map_element, "npc", map_info->npc_set) && result;
	//result = getAttrValue(map_element, "enemy", map_info->enemy_set) && result;
	//result = getAttrValue(map_element, "bonus", map_info->bonus_vec) && result;
	//result = getAttrValue(map_element, "exp", map_info->exp) && result;
	//result = getAttrValue(map_element, "yueli", map_info->yueli) && result;
	result = getAttrValue(map_element, "talk", map_info->talk) && result;

	MapResInfoMap_t::iterator it = m_map_info_map.find(map_info->map_id);
	if (it != m_map_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate map id : <%d>\n", map_info->map_id);
		delete map_info;
		return false;
	}

	m_map_info_map.insert(std::make_pair(map_info->map_id, map_info));

	return result;
}