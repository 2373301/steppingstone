
#ifndef MANAGE_SPLENDID_RES_HPP
#define MANAGE_SPLENDID_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SplendidRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageSplendidRes
{
public:
	ManageSplendidRes();
	~ManageSplendidRes();
public:
	bool loadSplendidVoteTime(Document * xml_doc);

	bool loadSplendidRankReward(Document * xml_doc);

	bool loadSplendidWorshipReward(Document * xml_doc);

	const SplendidVoteTimeMap_t & getSplendidVoteTimeMap();

	const SplendidRankRewardMap_t & getSplendidRankRewardMap();

	const SplendidWorshipRewardMap_t & getSplendidWorshipRewardMap();
protected:
	bool loadVoteTimeInfo(Element * element);

	bool loadRankRewardInfo(Element * element);

	bool loadWorshipRewardInfo(Element * element);
private:
	SplendidVoteTimeMap_t m_splendid_vote_time_map;

	SplendidRankRewardMap_t m_splendid_rank_reward_map;

	SplendidWorshipRewardMap_t m_splendid_worship_reward_map;
};

#endif