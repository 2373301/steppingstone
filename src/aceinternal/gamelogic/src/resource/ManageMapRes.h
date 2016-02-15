
#ifndef MANAGE_MAP_RES_HPP
#define MANAGE_MAP_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageMapRes
{
public:
	~ManageMapRes();
public:
	bool loadMapContent(Document * xml_doc);

	const MapResInfoMap_t & getMapInfoMap();
protected:
	bool loadMapInfo(Element * map_element);

private:

	MapResInfoMap_t m_map_info_map;
};

#endif