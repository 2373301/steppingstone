
#ifndef MANAGE_DAILY_QUEST_LIBRARY_RES_HPP
#define MANAGE_DAILY_QUEST_LIBRARY_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "DailyQuestLibraryRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageDailyQuestLibraryRes
{
public:
	~ManageDailyQuestLibraryRes()
	{
		cleanVectorContent(m_daily_quest_library_res_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const DailyQuestLibraryInfoVec_t & getDailyQuestLibraryInfo();
protected:
	bool loadInfo(Element * element);

private:

	DailyQuestLibraryInfoVec_t m_daily_quest_library_res_map;
};
#endif