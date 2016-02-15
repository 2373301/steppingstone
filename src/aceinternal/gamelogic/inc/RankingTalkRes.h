#ifndef RANKING_TALK_RES_HPP
#define RANKING_TALK_RES_HPP
struct RankingTalkInfo
{
	RankingTalkInfo()
	: id(0)
	{}
	uint32 id;
	Uint32Vec_t value;
};
#endif
