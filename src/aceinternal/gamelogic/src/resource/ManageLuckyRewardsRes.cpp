#include "Logger.h"
#include "ManageLuckyRewardsRes.h"
bool ManageLuckyRewardsRes::loadContent(Document * xml_doc)
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
const LuckyRewardsInfoVec_t & ManageLuckyRewardsRes::getLuckyRewardsInfo()
{
	return m_Lucky_Rewards_res_vec;
}
bool ManageLuckyRewardsRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	LuckyRewardsInfo * info = new LuckyRewardsInfo();
	result = getAttrValue(element,"id", info->id) && result;
	//result = getAttrValue(element,"light", info->light) && result;
	result = getAttrValue(element,"num", info->num) && result;
	result = getAttrValue(element,"percent_gold", info->percent_gold) && result;
	result = getAttrValue(element,"percent_item", info->percent_item) && result;
	result = getAttrValue(element,"percent_num", info->percent_num) && result;
	//result = getAttrValue(element,"spectial_icon", info->spectial_icon) && result;
	//result = getAttrValue(element,"spectial_tips", info->spectial_tips) && result;
	result = getAttrValue(element,"talk", info->talk) && result;
	result = getAttrValue(element,"type", info->type) && result;
	result = getAttrValue(element,"value", info->value) && result;
    result = getAttrValue(element,"condition", info->condition) && result;

	m_Lucky_Rewards_res_vec.push_back(info);
	return result;
}
