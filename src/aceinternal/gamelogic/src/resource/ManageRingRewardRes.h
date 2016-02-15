#ifndef MANAGE_RING_REWARD_RES_HPP
#define MANAGE_RING_REWARD_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RingRewardRes.h"
class GAMELOGIC_EXOPRT ManageRingRewardRes
{
public:
	bool loadContent(Document * xml_doc);
	const RingRewardInfoMap_t & getRingRewardInfo();
protected:
	bool loadInfo(Element * element);
private:
	RingRewardInfoMap_t m_ring_reward_res_map;
};
#endif
