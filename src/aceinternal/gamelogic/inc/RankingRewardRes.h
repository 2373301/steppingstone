#ifndef RANKING_REWARD_RES_HPP
#define RANKING_REWARD_RES_HPP
struct RankingRewardInfo
{
	RankingRewardInfo()
	: id(0)
	, rank_level(0)
	{}
	Uint32Vec_t hero_point;
	uint32 id;
	Uint32Vec_t player_level;
	uint32 rank_level;
	Uint32Vec_t rank_rewards;
};
#endif
