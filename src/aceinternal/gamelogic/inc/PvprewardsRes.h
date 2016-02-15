#ifndef PVPREWARDS__RES_HPP
#define PVPREWARDS__RES_HPP
struct PvprewardsInfo
{
	PvprewardsInfo()
	: level(0)
	, reward(0)
	, servers(0)
	, rewardtype(0)
	{}
	Uint32Vec_t bonusid;
	uint32 level;
	uint32 reward;
	uint32 servers;
	uint32 rewardtype;
};
#endif
