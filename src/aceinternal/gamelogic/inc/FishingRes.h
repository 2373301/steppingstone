#ifndef FISHING__RES_HPP
#define FISHING__RES_HPP
struct FishingInfo
{
	FishingInfo()
	:id(0)
	,time(0)
	{}
	Uint32Vec_t bonus;
	uint32 id;
	Uint32Vec_t level;
	uint32 time;
};
#endif
