
#ifndef MANAGE_SWEEP_STAGE_RES_INFO_HPP
#define MANAGE_SWEEP_STAGE_RES_INFO_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SweepStageResInfo.h"

class GAMELOGIC_EXOPRT ManageSweepStageResInfo
{
public:
	bool loadContent(Document * xml_doc);

	const SweepStageResInfo & getSweepStageResInfo();
protected:
	bool loadInfo(Element * element);

private:

	SweepStageResInfo m_sweep_stage_res_info;
};

#endif