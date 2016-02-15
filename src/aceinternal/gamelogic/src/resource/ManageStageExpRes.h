#ifndef MANAGE_STAGE_EXP_RES_HPP
#define MANAGE_STAGE_EXP_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "StageExpRes.h"
class GAMELOGIC_EXOPRT ManageStageExpRes
{
public:
	bool loadContent(Document * xml_doc);
	const StageExpInfoMap_t & getStageExpInfo();
protected:
	bool loadInfo(Element * element);
private:
	StageExpInfoMap_t m_Stage_Exp_res_map;
};
#endif
