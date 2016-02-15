#ifndef MANAGE_GOLD_CARD_RES_HPP
#define MANAGE_GOLD_CARD_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "GoldCardRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageGoldCardRes
{
public:
	~ManageGoldCardRes()
	{
		cleanMapSecondContent(m_gold_card_res_map);
	}
	bool loadContent(Document * xml_doc);
	const GoldCardInfoMap_t & getGoldCardInfo();
protected:
	bool loadInfo(Element * element);
private:
	GoldCardInfoMap_t m_gold_card_res_map;
};
#endif
