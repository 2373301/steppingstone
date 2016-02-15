
#ifndef MANAGE_PRACTICE_RES_HPP
#define MANAGE_PRACTICE_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PracticeRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManagePracticeRes
{
public:
	~ManagePracticeRes()
	{
		cleanMapSecondContent(m_wine_tank_res_map);
	}
public:
	bool loadWineTank(Document * xml_doc);
	bool loadGourd(Document * xml_doc);
	bool loadStomach(Document * xml_doc);
	bool loadWineIncrease(Document * xml_doc);
	bool loadTankUpgrade(Document * xml_doc);

public:
	const WineTankResInfoMap_t & getWineTankResInfoMap();
	const GourdResInfo & getGourdResInfo();
	const StomachResInfo & getStomachResInfo();
	const WineIncreaseResInfo & getWineIncreaseResInfo();
	const WineTankUpgradeResInfo & getWineTankUpgradeResInfo();

protected:
	bool loadWineTankInfo(Element * element);

private:
	WineTankResInfoMap_t m_wine_tank_res_map;
	GourdResInfo		m_gourd_res_info;
	StomachResInfo		m_stomach_res_info;
	WineIncreaseResInfo	m_wine_increase_res_info;
protected:
	WineTankUpgradeResInfo m_wine_tank_upgrade_res_info;
};
#endif