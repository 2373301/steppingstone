#ifndef NEXTDAY_INTEREST_RES_HPP
#define NEXTDAY_INTEREST_RES_HPP
struct NextdayInterestInfo
{
	NextdayInterestInfo()
	: back_max(0)
	, back_rate(0)
	, end_date(0)
	, id(0)
	, range(0)
	, star_date(0)
	{}
	uint32 back_max;
	uint32 back_rate;
	uint32 end_date;
	uint32 id;
	uint32 range;
	uint32 star_date;
};
#endif
