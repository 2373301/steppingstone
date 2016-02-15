#ifndef MANAGE_BATTLEGROUND_BUFF_RES_HPP
#define MANAGE_BATTLEGROUND_BUFF_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "BattlegroundBuffRes.h"
class GAMELOGIC_EXOPRT ManageBattlegroundBuffRes
{
public:
	bool loadContent(Document * xml_doc);
	const BattlegroundBuffInfoMap_t & getBattlegroundBuffInfo();
protected:
	bool loadInfo(Element * element);
private:
	BattlegroundBuffInfoMap_t m_Battleground_Buff_res_map;
};
#endif
