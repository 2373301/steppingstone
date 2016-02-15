#ifndef DODGE__RES_HPP
#define DODGE__RES_HPP
struct DodgeInfo
{
	DodgeInfo()
	: group(0)
	, id(0)
	, item_unlock(0)
	, level_unlock(0)
	, need_dodge_num(0)
	//, need_dodgemini_group1(0)
	//, need_dodgemini_group2(0)
	//, need_dodgemini_group3(0)
	//, need_dodgemini_group4(0)
	//, need_dodgemini_group5(0)
	//, need_dodgemini_group6(0)
	//, need_level1(0)
	//, need_level2(0)
	//, need_level3(0)
	//, need_level4(0)
	//, need_level5(0)
	//, need_level6(0)
	, skill_id1(0)
	, skill_id2(0)
	, skill_level(0)
	, talk(0)
	, max(0)
	, need_level(0)
	, need_id(0)
	, first_strike(0)
	, level(0)
//	, _type(0)
//	, isjar(0)
	{}
	uint32 group;
	uint32 id;
	uint32 item_unlock;
	uint32 level_unlock;
	uint32 need_dodge_num;
	//uint32 need_dodgemini_group1;
	//uint32 need_dodgemini_group2;
	//uint32 need_dodgemini_group3;
	//uint32 need_dodgemini_group4;
	//uint32 need_dodgemini_group5;
	//uint32 need_dodgemini_group6;
	//uint32 need_level1;
	//uint32 need_level2;
	//uint32 need_level3;
	//uint32 need_level4;
	//uint32 need_level5;
	//uint32 need_level6;
	Uint32Vec_t need_part_group;
	Uint32Vec_t need_part_level;
	uint32 skill_id1;
	uint32 skill_id2;
	uint32 skill_level;
	uint32 talk;
	uint32 max;
	uint32 need_level;
	uint32 need_id;
	uint32 first_strike;
	uint32 _type;
	uint32 level;
//	uint32 isjar;
};
#endif
