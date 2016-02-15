#ifndef TALENTGROUP__RES_HPP
#define TALENTGROUP__RES_HPP
struct TalentgroupInfo
{
	TalentgroupInfo()
	: gold_rate1(0)
	, group_id(0)
	, id(0)
	, item_rate1(0)
	{}
	uint32 gold_rate1;
	uint32 group_id;
	uint32 id;
	uint32 item_rate1;
};
#endif
