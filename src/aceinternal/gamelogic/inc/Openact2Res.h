#ifndef OPENACT2__RES_HPP
#define OPENACT2__RES_HPP
struct Openact2Info
{
	Openact2Info()
	: award_day(0)
	, bonus(0)
	, id(0)
	, start_day(0)
	, target1(0)
	, target2(0)
	, target3(0)
	{}
	uint32 award_day;
	uint32 bonus;
	uint32 id;
	Uint32Vec_t results_day;
	uint32 start_day;
	uint32 target1;
	uint32 target2;
	uint32 target3;
};
#endif
