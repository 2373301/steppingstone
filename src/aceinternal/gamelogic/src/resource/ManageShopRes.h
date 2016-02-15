
#ifndef MANAGE_SHOP_RES_PP
#define MANAGE_SHOP_RES_PP

#pragma warning(push, 2)
#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"
#pragma warning(pop)

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include "share_fun.h"

class /*GAMELOGIC_EXOPRT*/ ManageShopRes
{
public:
	~ManageShopRes()
	{
	}

public:
	bool loadContent(Document * xml_doc);

	const ShopInfoMap & getShopInfoMap();

private:
	bool loadInfo(Element * element);

    ShopInfoMap shopInfoMap;
    boost::ptr_map<uint32, ShopInfo> shopInfoMapR;

    typedef boost::ptr_vector<ShopGrid> ShopContents;
    typedef boost::ptr_map<uint32, ShopContents> ShopMap;
    ShopMap shop_map;
};

#endif
