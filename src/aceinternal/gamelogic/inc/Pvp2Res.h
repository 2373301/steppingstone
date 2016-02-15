#ifndef PVP_2_RES_HPP
#define PVP_2_RES_HPP
struct Pvp2Info
{
	Pvp2Info()
	: battletimes(0)
	, defeatedtimes1(0)
	, defeatedtimes2(0)
	, joinlevel(0)
	, jointime1(0)
	, jointime2(0)
	, part1begintime(0)
	, part1num(0)
	, part1resttime(0)
	, part1rundtime(0)
	, part2begintime1(0)
	, part2begintime2(0)
	, part2begintime3(0)
	, part2begintime4(0)
	, part2begintime5(0)
	, part2rundtime(0)
	, start_time(0)
	{}
	uint32 battletimes;
	uint32 defeatedtimes1;
	uint32 defeatedtimes2;
	uint32 joinlevel;
	uint32 jointime1;
	uint32 jointime2;
	uint32 part1begintime;
	uint32 part1num;
	uint32 part1resttime;
	uint32 part1rundtime;
	uint32 part2begintime1;
	uint32 part2begintime2;
	uint32 part2begintime3;
	uint32 part2begintime4;
	uint32 part2begintime5;
	uint32 part2rundtime;
	uint32 start_time;
};
#endif
