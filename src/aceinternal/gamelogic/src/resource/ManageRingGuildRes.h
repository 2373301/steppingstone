#ifndef MANAGE_RING_GUILD_RES_HPP
#define MANAGE_RING_GUILD_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RingGuildRes.h"
class GAMELOGIC_EXOPRT ManageRingGuildRes
{
public:
	bool loadContent(Document * xml_doc);
	const RingGuildInfoMap_t & getRingGuildInfo();
protected:
	bool loadInfo(Element * element);
private:
	RingGuildInfoMap_t m_ring_guild_res_map;
};
#endif
