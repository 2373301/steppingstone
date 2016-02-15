#ifndef MANAGE_SOUL_SUPER_RES_HPP
#define MANAGE_SOUL_SUPER_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SoulSuperRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageSoulSuperRes
{
public:
	~ManageSoulSuperRes()
	{
		cleanMapSecondContent(m_soul_super_res_map);
	}
	bool loadContent(Document * xml_doc);
	const SoulSuperInfoMap_t & getSoulSuperInfo();
protected:
	bool loadInfo(Element * element);
private:
	SoulSuperInfoMap_t m_soul_super_res_map;
};
#endif
