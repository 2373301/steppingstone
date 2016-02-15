#ifndef MANAGE_ACHIEVEMENT_RES_H
#define MANAGE_ACHIEVEMENT_RES_H

#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "AchievementRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageAchievementRes
{
public:
	~ManageAchievementRes()
	{
		cleanMapSecondContent(m_achievement_res_map);
		cleanMapSecondContent(m_achievement_reward_map);
		cleanMapSecondContent(m_achievement_unlock_map);
	}
public:
	bool loadContentAchieveFun(Document * xml_doc);
	bool loadContentAchievement(Document * xml_doc);
	bool loadContentAchieveScore(Document * xml_doc);


	const AchievementInfoMap_t & getAchievementInfo();
	const AchievementRewardMap_t & getAchievementReward();
	const AchievementUnlockMap_t & getAchievementUnlock();

protected:
	bool loadAchieveFunInfo(Element * element);
	bool loadAchievementInfo(Element * element);
	bool loadAchieveScoreInfo(Element * element);

private:

	AchievementInfoMap_t	m_achievement_res_map;
	AchievementRewardMap_t  m_achievement_reward_map;
	AchievementUnlockMap_t  m_achievement_unlock_map;
};
#endif