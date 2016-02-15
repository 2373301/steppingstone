#include "Logger.h"
#include "ManageBattlegroundBetRes.h"
bool ManageBattlegroundBetRes::loadContent(Document * xml_doc)
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
const BattlegroundBetInfoMap_t & ManageBattlegroundBetRes::getBattlegroundBetInfo()
{
	return m_Battleground_Bet_res_map;
}
bool ManageBattlegroundBetRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	BattlegroundBetInfo * info = new BattlegroundBetInfo();
	result = getAttrValue(element,"bet_a1", info->bet_a1) && result;
	result = getAttrValue(element,"bet_a2", info->bet_a2) && result;
	result = getAttrValue(element,"bet_a3", info->bet_a3) && result;
	result = getAttrValue(element,"faction_id", info->faction_id) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level_a1", info->level_a1) && result;
	BattlegroundBetInfoMap_t::iterator it = m_Battleground_Bet_res_map.find(info->id);
	if (it != m_Battleground_Bet_res_map.end())
	{
		DEF_LOG_ERROR("failed to load BATTLEGROUNDBET, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Battleground_Bet_res_map.insert(std::make_pair(info->id, info));
	return result;
}
