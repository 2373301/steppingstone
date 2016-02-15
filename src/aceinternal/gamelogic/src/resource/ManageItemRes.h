
#ifndef MANAGE_ITEM_RES_HPP
#define MANAGE_ITEM_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageItemRes
{
public:
	~ManageItemRes()
	{
		cleanMapSecondContent(m_item_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const ItemResInfoMap_t & getItemInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	ItemResInfoMap_t m_item_info_map;
};

#endif