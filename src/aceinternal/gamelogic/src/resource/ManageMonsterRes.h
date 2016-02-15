
#ifndef MANAGE_MONSTER_RES_HPP
#define MANAGE_MONSTER_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"

class GAMELOGIC_EXOPRT ManageMonsterRes 
{
public:
	~ManageMonsterRes();
public:
	bool loadMonsterContent(Document * xml_doc);

	const MonsterResInfoMap_t & getMonsterInfoMap();
protected:
	bool loadMonsterInfo(Element * monster_element);

private:

	MonsterResInfoMap_t m_monster_info_map;
};
#endif