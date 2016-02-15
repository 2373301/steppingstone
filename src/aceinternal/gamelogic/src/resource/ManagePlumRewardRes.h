#ifndef MANAGE_PLUM_REWARD_RES_HPP
#define MANAGE_PLUM_REWARD_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PlumRewardRes.h"
class GAMELOGIC_EXOPRT ManagePlumRewardRes
{
public:
	bool loadContent(Document * xml_doc);
	const PlumRewardInfoVec_t & getPlumRewardInfo();
protected:
	bool loadInfo(Element * element);
private:
	PlumRewardInfoVec_t m_plum_reward_res_vec;
};
#endif
