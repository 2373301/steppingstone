#ifndef MANAGE_TOWER_BUFF_RES_HPP
#define MANAGE_TOWER_BUFF_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TowerBuffRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageTowerBuffRes
{
public:
	~ManageTowerBuffRes()
	{
		cleanMapSecondContent(m_tower_Buff_res_map);
	}
public:
	bool loadContent(Document * xml_doc);
	const TowerBuffInfoMap_t & getTowerBuffInfo();
protected:
	bool loadInfo(Element * element);
private:
	TowerBuffInfoMap_t m_tower_Buff_res_map;
};
#endif
