#ifndef MANAGE_RING_QUEST_RES_HPP
#define MANAGE_RING_QUEST_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RingQuestRes.h"
class GAMELOGIC_EXOPRT ManageRingQuestRes
{
public:
	bool loadContent(Document * xml_doc);
	const RingQuestInfoVec_t & getRingQuestInfo();
protected:
	bool loadInfo(Element * element);
private:
	RingQuestInfoVec_t m_ring_quest_res_vec;
};
#endif
