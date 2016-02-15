#ifndef BATTLEGROUND_REWARD_RES_HPP
#define BATTLEGROUND_REWARD_RES_HPP
struct BattlegroundRewardInfo
{
	BattlegroundRewardInfo()
	: bonus_id(0)
	, faction_id(0)
	, frame_a1(0)
	, frame_a2(0)
	, frame_a3(0)
	, frame_a4(0)
	, id(0)
	, level(0)
	, money_b1(0)
	, money_b2(0)
	, money_b3(0)
	, money_b4(0)
	, weiwang_c1(0)
	, weiwang_c2(0)
	, weiwang_c3(0)
	, weiwang_c4(0)
	{}
	uint32 bonus_id;
	uint32 faction_id;
	uint32 frame_a1;
	uint32 frame_a2;
	Uint32Vec_t frame_a3;
	uint32 frame_a4;
	uint32 id;
	Uint32Vec_t level;
	uint32 money_b1;
	uint32 money_b2;
	Uint32Vec_t money_b3;
	uint32 money_b4;
	uint32 weiwang_c1;
	uint32 weiwang_c2;
	Uint32Vec_t weiwang_c3;
	uint32 weiwang_c4;
    Uint32Vec_t frame_limit;
    Uint32Vec_t money_limit;
    Uint32Vec_t weiwang_limit;

};
#endif
