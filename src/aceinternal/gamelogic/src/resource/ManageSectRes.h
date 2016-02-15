
#ifndef MANAGE_SECT_RES_HPP
#define MANAGE_SECT_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageSectRes
{
public:
	~ManageSectRes()
	{
		cleanMapSecondContent(m_sect_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const SectResInfoMap_t & getSectInfoMap();
protected:
	bool loadInfo(Element * map_element);

private:

	SectResInfoMap_t m_sect_info_map;
};

#endif