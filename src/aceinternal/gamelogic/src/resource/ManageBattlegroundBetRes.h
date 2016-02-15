#ifndef MANAGE_BATTLEGROUND_BET_RES_HPP
#define MANAGE_BATTLEGROUND_BET_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "BattlegroundBetRes.h"
class GAMELOGIC_EXOPRT ManageBattlegroundBetRes
{
public:
	bool loadContent(Document * xml_doc);
	const BattlegroundBetInfoMap_t & getBattlegroundBetInfo();
protected:
	bool loadInfo(Element * element);
private:
	BattlegroundBetInfoMap_t m_Battleground_Bet_res_map;
};
#endif
