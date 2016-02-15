#ifndef HORSEMAN_LEVELUP_RES_HPP
#define HORSEMAN_LEVELUP_RES_HPP
struct HorsemanLevelupInfo
{
	HorsemanLevelupInfo()
	: riding_exp(0)
	, riding_lv(0)
	{}
	uint32 riding_exp;
	uint32 riding_lv;
};
#endif
