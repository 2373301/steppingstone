#ifndef MANAGE_BATTLEGROUND_DATETIME_RES_HPP
#define MANAGE_BATTLEGROUND_DATETIME_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "BattlegroundDatetimeRes.h"
class GAMELOGIC_EXOPRT ManageBattlegroundDatetimeRes
{
public:
	bool loadContent(Document * xml_doc);
	const BattlegroundDatetimeInfoMap_t & getBattlegroundDatetimeInfo();
protected:
	bool loadInfo(Element * element);
private:
	BattlegroundDatetimeInfoMap_t m_Battleground_DateTime_res_map;
};
#endif
