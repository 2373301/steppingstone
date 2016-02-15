#ifndef FAIL_SCORE_RES_HPP
#define FAIL_SCORE_RES_HPP
struct FailScoreInfo
{
	FailScoreInfo()
	: id(0)
	, star1(0)
	, star2(0)
	, star3(0)
	, star4(0)
	, star5(0)
	{}
	uint32 id;
	uint32 star1;
	uint32 star2;
	uint32 star3;
	uint32 star4;
	uint32 star5;
};
#endif
