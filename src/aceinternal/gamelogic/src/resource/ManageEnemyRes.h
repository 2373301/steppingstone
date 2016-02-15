
#ifndef MANAGE_ENEMY_RES_HPP
#define MANAGE_ENEMY_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"

class GAMELOGIC_EXOPRT ManageEnemyRes
{
public:
	~ManageEnemyRes();

public:
	bool loadEnemyContent(Document * xml_doc);

	const EnemyResInfoMap_t & getEnemyInfo();
protected:
	bool loadEnemyInfo(Element * enemy_element);
private:
	EnemyResInfoMap_t m_enemy_info_map;
};

#endif