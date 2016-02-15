#ifndef SOUL_STAGE_RES_HPP
#define SOUL_STAGE_RES_HPP
struct SoulStageInfo
{
	SoulStageInfo()
	: bonus(0)
	, enemy_id(0)
	, id(0)
	, level(0)
	{}
	uint32 bonus;
	uint32 enemy_id;
	uint32 id;
	uint32 level;
};
#endif
