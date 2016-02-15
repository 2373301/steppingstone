#ifndef GEMGROUP__RES_HPP
#define GEMGROUP__RES_HPP
struct GemgroupInfo
{
	GemgroupInfo()
	: group(0)
	, num(0)
	, part(0)
	{}
	uint32 group;
	uint32 num;
	uint32 part;
};
#endif
