
#ifndef MANAGE_ITEM_STRENG_RES_HPP
#define MANAGE_ITEM_STRENG_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageItemStrengRes
{
public:
	~ManageItemStrengRes()
	{
		cleanMapSecondContent(m_item_streng_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const ItemStrengResInfoMap_t & getItemStrengInfoMap();
protected:
	bool loadInfo(Element * map_element);

private:

	ItemStrengResInfoMap_t m_item_streng_info_map;
};

#endif
