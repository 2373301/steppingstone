#ifndef SECTION_BONUS_RES_HPP
#define SECTION_BONUS_RES_HPP
struct SectionBonusInfo
{
	SectionBonusInfo()
	: bonus1(0)
	, bonus2(0)
	, end_date(0)
	, id(0)
	, star_date(0)
	{}
	uint32 bonus1;
	uint32 bonus2;
	uint32 end_date;
	uint32 id;
	uint32 star_date;
};
#endif
