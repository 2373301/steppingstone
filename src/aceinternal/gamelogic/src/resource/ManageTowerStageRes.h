#ifndef MANAGE_TOWER_STAGE_RES_HPP
#define MANAGE_TOWER_STAGE_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TowerStageRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageTowerStageRes
{
public:
	~ManageTowerStageRes()
	{
		cleanMapSecondContent(m_tower_stage_res_map);
	}
public:
	bool loadContent(Document * xml_doc);
	const TowerStageInfoMap_t & getTowerStageInfo();

protected:
	bool loadInfo(Element * element);
private:
	TowerStageInfoMap_t m_tower_stage_res_map;
};
#endif
