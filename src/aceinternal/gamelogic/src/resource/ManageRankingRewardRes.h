#ifndef MANAGE_RANKING_REWARD_RES_HPP
#define MANAGE_RANKING_REWARD_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RankingRewardRes.h"
class GAMELOGIC_EXOPRT ManageRankingRewardRes
{
public:
	bool loadContent(Document * xml_doc);
	const RankingRewardInfoMap_t & getRankingRewardInfo();
protected:
	bool loadInfo(Element * element);
private:
	RankingRewardInfoMap_t m_Ranking_Reward_res_map;
};
#endif
