
#ifndef MANAGE_QUEST_RES_HPP
#define MANAGE_QUEST_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageQuestRes
{
public:
	~ManageQuestRes()
	{
		cleanMapSecondContent(m_quest_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const QuestResInfoMap_t & getQuestInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	QuestResInfoMap_t m_quest_info_map;
};

#endif