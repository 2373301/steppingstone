#ifndef TOWER_REWARDS_RES_HPP
#define TOWER_REWARDS_RES_HPP
struct TowerRewardsInfo
{
	TowerRewardsInfo()
	: bonus1(0)
	,bonus2(0)
	,bonus3(0)
	,bonus4(0)
	,bonus5(0)
	,exp1(0)
	,exp2(0)
	,level_group(0)
	{}
	uint32 bonus1;
	uint32 bonus2;
	uint32 bonus3;
	uint32 bonus4;
	uint32 bonus5;
	uint32 exp1;
	uint32 exp2;
	uint32 level_group;
};
#endif
