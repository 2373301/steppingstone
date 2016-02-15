#ifndef MANAGE_EQUIP_GETTING__RES_HPP
#define MANAGE_EQUIP_GETTING__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "Equip_gettingRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageEquip_gettingRes
{
public:
	~ManageEquip_gettingRes()
	{
		cleanMapSecondContent(m_equip_getting__res_map);
	}
	bool loadContent(Document * xml_doc);
	const Equip_gettingInfoMap_t & getEquip_gettingInfo();
protected:
	bool loadInfo(Element * element);
private:
	Equip_gettingInfoMap_t m_equip_getting__res_map;
};
#endif
