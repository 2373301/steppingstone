#ifndef RING_RANDOM_RES_HPP
#define RING_RANDOM_RES_HPP
struct RingRandomInfo
{
	RingRandomInfo()
	:id(0)
	,lv(0)
	{}
	Uint32Vec_t bonus_id;
	Uint32Vec_t guil_bonus;
	uint32 id;
	uint32 lv;
};
#endif
