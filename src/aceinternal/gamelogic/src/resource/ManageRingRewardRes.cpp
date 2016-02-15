#include "Logger.h"
#include "ManageRingRewardRes.h"
bool ManageRingRewardRes::loadContent(Document * xml_doc)
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
const RingRewardInfoMap_t & ManageRingRewardRes::getRingRewardInfo()
{
	return m_ring_reward_res_map;
}
bool ManageRingRewardRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	RingRewardInfo * info = new RingRewardInfo();
	result = getAttrValue(element,"bonus_id1", info->bonus_id1) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"level1", info->level1) && result;
	result = getAttrValue(element,"ring_quest_max", info->ring_quest_max) && result;
	result = getAttrValue(element, "ring_loop", info->ring_loop) && result;
	if (info->bonus_id1.size() != info->level1.size())
	{
		DEF_LOG_ERROR("info->bonus_id1.size() != info->level1.size()");
		return false;
	}
	RingRewardInfoMap_t::iterator it = m_ring_reward_res_map.find(info->id);
	if (it != m_ring_reward_res_map.end())
	{
		DEF_LOG_ERROR("failed to load RINGREWARD, get reduplicate id <%d>\n",info->id);
		return false;
	}
	m_ring_reward_res_map.insert(std::make_pair(info->id, info));
	return result;
}
