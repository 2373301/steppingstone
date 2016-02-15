#ifndef MATECOLOR__RES_HPP
#define MATECOLOR__RES_HPP
struct MatecolorInfo
{
	MatecolorInfo()
	: blue_exp(0)
	, green_exp(0)
	, lv(0)
	, orange_exp(0)
	, purple_exp(0)
	{}
	uint32 blue_exp;
	uint32 green_exp;
	uint32 lv;
	uint32 orange_exp;
	uint32 purple_exp;
};
#endif
