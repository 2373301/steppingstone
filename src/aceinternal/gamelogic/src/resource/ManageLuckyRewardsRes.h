#ifndef MANAGE_LUCKY_REWARDS_RES_HPP
#define MANAGE_LUCKY_REWARDS_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "LuckyRewardsRes.h"
class GAMELOGIC_EXOPRT ManageLuckyRewardsRes
{
public:
	bool loadContent(Document * xml_doc);
	const LuckyRewardsInfoVec_t & getLuckyRewardsInfo();
protected:
	bool loadInfo(Element * element);
private:
	LuckyRewardsInfoVec_t m_Lucky_Rewards_res_vec;
};
#endif
