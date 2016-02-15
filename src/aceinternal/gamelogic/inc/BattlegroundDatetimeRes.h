#ifndef BATTLEGROUND_DATETIME_RES_HPP
#define BATTLEGROUND_DATETIME_RES_HPP
struct BattlegroundDatetimeInfo
{
	BattlegroundDatetimeInfo()
	: end_time(0)
	, id(0)
	, level(0)
	, ready_time(0)
	, round_time(0)
	, start_time(0)
	, cd1(0)
	, cd2(0)
	, cd3(0)
	, cd4(0)
	{}
	uint32 end_time;
	uint32 id;
	Uint32Vec_t level;
	uint32 ready_time;
	uint32 round_time;
	uint32 start_time;
	
	uint32 cd1;
	uint32 cd2;
	uint32 cd3;
	uint32 cd4;
};
#endif
