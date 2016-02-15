
#ifndef MANAGE_FLOWER_RES_INFO_HPP
#define MANAGE_FLOWER_RES_INFO_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "FlowerRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageFlowerResInfo
{
public:
	~ManageFlowerResInfo()
	{
		cleanMapSecondContent(m_flower_res_info);
	}
public:
	bool loadContent(Document * xml_doc);

	const FlowerResMap_t & getFlowerResInfo();

protected:
	bool loadInfo(Element * element);

private:

	FlowerResMap_t m_flower_res_info;
};

#endif