#ifndef MANAGE_DAILYPOINTS_RES_H
#define MANAGE_DAILYPOINTS_RES_H

#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "DailyPointsRes.h"
#include "share_fun.h"

class ManageDailyPointsRes
{
public:
	~ManageDailyPointsRes()
	{
		cleanMapSecondContent(m_dailypoints_events_map);
		cleanMapSecondContent(m_dailypoints_reward_map);
	}
public:
	bool loadDailyPointsEevent(Document * xml_doc);
	bool loadDailyPointsReward(Document * xml_doc);

	const DailyPointsEventsMap_t & getDailyPointsEeventMap();
	const DailyPointsRewardMap_t & getDailyPointsRewardMap();

protected:
	bool loadDailyPointsEeventInfo(Element * element);
	bool loadDailyPointsRewardInfo(Element * element);

private:

	DailyPointsEventsMap_t	m_dailypoints_events_map;
	DailyPointsRewardMap_t  m_dailypoints_reward_map;
};
#endif