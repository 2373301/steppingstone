#ifndef PLUM__RES_HPP
#define PLUM__RES_HPP
struct PlumInfo
{
	PlumInfo()
	: capacity_silver(0)
	, capacity_gold(0)
	, guarantees(0)
	, id(0)
	{}
	uint32 capacity_silver;
	uint32 capacity_gold;
	uint32 guarantees;
	uint32 id;
};
#endif
