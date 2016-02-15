#ifndef RING_GUILD_RES_HPP
#define RING_GUILD_RES_HPP
struct RingGuildInfo
{
	RingGuildInfo()
	:id(0)
	,ring_quest_max(0)
	,ring_loop(0)
	{}
	Uint32Vec_t	bonus_id1;
	uint32		id;
	Uint32Vec_t level1;
	uint32		ring_quest_max;
	uint32 ring_loop;
};
#endif
