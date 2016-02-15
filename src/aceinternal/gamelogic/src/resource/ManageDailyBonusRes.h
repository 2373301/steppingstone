
#ifndef MANAGE_DAILY_BONUS_RES_HPP
#define MANAGE_DAILY_BONUS_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageDailyBonusRes
{
public:
	~ManageDailyBonusRes()
	{
		cleanMapSecondContent(m_daily_bonus_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const DailyBonusResInfoMap_t & getDailyBonusInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	DailyBonusResInfoMap_t m_daily_bonus_info_map;
};

#endif