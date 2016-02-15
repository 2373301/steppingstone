
#ifndef MANAGE_SKILL_RES_HPP
#define MANAGE_SKILL_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"

class GAMELOGIC_EXOPRT ManageSkillRes
{
public:
	~ManageSkillRes();
public:
	bool loadSkillContent(Document * xml_doc);

	const SkillResInfoMap_t & getSkillInfoMap();
protected:
	bool loadSkillInfo(Element * skill_element);
private:
	SkillResInfoMap_t m_skill_info_map;
};
#endif