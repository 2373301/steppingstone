#ifndef GEMSYNTHESIS__RES_HPP
#define GEMSYNTHESIS__RES_HPP
struct GemsynthesisInfo
{
	GemsynthesisInfo()
	: cost(0)
	, id(0)
	, needitem(0)
	, neednum(0)
	{}
	uint32 cost;
	uint32 id;
	uint32 needitem;
	uint32 neednum;
};
#endif
