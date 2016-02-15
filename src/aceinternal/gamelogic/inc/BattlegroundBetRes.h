#ifndef BATTLEGROUND_BET_RES_HPP
#define BATTLEGROUND_BET_RES_HPP
struct BattlegroundBetInfo
{
	BattlegroundBetInfo()
	: bet_a1(0)
	, bet_a2(0)
	, bet_a3(0)
	, faction_id(0)
	, id(0)
	{}
	uint32 bet_a1;
	uint32 bet_a2;
	uint32 bet_a3;
	uint32 faction_id;
	uint32 id;
	Uint32Vec_t level_a1;
};
#endif
