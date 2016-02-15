#ifndef TOWER_STAGE_RES_HPP
#define TOWER_STAGE_RES_HPP
struct TowerStageInfo
{
	TowerStageInfo()
	: bonus(0)
	,enemy(0)
	,exp(0)
	,level_group(0)
	,move_group(0)
	{}
	uint32 bonus;
	uint32 enemy;
	uint32 exp;
	uint32 level_group;
	uint32 move_group;
	uint32 floor;
};
#endif
