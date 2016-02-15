#ifndef OPENACT1__RES_HPP
#define OPENACT1__RES_HPP
struct Openact1Info
{
	Openact1Info()
	: end_day(0)
	, gap(0)
	, id(0)
	, start_day(0)
	, target(0)
	, target_num(0)
	{}
	uint32 end_day;
	uint32 gap;
	uint32 id;
	uint32 start_day;
	uint32 target;
	uint32 target_num;
	Uint32Vec_t vip;
	Uint32Vec_t bouns;
};
#endif
