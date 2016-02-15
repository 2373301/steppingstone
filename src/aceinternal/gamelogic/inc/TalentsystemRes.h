#ifndef TALENTSYSTEM__RES_HPP
#define TALENTSYSTEM__RES_HPP
struct TalentsystemInfo
{
	TalentsystemInfo()
	: need_gold1(0)
	, need_gold2(0)
	, need_item1(0)
	, need_item2(0)
	{}
	Uint32Vec_t level;
	Uint32Vec_t need_color;
	uint32 need_gold1;
	uint32 need_gold2;
	uint32 need_item1;
	uint32 need_item2;
	Uint32Vec_t unlock_gold;
};
#endif
