#ifndef MAZEREWARDS__RES_HPP
#define MAZEREWARDS__RES_HPP
struct MazerewardsInfo
{
	MazerewardsInfo()
	: event(0)
	, mazeid(0)
	, num(0)
	, percent(0)
	, roomid(0)
	, type(0)
	, value(0)
	, minLevel(0)
	, maxLevel(0)
	{}
	uint32 event;
	uint32 mazeid;
	uint32 num;
	uint32 percent;
	uint32 roomid;
	uint32 type;
	uint32 value;
	uint32 group;
	uint32 minLevel;
	uint32 maxLevel;
};
#endif
