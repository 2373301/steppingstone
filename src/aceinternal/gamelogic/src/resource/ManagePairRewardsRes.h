#ifndef MANAGE_PAIR_REWARDS_RES_HPP
#define MANAGE_PAIR_REWARDS_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PairRewardsRes.h"
class GAMELOGIC_EXOPRT ManagePairRewardsRes
{
public:
	bool loadContent(Document * xml_doc);
	const PairRewardsInfoVec_t & getPairRewardsInfo();
protected:
	bool loadInfo(Element * element);
private:
	PairRewardsInfoVec_t m_pair_rewards_res_vec;
};
#endif
