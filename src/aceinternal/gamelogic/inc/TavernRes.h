#ifndef TAVERN__RES_HPP
#define TAVERN__RES_HPP
struct TavernInfo
{
	TavernInfo()
	: level(0)
	,resource1(0)
	,resource1_num(0)
	,resource2(0)
	,resource2_num(0)
	,resource3(0)
	,resource3_num(0)
	,type(0)
	{}
	uint32 level;
	uint32 resource1;
	uint32 resource1_num;
	uint32 resource2;
	uint32 resource2_num;
	uint32 resource3;
	uint32 resource3_num;
	uint32 type;

	static uint32 make_key(uint32 type, uint32 level)
	{
		return 100000 * type + level;
	}
};
#endif
