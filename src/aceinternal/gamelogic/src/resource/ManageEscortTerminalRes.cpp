#include "Logger.h"
#include "ManageEscortTerminalRes.h"
bool ManageEscortTerminalRes::loadContent(Document * xml_doc)
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
const EscortTerminalInfoMap_t & ManageEscortTerminalRes::getEscortTerminalInfo()
{
	return m_escort_terminal_res_map;
}
bool ManageEscortTerminalRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	EscortTerminalInfo * info = new EscortTerminalInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"reward_money", info->reward_money) && result;
	result = getAttrValue(element,"reward_shengwang", info->reward_shengwang) && result;
	result = getAttrValue(element,"reward_yueli", info->reward_yueli) && result;
	result = getAttrValue(element,"time", info->time) && result;
	result = getAttrValue(element,"unlock", info->unlock) && result;

	if (info->level.empty())
	{
		return false;
	}

	if (info->level.size() != info->reward_money.size()
		|| info->level.size() != info->reward_shengwang.size()
		|| info->level.size() != info->reward_yueli.size())
	{
		return false;
	}

	EscortTerminalInfoMap_t::iterator it = m_escort_terminal_res_map.find(info->id);
	if (it != m_escort_terminal_res_map.end())
	{
		DEF_LOG_ERROR("failed to load ESCORTTERMINAL, get reduplicate daily quest function id <%d>\n",info->id);
		return false;
	}
	m_escort_terminal_res_map.insert(std::make_pair(info->id, info));
	return result;
}
