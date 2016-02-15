#ifndef BATTLEGROUND_BUFF_RES_HPP
#define BATTLEGROUND_BUFF_RES_HPP
struct BattlegroundBuffInfo
{
	BattlegroundBuffInfo()
	: type(0)
	,rate(0)
	{}
	Uint32Vec_t cost;
	Uint32Vec_t probability;
	uint32 type;
	uint32 rate;
};
#endif
