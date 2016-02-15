#include "Logger.h"
#include "ManageRankingRewardRes.h"
bool ManageRankingRewardRes::loadContent(Document * xml_doc)
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
const RankingRewardInfoMap_t & ManageRankingRewardRes::getRankingRewardInfo()
{
	return m_Ranking_Reward_res_map;
}
bool ManageRankingRewardRes::loadInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}
	bool result = true;
	RankingRewardInfo * info = new RankingRewardInfo();
	result = getAttrValue(element,"hero_point", info->hero_point) && result;
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"player_level", info->player_level) && result;
	result = getAttrValue(element,"rank_level", info->rank_level) && result;
	result = getAttrValue(element,"rank_rewards", info->rank_rewards) && result;
	RankingRewardInfoMap_t::iterator it = m_Ranking_Reward_res_map.find(info->id);
    RankingRewardPtr obj;
    bool existed = false;
	if (it != m_Ranking_Reward_res_map.end())
	{
		obj = it->second;
        existed = true;
	}
    else
    {
        obj.reset(new RankingRewardVec);
    }

    obj->push_back(info);
    if( !existed)
        m_Ranking_Reward_res_map.insert(std::make_pair(info->id, obj));
	

	return result;
}
