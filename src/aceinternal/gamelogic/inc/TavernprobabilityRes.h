#ifndef TAVERNPROBABILITY__RES_HPP
#define TAVERNPROBABILITY__RES_HPP

struct TavernprobabilityInfo
{
	TavernprobabilityInfo()
	: probability2(0)
	,spe_probability2(0)
	,type(0)
	,level(0)
	, max_random_weight(0)
	, max_change_weight(0)
	, minimum(0)
	, judge(0)
	{}

Uint32Vec_t hire_id;
Uint32Vec_t probability1;
	uint32 probability2;
Uint32Vec_t spe_probability1;
	uint32 spe_probability2;
	uint32 type;
	uint32 level;
	uint32 max_random_weight;
	uint32 max_change_weight;
	uint32 card_probability1;
	uint32 card_probability2;
	uint32 minimum;
	uint32 judge;

	static uint32 make_key(uint32 type, uint32 level)
	{
		return 100000 * type + level;
	}
};
#endif
