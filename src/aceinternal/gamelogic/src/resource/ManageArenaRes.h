
#ifndef MANAGE_ARENA_RES_PP
#define MANAGE_ARENA_RES_PP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageArenaRes
{
public:
	~ManageArenaRes()
	{
		cleanVectorContent(m_arena_info_vec);
	}
public:
	bool loadContent(Document * xml_doc);

	const ArenaResInfoVec_t & getArenaInfoVec();
protected:
	bool loadInfo(Element * element);

private:

	ArenaResInfoVec_t m_arena_info_vec;
};

#endif