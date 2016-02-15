#ifndef RING_QUEST_RES_HPP
#define RING_QUEST_RES_HPP
struct RingQuestInfo
{
	RingQuestInfo()
	: id(0)
	,lv(0)
	{}
	uint32 id;
	uint32 lv;
Uint32Vec_t questid1;
Uint32Vec_t questid2;
};
#endif
