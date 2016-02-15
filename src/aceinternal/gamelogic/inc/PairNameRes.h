#ifndef PAIR_NAME_RES_HPP
#define PAIR_NAME_RES_HPP
struct PairNameInfo
{
	PairNameInfo()
	: colour(0)
	, get_way1(0)
	, get_way2(0)
	, get_way3(0)
	, get_way4(0)
	, id(0)
	, level(0)
//	, name(0)
	, talk(0)
	, value1(0)
	, value2(0)
	, value3(0)
	, value4(0)
	{}
	uint32 colour;
	uint32 get_way1;
	uint32 get_way2;
	uint32 get_way3;
	uint32 get_way4;
	uint32 id;
	uint32 level;
	string name;
	uint32 talk;
	uint32 value1;
	uint32 value2;
	uint32 value3;
	uint32 value4;
};
#endif
