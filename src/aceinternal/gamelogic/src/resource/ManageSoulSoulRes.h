#ifndef MANAGE_SOUL_SOUL_RES_HPP
#define MANAGE_SOUL_SOUL_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SoulSoulRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageSoulSoulRes
{
public:
	~ManageSoulSoulRes()
	{
		cleanMapSecondContent(m_soul_soul_res_map);
	}
	bool loadContent(Document * xml_doc);
	const SoulSoulInfoMap_t & getSoulSoulInfo();
protected:
	bool loadInfo(Element * element);
private:
	SoulSoulInfoMap_t m_soul_soul_res_map;
};
#endif
