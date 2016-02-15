#include "Logger.h"
#include "ManageBattlegroundDatetimeRes.h"
bool ManageBattlegroundDatetimeRes::loadContent(Document * xml_doc)
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
const BattlegroundDatetimeInfoMap_t & ManageBattlegroundDatetimeRes::getBattlegroundDatetimeInfo()
{
	return m_Battleground_DateTime_res_map;
}
bool ManageBattlegroundDatetimeRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	BattlegroundDatetimeInfo * info = new BattlegroundDatetimeInfo();
	result = getAttrValue(element,"end_time", info->end_time) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"ready_time", info->ready_time) && result;
	result = getAttrValue(element,"round_time", info->round_time) && result;
	result = getAttrValue(element,"start_time", info->start_time) && result;
	result = getAttrValue(element,"cd1", info->cd1) && result;
	result = getAttrValue(element,"cd2", info->cd2) && result;
	result = getAttrValue(element,"cd3", info->cd3) && result;
	result = getAttrValue(element,"cd4", info->cd4) && result;
	BattlegroundDatetimeInfoMap_t::iterator it = m_Battleground_DateTime_res_map.find(info->id);
	if (it != m_Battleground_DateTime_res_map.end())
	{
		DEF_LOG_ERROR("failed to load BATTLEGROUNDDATETIME, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_Battleground_DateTime_res_map.insert(std::make_pair(info->id, info));
	return result;
}
