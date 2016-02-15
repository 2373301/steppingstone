
#ifndef MANAGE_NPC_RES_HPP
#define MANAGE_NPC_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"

class GAMELOGIC_EXOPRT ManageNpcRes
{
public:
	~ManageNpcRes();
public:
	bool loadNpcContent(Document * xml_doc);

	const NpcResInfoMap_t & getNpcInfoMap();
protected:
	bool loadNpcInfo(Element * npc_element);
private:
	NpcResInfoMap_t m_npc_res_info_map;
};
#endif