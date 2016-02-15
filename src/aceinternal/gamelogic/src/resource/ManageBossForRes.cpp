
#include "ManageBossForRes.h"
#include "Logger.h"

bool ManageBossForRes::loadContent(Document * xml_doc)
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

const BossForResInfoMap_t & ManageBossForRes::getBossForInfo()
{
	return m_boss_for_info_map;
}

bool ManageBossForRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;

	BossForResInfo * info = new BossForResInfo();

	result = getAttrValue(element, "map_id", info->map_id) && result;
	result = getAttrValue(element, "enemy_id", info->enemy_id) && result;
	result = getAttrValue(element, "role_id", info->role_id) && result;
	result = getAttrValue(element, "boss_starttime", info->boss_starttime) && result;
	result = getAttrValue(element, "boss_endtime", info->boss_endtime) && result;
	result = getAttrValue(element, "buff", info->buff) && result;

	BossForResInfoMap_t::iterator it = m_boss_for_info_map.find(info->map_id);
	if (it != m_boss_for_info_map.end())
	{
		DEF_LOG_ERROR("get reduplicate boss enemy : <%d>\n", info->map_id);
		return false;
	}
	m_boss_for_info_map.insert(std::make_pair(info->map_id, info));


	return result;
}