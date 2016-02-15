
#ifndef MANAGE_STAGE1ST_RES_HPP
#define MANAGE_STAGE1ST_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageStage1stRes
{
public:
	~ManageStage1stRes()
	{
		cleanMapSecondContent(m_stage1st_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const Stage1stResInfoMap_t & getStage1stInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	Stage1stResInfoMap_t m_stage1st_info_map;
};

#endif