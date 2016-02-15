#ifndef LUCKY_REWARDS_RES_HPP
#define LUCKY_REWARDS_RES_HPP
struct LuckyRewardsInfo
{
	LuckyRewardsInfo()
	: id(0)
	, light(0)
	, num(0)
	, percent_gold(0)
	, percent_item(0)
	, percent_num(0)
	, spectial_icon(0)
	, spectial_tips(0)
	, talk(0)
	, type(0)
	, value(0)
    ,single_pos_gold(0)
    ,single_pos_item(0)
    ,series_pos_gold(0)
    ,series_pos_item(0)
    ,condition(0)
	{}
	uint32 id;
	uint32 light;
	uint32 num;
	uint32 percent_gold;
	uint32 percent_item;
	uint32 percent_num;
	uint32 spectial_icon;
	uint32 spectial_tips;
	uint32 talk;
	uint32 type;
	uint32 value;

    // 抽奖专用
    // 单抽
    uint32 single_pos_gold;
    uint32 single_pos_item;

    // 连续
    uint32 series_pos_gold;
    uint32 series_pos_item;

    uint32 condition;
};
#endif
