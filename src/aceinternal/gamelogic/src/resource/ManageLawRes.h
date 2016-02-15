
#ifndef MANAGE_LAW_RES_HPP
#define MANAGE_LAW_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageLawRes
{
public:
	~ManageLawRes()
	{
		cleanVectorContent(m_law_info_vec);
	}
public:
	bool loadContent(Document * xml_doc);

	const LawResInfoVec_t & getLawInfoVec();
protected:
	bool loadInfo(Element * map_element);

private:

	LawResInfoVec_t m_law_info_vec;
};

#endif