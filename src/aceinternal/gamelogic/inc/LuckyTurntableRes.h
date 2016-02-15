#ifndef LUCKY_TURNTABLE_RES_HPP
#define LUCKY_TURNTABLE_RES_HPP
struct LuckyTurntableInfo
{
	LuckyTurntableInfo()
	: gold_percent(0)
	, initial_fund(0)
	, lucky_item(0)
	, need_gold(0)
	, num(0)
    ,fund_max(0)
    ,fund_min(0)
    ,discount(0)
    ,accum_num(0)
    ,accum_id(0)
	{}

	Uint32Vec_t end_time;
	uint32 gold_percent;
	uint32 initial_fund;
	uint32 lucky_item;
	uint32 need_gold;
	uint32 num;
	Uint32Vec_t start_time;
    uint32 fund_max;
    uint32 fund_min;

    uint32 discount;
    uint32 accum_num;
    uint32 accum_id;
};
#endif
