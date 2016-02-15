
#ifndef MANAGE_LEVEL_EXP_RES_HPP
#define MANAGE_LEVEL_EXP_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageLevelExpRes
{
public:
	~ManageLevelExpRes()
	{
		cleanMapSecondContent(m_level_exp_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const LevelExpResInfoMap_t & getLevelExpInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	LevelExpResInfoMap_t m_level_exp_info_map;
};

#endif