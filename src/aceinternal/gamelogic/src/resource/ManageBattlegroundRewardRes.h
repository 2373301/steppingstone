#ifndef MANAGE_BATTLEGROUND_REWARD_RES_HPP
#define MANAGE_BATTLEGROUND_REWARD_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "BattlegroundRewardRes.h"
class GAMELOGIC_EXOPRT ManageBattlegroundRewardRes
{
public:
	bool loadContent(Document * xml_doc);
	const BattlegroundRewardInfoMap_t & getBattlegroundRewardInfo();
protected:
	bool loadInfo(Element * element);
private:
	BattlegroundRewardInfoMap_t m_Battleground_Reward_res_map;
};
#endif
