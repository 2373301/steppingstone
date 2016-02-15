
#ifndef MANAGE_DAILY_QUEST_RES_HPP
#define MANAGE_DAILY_QUEST_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "DailyQuestRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageDailyQuestRes
{
public:
	~ManageDailyQuestRes()
	{
		cleanMapSecondContent(m_daily_quest_res_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const DailyQuestInfoMap_t & getDailyQuestInfo();
protected:
	bool loadInfo(Element * element);

private:

	DailyQuestInfoMap_t m_daily_quest_res_map;
};
#endif