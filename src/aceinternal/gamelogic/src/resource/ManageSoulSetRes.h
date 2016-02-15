#ifndef MANAGE_SOUL_SET_RES_HPP
#define MANAGE_SOUL_SET_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SoulSetRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageSoulSetRes
{
public:
	~ManageSoulSetRes()
	{
		cleanMapSecondContent(m_soul_set_res_map);
	}
	bool loadContent(Document * xml_doc);
	const SoulSetInfoMap_t & getSoulSetInfo();
protected:
	bool loadInfo(Element * element);
private:
	SoulSetInfoMap_t m_soul_set_res_map;
};
#endif
