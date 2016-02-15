#ifndef TALENT__RES_HPP
#define TALENT__RES_HPP
struct TalentInfo
{
	TalentInfo()
	: gold_rate1(0)
	, gold_rate2(0)
	, group_id(0)
	, id(0)
	, is_show(0)
	, item_rate1(0)
	, item_rate2(0)
	, skill_id(0)
	{}
	uint32 gold_rate1;
	uint32 gold_rate2;
	uint32 group_id;
	uint32 id;
	uint32 is_show;
	uint32 item_rate1;
	uint32 item_rate2;
	uint32 skill_id;
};
#endif
