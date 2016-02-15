#ifndef PAIR_REWARDS_RES_HPP
#define PAIR_REWARDS_RES_HPP
struct PairRewardsInfo
{
	PairRewardsInfo()
	: level(0)
	, reward(0)
	, rewardtype(0)
	, servers(0)
	{}
	Uint32Vec_t bonusid;
	uint32 level;
	uint32 reward;
	uint32 rewardtype;
	uint32 servers;
};
#endif
