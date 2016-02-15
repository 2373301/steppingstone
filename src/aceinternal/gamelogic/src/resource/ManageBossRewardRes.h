
#ifndef MANAGE_BOSS_REWARD_RES_HPP
#define MANAGE_BOSS_REWARD_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageBossRewardRes
{
public:
	~ManageBossRewardRes()
	{
		cleanMapSecondContent(m_boss_reward_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const BossRewardResInfoMap_t & getBossRewardInfo();
protected:
	bool loadInfo(Element * element);

private:

	BossRewardResInfoMap_t m_boss_reward_info_map;
};

#endif