#ifndef FUTURE_BONUS_RES_HPP
#define FUTURE_BONUS_RES_HPP
struct FutureBonusInfo
{
	FutureBonusInfo()
	: bonus1(0)
	, bonus10(0)
	, bonus2(0)
	, bonus3(0)
	, bonus4(0)
	, bonus5(0)
	, bonus6(0)
	, bonus7(0)
	, bonus8(0)
	, bonus9(0)
	, end_date(0)
	, id(0)
	, principal(0)
	, star_date(0)
	{}
	uint32 bonus1;
	uint32 bonus10;
	uint32 bonus2;
	uint32 bonus3;
	uint32 bonus4;
	uint32 bonus5;
	uint32 bonus6;
	uint32 bonus7;
	uint32 bonus8;
	uint32 bonus9;
	uint32 end_date;
	uint32 id;
	uint32 principal;
	uint32 star_date;
};
#endif
