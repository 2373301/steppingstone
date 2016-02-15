#ifndef MAZEEXPLORATION__RES_HPP
#define MAZEEXPLORATION__RES_HPP
struct MazeexplorationInfo
{
	MazeexplorationInfo()
	: explorationid(0)
	, explorationvalue(0)
	, maxexploration(0)
	, resumetime(0)
	, resumevalue(0)
	, roomclearid(0)
	{}
	uint32 explorationid;
	uint32 explorationvalue;
	uint32 maxexploration;
	uint32 resumetime;
	uint32 resumevalue;
	uint32 roomclearid;
};
#endif
