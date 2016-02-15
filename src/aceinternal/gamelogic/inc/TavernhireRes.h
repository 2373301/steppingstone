#ifndef TAVERNHIRE__RES_HPP
#define TAVERNHIRE__RES_HPP
struct TavernhireInfo
{
	TavernhireInfo()
	: level(0)
	,resource_num(0)
	,type(0)
	{}
	uint32 level;
	uint32 resource_num;
	Uint32Vec_t spe_probability;
	uint32 type;
	Uint32Vec_t spe_hire;

	static uint32 make_key(uint32 type, uint32 level)
	{
		return 100000 * type + level;
	}
};
#endif
