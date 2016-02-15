#ifndef OFFLINEHANGUP__RES_HPP
#define OFFLINEHANGUP__RES_HPP
struct OfflinehangupInfo
{
	OfflinehangupInfo()
	: a(0)
	,b(0)
	,time_v(0)
	, maxtime(0)
	{}
	uint32 a;
	uint32 b;
	uint32 time_v;
	uint32 maxtime;
};
#endif
