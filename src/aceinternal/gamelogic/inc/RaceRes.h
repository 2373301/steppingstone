#ifndef RACE__RES_HPP
#define RACE__RES_HPP
struct RaceInfo
{
	RaceInfo()
	: fail_bonus_id(0)
	, level(0)
	, type(0)
	, victory_bonus_id(0)
	{}
	uint32 fail_bonus_id;
	uint32 level;
	uint32 type;
	uint32 victory_bonus_id;
};
#endif
