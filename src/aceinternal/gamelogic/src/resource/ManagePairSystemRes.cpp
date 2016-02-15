#include "Logger.h"
#include "ManagePairSystemRes.h"
bool ManagePairSystemRes::loadContent(Document * xml_doc)
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
const PairSystemInfo & ManagePairSystemRes::getPairSystemInfo()
{
	return m_pair_system_res_;
}
bool ManagePairSystemRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	PairSystemInfo * info = &m_pair_system_res_;
	result = getAttrValue(element,"battle_num", info->battle_num) && result;
	result = getAttrValue(element,"battletimes", info->battletimes) && result;
	result = getAttrValue(element,"benfu_first_reward1", info->benfu_first_reward1) && result;
	result = getAttrValue(element,"benfu_first_reward2", info->benfu_first_reward2) && result;
	result = getAttrValue(element,"benfu_first_reward3", info->benfu_first_reward3) && result;
	result = getAttrValue(element,"benfu_next_reward", info->benfu_next_reward) && result;
	result = getAttrValue(element,"cd_time", info->cd_time) && result;
	result = getAttrValue(element,"defeatedtimes1", info->defeatedtimes1) && result;
	result = getAttrValue(element,"defeatedtimes2", info->defeatedtimes2) && result;
	result = getAttrValue(element,"di_begintime", info->di_begintime) && result;
	result = getAttrValue(element,"initial_name", info->initial_name) && result;
	result = getAttrValue(element,"join_max", info->join_max) && result;
	result = getAttrValue(element,"jointime1", info->jointime1) && result;
	result = getAttrValue(element,"jointime2", info->jointime2) && result;
	result = getAttrValue(element,"kuafu_begintime", info->kuafu_begintime) && result;
	result = getAttrValue(element,"kuafu_first_reward1", info->kuafu_first_reward1) && result;
	result = getAttrValue(element,"kuafu_first_reward2", info->kuafu_first_reward2) && result;
	result = getAttrValue(element,"kuafu_first_reward3", info->kuafu_first_reward3) && result;
	result = getAttrValue(element,"kuafu_next_reward", info->kuafu_next_reward) && result;
	result = getAttrValue(element,"leader_chenghao", info->leader_chenghao) && result;
	result = getAttrValue(element,"part1begintime", info->part1begintime) && result;
	result = getAttrValue(element,"part1num", info->part1num) && result;
	result = getAttrValue(element,"part1rundtime", info->part1rundtime) && result;
	result = getAttrValue(element,"part2begintime1", info->part2begintime1) && result;
	result = getAttrValue(element,"part2begintime2", info->part2begintime2) && result;
	result = getAttrValue(element,"part2begintime3", info->part2begintime3) && result;
	result = getAttrValue(element,"part2begintime4", info->part2begintime4) && result;
	result = getAttrValue(element,"part2rundtime", info->part2rundtime) && result;
	result = getAttrValue(element,"partner_chenghao", info->partner_chenghao) && result;
	result = getAttrValue(element,"return", info->_return) && result;
	result = getAttrValue(element,"team_level", info->team_level) && result;
	result = getAttrValue(element,"team_money", info->team_money) && result;
	result = getAttrValue(element,"name_min", info->name_min) && result;
	result = getAttrValue(element,"name_max", info->name_max) && result;

	return result;
}
