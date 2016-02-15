
#ifndef MANAGE_BOSS_FOR_RES_HPP
#define MANAGE_BOSS_FOR_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageBossForRes
{
public:
	~ManageBossForRes()
	{
		cleanMapSecondContent(m_boss_for_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const BossForResInfoMap_t & getBossForInfo();
protected:
	bool loadInfo(Element * element);

private:

	BossForResInfoMap_t m_boss_for_info_map;
};

#endif