#ifndef MANAGE_TAVERNSPEBILITY__RES_HPP
#define MANAGE_TAVERNSPEBILITY__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TavernspebilityRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageTavernspebilityRes
{
public:
	~ManageTavernspebilityRes()
	{
		cleanMapSecondContent(m_tavernspebility__res_map);
	}
	bool loadContent(Document * xml_doc);
	const TavernspebilityInfoMap_t & getTavernspebilityInfo();
protected:
	bool loadInfo(Element * element);
private:
	TavernspebilityInfoMap_t m_tavernspebility__res_map;
};
#endif
