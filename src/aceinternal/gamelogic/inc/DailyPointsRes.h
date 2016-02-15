#ifndef DAILYPOINTS_RES_H
#define DAILYPOINTS_RES_H

struct DailyPointsEvents
{
	DailyPointsEvents()
		: id(0)
		, event_id(0)
		, num(0)
		, min_lev(0)
		, max_lev(0)
		, point(0)
	{}

	uint32 id;
	uint32 event_id;
	uint32 num;
	uint32 min_lev;
	uint32 max_lev;
	uint32 point;

};

struct DailyPointsReward
{
	DailyPointsReward()
		: id(0)
		, lev(0)
	{}

	uint32 id;
	uint32 lev;
	Uint32Vec_t vec_point;
	Uint32Vec_t vec_bonus_id;
	Uint32Vec_t vec_guild_exp;
};

#endif