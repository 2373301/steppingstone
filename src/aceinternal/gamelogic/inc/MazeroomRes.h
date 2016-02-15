#ifndef MAZEROOM__RES_HPP
#define MAZEROOM__RES_HPP
struct MazeroomInfo
{
	MazeroomInfo()
	: id(0)
	, isallopen(0)
	, mapid(0)
	, maxtrip(0)
	, mazeid(0)
	, opentype(0)
	, tripcannotseemax(0)
	, tripcannotseemin(0)
	, tripcanseemax(0)
	, tripcanseemin(0)
	, unlock(0)
	, value1(0)
	, value2(0)
	{}
	uint32 id;
	uint32 isallopen;
	uint32 mapid;
	uint32 maxtrip;
	uint32 mazeid;
	uint32 opentype;
	uint32 tripcannotseemax;
	uint32 tripcannotseemin;
	uint32 tripcanseemax;
	uint32 tripcanseemin;
	uint32 unlock;
	uint32 value1;
	uint32 value2;
};
#endif
