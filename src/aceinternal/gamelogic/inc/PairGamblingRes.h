#ifndef PAIR_GAMBLING_RES_HPP
#define PAIR_GAMBLING_RES_HPP
struct PairGamblingInfo
{
	PairGamblingInfo()
	: lose_rate(0)
	, round(0)
	, subtype(0)
	, type(0)
	, win_rate(0)
	{}
	Uint32Vec_t level;
	uint32 lose_rate;
	Uint32Vec_t money;
	uint32 round;
	uint32 subtype;
	uint32 type;
	uint32 win_rate;
};
#endif
