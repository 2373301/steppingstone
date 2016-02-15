
#ifndef MANAGE_PRANDOM_RES_HPP
#define MANAGE_PRANDOM_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManagePrandomRes
{
public:
	~ManagePrandomRes()
	{
		cleanMapSecondContent(m_prandom_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const PrandomResInfoMap_t & getPrandomInfoMap();
protected:
	bool loadInfo(Element * map_element);

private:

	PrandomResInfoMap_t m_prandom_info_map;
};

#endif