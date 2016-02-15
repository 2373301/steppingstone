#ifndef MANAGE_TOWER_REWARDS_RES_HPP
#define MANAGE_TOWER_REWARDS_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TowerRewardsRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageTowerRewardsRes
{
public:
	~ManageTowerRewardsRes()
	{
		cleanMapSecondContent(m_tower_rewards_res_map);
	}
public:
	bool loadContent(Document * xml_doc);
	const TowerRewardsInfoMap_t & getTowerRewardsInfo();
protected:
	bool loadInfo(Element * element);
private:
	TowerRewardsInfoMap_t m_tower_rewards_res_map;
};
#endif
