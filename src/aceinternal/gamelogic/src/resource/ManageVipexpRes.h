
#ifndef MANAGE_VIPEXP_RES_HPP
#define MANAGE_VIPEXP_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageVipexpRes
{
public:
	~ManageVipexpRes()
	{
		cleanMapSecondContent(m_vipexp_res_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const VipexpResInfoMap_t & getVipexpInfo();
protected:
	bool loadInfo(Element * element);

private:

	VipexpResInfoMap_t m_vipexp_res_map;
private:
};
#endif