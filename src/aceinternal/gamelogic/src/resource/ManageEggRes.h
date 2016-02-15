
#ifndef MANAGE_EGG_RES_HPP
#define MANAGE_EGG_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageEggRes
{
public:
	~ManageEggRes()
	{
		cleanVectorContent(m_egg_info_vec);
	}
public:
	bool loadContent(Document * xml_doc);

	const EggResInfoVec_t & getEggInfo();
protected:
	bool loadInfo(Element * element);

private:

	EggResInfoVec_t m_egg_info_vec;
};

#endif