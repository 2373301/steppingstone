#include "Logger.h"
#include "ManagePlumRewardRes.h"
bool ManagePlumRewardRes::loadContent(Document * xml_doc)
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
const PlumRewardInfoVec_t & ManagePlumRewardRes::getPlumRewardInfo()
{
	return m_plum_reward_res_vec;
}
bool ManagePlumRewardRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	PlumRewardInfo * info = new PlumRewardInfo();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level_group", info->level_group) && result;
	uint32 reward_type = 0;
	uint32 reward_value = 0;
	result = getAttrValue(element,"resource1", reward_type) && result;
	result = getAttrValue(element,"resource1_num", reward_value) && result;
	if (reward_type != 0)
	{
		info->reward_type.push_back(reward_type);
		info->reward_value.push_back(reward_value);
	}
	result = getAttrValue(element,"resource2", reward_type) && result;
	result = getAttrValue(element,"resource2_num", reward_value) && result;
	if (reward_type != 0)
	{
		info->reward_type.push_back(reward_type);
		info->reward_value.push_back(reward_value);
	}
	result = getAttrValue(element,"resource3", reward_type) && result;
	result = getAttrValue(element,"resource3_num", reward_value) && result;
	if (reward_type != 0)
	{
		info->reward_type.push_back(reward_type);
		info->reward_value.push_back(reward_value);
	}
	result = getAttrValue(element,"type", info->_type) && result;
	m_plum_reward_res_vec.push_back(info);
	return result;
}
