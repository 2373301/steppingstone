#ifndef MAZE__RES_HPP
#define MAZE__RES_HPP
struct MazeInfo
{
	MazeInfo()
//	: bosstarget(0)
	: entrance(0)
	, id(0)
	, item(0)
	, level(0)
	, lowtrip(0)
	, maxrewards(0)
	, opemtime1(0)
	, opemtime2(0)
	, opentimes(0)
	, periodtime("")
	, subtype(0)
	, target(0)
	, type(0)
	, bossroom(0)
	{}
//	uint32 bosstarget;
	uint32 entrance;
	uint32 id;
	uint32 item;
	uint32 level;
	uint32 lowtrip;
	uint32 maxrewards;
	uint32 opemtime1;
	uint32 opemtime2;
	uint32 opentimes;
	std::string periodtime;
	uint32 subtype;
	uint32 target;
	uint32 type;
	uint32 goldblessmeid;
	uint32 bossroom;
};
#endif
