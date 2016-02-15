#ifndef MATEGATE__RES_HPP
#define MATEGATE__RES_HPP
struct MategateInfo
{
	MategateInfo()
	: blue_love(0)
	, blue_pro1(0)
	, blue_pro2(0)
	, cost(0)
	, free_num(0)
	, green_love(0)
	, green_pro1(0)
	, green_pro2(0)
	, id(0)
	, num(0)
	, orange_love(0)
	, orange_pro1(0)
	, orange_pro2(0)
	, purple_love(0)
	, purple_pro1(0)
	, purple_pro2(0)
	{}
	uint32 blue_love;
	uint32 blue_pro1;
	uint32 blue_pro2;
	uint32 cost;
	uint32 free_num;
	uint32 green_love;
	uint32 green_pro1;
	uint32 green_pro2;
	uint32 id;
	uint32 num;
	uint32 orange_love;
	uint32 orange_pro1;
	uint32 orange_pro2;
	uint32 purple_love;
	uint32 purple_pro1;
	uint32 purple_pro2;
	uint32 must_get;
	uint32 orange_limit;
};
#endif
