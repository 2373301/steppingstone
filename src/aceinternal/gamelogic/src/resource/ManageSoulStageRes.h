#ifndef MANAGE_SOUL_STAGE_RES_HPP
#define MANAGE_SOUL_STAGE_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SoulStageRes.h"
class GAMELOGIC_EXOPRT ManageSoulStageRes
{
public:
	bool loadContent(Document * xml_doc);
	const SoulStageInfoMap_t & getSoulStageInfo();
protected:
	bool loadInfo(Element * element);
private:
	SoulStageInfoMap_t m_Soul_Stage_res_map;
};
#endif
