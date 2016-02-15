#ifndef SUIT__RES_HPP
#define SUIT__RES_HPP
struct SuitInfo
{
	SuitInfo()
	: group(0)
	, id(0)
	, num(0)
	, property_id(0)
	, value(0)
	{}
	uint32 group;
	uint32 id;
	uint32 num;
	Uint32Vec_t property_id;
	Int32Vec_t value;

	static uint32 make_key(uint32 group, uint32 num)
	{
		return 100000 * group + num;
	}
};
#endif
