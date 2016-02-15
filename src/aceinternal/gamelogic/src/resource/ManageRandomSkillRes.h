
#ifndef MANAGE_RANDOM_SKILL_RES_HPP
#define MANAGE_RANDOM_SKILL_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageRandomSkillRes
{
public:
	~ManageRandomSkillRes()
	{
		cleanMapSecondContent(m_randomskill_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const RandomSkillResInfoMap_t & getRandomSkillInfoMap();
protected:
	bool loadInfo(Element * map_element);

private:

	RandomSkillResInfoMap_t m_randomskill_info_map;
};

#endif