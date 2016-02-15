#ifndef MANAGE_RUNES_LEVEL_RES_HPP
#define MANAGE_RUNES_LEVEL_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RunesLevelRes.h"
#include "share_fun.h"
class GAMELOGIC_EXOPRT ManageRunesLevelRes
{
public:
	~ManageRunesLevelRes()
	{
		cleanMapSecondContent(m_runes_level_res_map);
	}
	bool loadContent(Document * xml_doc);
	const RunesLevelInfoMap_t & getRunesLevelInfo();
protected:
	bool loadInfo(Element * element);
private:
	RunesLevelInfoMap_t m_runes_level_res_map;
};
#endif
