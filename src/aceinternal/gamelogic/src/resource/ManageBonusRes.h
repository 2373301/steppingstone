
#ifndef MANAGE_BONUS_RES_HPP
#define MANAGE_BONUS_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageBonusRes
{
public:
	~ManageBonusRes()
	{
		cleanMapSecondContent(m_bonus_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const BonusResInfoMap_t & getBonusInfoMap();
protected:
	bool loadInfo(Element * map_element);

private:

	BonusResInfoMap_t m_bonus_info_map;
};

#endif