
#ifndef MANAGE_HIRE_RES_HPP
#define MANAGE_HIRE_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageHireRes
{
public:
	~ManageHireRes()
	{
		cleanMapSecondContent(m_hire_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const HireResInfoMap_t & getHireInfoMap();
protected:
	bool loadInfo(Element * map_element);

private:

	HireResInfoMap_t m_hire_info_map;
};

#endif