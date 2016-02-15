#ifndef RUNES_CLASS_RES_HPP
#define RUNES_CLASS_RES_HPP
struct RunesClassInfo
{
	RunesClassInfo()
	: classs(0)
	, max_level(0)
	, money(0)
	, need_runes_fire(0)
	, player_level(0)
	{}
	uint32 classs;
	uint32 max_level;
	uint32 money;
	uint32 need_runes_fire;
	uint32 player_level;
};
#endif
