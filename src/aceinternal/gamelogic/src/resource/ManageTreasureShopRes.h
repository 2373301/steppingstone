#ifndef MANAGE_TREASURE_SHOP_RES_HPP
#define MANAGE_TREASURE_SHOP_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TreasureShopRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageTreasureShopRes
{
public:
	~ManageTreasureShopRes()
	{
		cleanMapSecondContent(m_treasure_shop_res_map);
	}
	bool loadContent(Document * xml_doc);
	const TreasureShopInfoMap_t & getTreasureShopInfo();
protected:
	bool loadInfo(Element * element);
private:
	TreasureShopInfoMap_t m_treasure_shop_res_map;
};
#endif
