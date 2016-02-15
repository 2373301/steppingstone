
#ifndef MANAGE_BOSS_GUWU_RES_HPP
#define MANAGE_BOSS_GUWU_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageBossGuWuRes
{
public:
	~ManageBossGuWuRes()
	{
		cleanMapSecondContent(m_boss_guwu_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const BossGuWuResInfoMap_t & getBossGuWuInfo();
protected:
	bool loadInfo(Element * element);

private:

	BossGuWuResInfoMap_t m_boss_guwu_info_map;
};

#endif