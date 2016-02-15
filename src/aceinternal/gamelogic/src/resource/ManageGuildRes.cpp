#include "Logger.h"
#include "ManageGuildRes.h"

bool ManageGuildRes::loadGuildNity(Document * xml_doc)
{
	if (NULL == xml_doc) return false;

	Element * root_ele = xml_doc->get_root();
	if (NULL == root_ele) return false;

	bool result = true;
	Elements ele_list = root_ele->get_elements();
	for (Elements::iterator it = ele_list.begin(); it != ele_list.end(); ++it)
	{
		result = loadGuildNityMapInfo(*it) && result;
	}

	return result;
}

bool ManageGuildRes::loadGuildActivity(Document * xml_doc)
{
	if (NULL == xml_doc) return false;

	Element * root_ele = xml_doc->get_root();
	if (NULL == root_ele) return false;

	bool result = true;
	Elements ele_list = root_ele->get_elements();
	for (Elements::iterator it = ele_list.begin(); it != ele_list.end(); ++it)
	{
		result = loadGuildActivityInfo(*it) && result;
	}

	return result;
}

const Guild_NityMap_t & ManageGuildRes::getGuildNityMap()
{
	return m_guild_nity_map;
}

const Guild_ActivityVector_t & ManageGuildRes::getGuildActivityVector()
{
	return m_guild_activity_vector;
}

bool ManageGuildRes::loadGuildNityMapInfo(Element * element)
{
	if (NULL == element) return false;

	bool result = true;
	Guild_Nity * info = new Guild_Nity();

	result = getAttrValue(element, "level", info->level) && result;
	result = getAttrValue(element, "player_num", info->player_num) && result;
	result = getAttrValue(element, "exp", info->exp) && result;
	result = getAttrValue(element, "systemunlock", info->vec_systemunlock) && result;
	result = getAttrValue(element, "pic", info->vec_pic) && result;
	result = getAttrValue(element, "impeach_condition", info->impeach_condition) && result;
	result = getAttrValue(element, "impeach_cost", info->impeach_cost) && result;
	result = getAttrValue(element, "impeach_costnum", info->impeach_costnum) && result;
	result = getAttrValue(element, "impeach_time", info->impeach_time) && result;
	result = getAttrValue(element, "impeach_playernum", info->impeach_playernum) && result;
	result = getAttrValue(element, "impeach_gold", info->impeach_gold) && result;

	Guild_NityMap_t::iterator it = m_guild_nity_map.find(info->level);
	if (it != m_guild_nity_map.end())
	{
		DEF_LOG_ERROR("failed to load confraternity.xml, get reduplicate  level <%d>\n", info->level);
		return false;
	}

	m_guild_nity_map.insert(std::make_pair(info->level, info));
	return result;
}

bool ManageGuildRes::loadGuildActivityInfo(Element * element)
{
	if (NULL == element) return false;
	
	bool result = true;
	uint32 activity_id = 0;
	result = getAttrValue(element, "confrateractivity", activity_id) && result;

	Guild_ActivityVector_t::iterator it = std::find(m_guild_activity_vector.begin(),m_guild_activity_vector.end(), activity_id);
	if (it != m_guild_activity_vector.end())
	{
		DEF_LOG_ERROR("failed to load confrateractivity.xml, get reduplicate confrateractivity <%d>\n", activity_id);
		return false;
	}

	m_guild_activity_vector.push_back(activity_id);
	return result;
}