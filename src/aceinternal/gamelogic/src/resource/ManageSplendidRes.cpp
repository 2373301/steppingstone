
#include "Logger.h"
#include "ManageSplendidRes.h"

ManageSplendidRes::ManageSplendidRes()
{

}

ManageSplendidRes::~ManageSplendidRes()
{

}

bool ManageSplendidRes::loadSplendidVoteTime(Document * xml_doc)
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
		result = loadVoteTimeInfo(*it) && result;
	}
	return result;
}

bool ManageSplendidRes::loadSplendidRankReward(Document * xml_doc)
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
		result = loadRankRewardInfo(*it) && result;
	}
	return result;
}

bool ManageSplendidRes::loadSplendidWorshipReward(Document * xml_doc)
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
		result = loadWorshipRewardInfo(*it) && result;
	}

	if (m_splendid_worship_reward_map.size() == 0)
	{
		return false;
	}

	return result;
}

const SplendidVoteTimeMap_t & ManageSplendidRes::getSplendidVoteTimeMap()
{
	return m_splendid_vote_time_map;
}

const SplendidRankRewardMap_t & ManageSplendidRes::getSplendidRankRewardMap()
{
	return m_splendid_rank_reward_map;
}

const SplendidWorshipRewardMap_t & ManageSplendidRes::getSplendidWorshipRewardMap()
{
	return m_splendid_worship_reward_map;
}

bool ManageSplendidRes::loadVoteTimeInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;
	SplendidVoteTime *  info  = new SplendidVoteTime();
	result = getAttrValue(element,"id", info->id) && result;
	result = getAttrValue(element,"type", info->type_value) && result;
	result = getAttrValue(element,"start_time", info->start_time) && result;
	result = getAttrValue(element,"voting_start_time", info->vote_start_time) && result;
	if (2 != info->vote_start_time.size())
	{
		DEF_LOG_ERROR("error to config vote start time, the size should be 2, but it is <%d>\n", info->vote_start_time.size());
		return false;
	}
	result = getAttrValue(element,"voting_end_time", info->vote_end_time) && result;
	if (3 != info->vote_end_time.size())
	{
		DEF_LOG_ERROR("error to config vote end time, the size should be 3, but it is <%d>\n", info->vote_start_time.size());
		return false;
	}

	result = getAttrValue(element,"periodtime", info->period_time) && result;
	result = getAttrValue(element,"min_time", info->min_time) && result;

	SplendidVoteTimeMap_t::iterator it = m_splendid_vote_time_map.find(info->id);
	if (it != m_splendid_vote_time_map.end())
	{
		return false;
	}

	m_splendid_vote_time_map[info->id] = info;

	return true;
}

bool ManageSplendidRes::loadRankRewardInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;
	SplendidRankReward *  info  = new SplendidRankReward();
	result = getAttrValue(element,"rank", info->id) && result;
	result = getAttrValue(element,"bonus_id", info->bonus_id) && result;

	SplendidRankRewardMap_t::iterator it = m_splendid_rank_reward_map.find(info->id);
	if (it != m_splendid_rank_reward_map.end())
	{
		return false;
	}

	m_splendid_rank_reward_map.insert(std::make_pair(info->id, info));

	return true;
}

bool ManageSplendidRes::loadWorshipRewardInfo(Element * element)
{
	if (NULL == element)
	{
		return false;
	}

	bool result = true;
	SplendidWorshipReward *  info  = new SplendidWorshipReward();
	result = getAttrValue(element,"level", info->level) && result;
	result = getAttrValue(element,"worship_rewards", info->worship_reward) && result;
	result = getAttrValue(element,"worshiped_rewards", info->be_worshiped_reward) && result;
	result = getAttrValue(element,"worship_num", info->worship_num_vec) && result;
	result = getAttrValue(element,"rank_rewards", info->worship_num_reward_vec) && result;
	result = getAttrValue(element,"vote_rewards", info->vote_reward) && result;
	result = getAttrValue(element,"worshiped_limit", info->worshiped_limit) && result;

	if (info->worship_num_vec.size() != info->worship_num_reward_vec.size())
	{
		return false;
	}

	m_splendid_worship_reward_map.push_back(info);
	
	return true;
}