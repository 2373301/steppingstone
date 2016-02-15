
#ifndef MANAGE_SPECIAL_SHOP_RES_PP
#define MANAGE_SPECIAL_SHOP_RES_PP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageSpecialShopRes
{
public:
	~ManageSpecialShopRes()
	{
		cleanMapSecondContent(m_special_shop_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const SpecialShopResInfoMap_t & getSpecialShopInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	SpecialShopResInfoMap_t m_special_shop_info_map;
};

#endif