
#ifndef MANAGE_VIP_RES_HPP
#define MANAGE_VIP_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "VipRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageVipRes
{
public:
	~ManageVipRes()
	{
		cleanMapSecondContent(m_vip_res_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const VipInfoMap_t & getVipInfo();
protected:
	bool loadInfo(Element * element);

private:

	VipInfoMap_t m_vip_res_map;
};
#endif