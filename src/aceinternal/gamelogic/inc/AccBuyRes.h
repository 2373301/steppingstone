#ifndef ACC_BUY_RES_HPP
#define ACC_BUY_RES_HPP
struct AccBuyInfo
{
	AccBuyInfo()
	: bonus(0)
	, end_date(0)
	, id(0)
	, num(0)
	, star_date(0)
	{}
	uint32 bonus;
	uint32 end_date;
	uint32 id;
	uint32 num;
	uint32 star_date;
};
#endif
