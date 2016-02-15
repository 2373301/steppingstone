
#ifndef MANAGE_HAMMER_RES_HPP
#define MANAGE_HAMMER_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageHammerRes
{
public:
	~ManageHammerRes()
	{
		cleanMapSecondContent(m_hammer_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const HammerResInfoMap_t & getHammerInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	HammerResInfoMap_t m_hammer_info_map;
};

#endif