#ifndef MATEUP__RES_HPP
#define MATEUP__RES_HPP
struct MateupInfo
{
	MateupInfo()
	: cost(0)
	, crit_pro(0)
	, exp(0)
	, id(0)
	{}
	uint32 cost;
	uint32 crit_pro;
	uint32 exp;
	uint32 id;
	uint32 pay;
};
#endif
