#ifndef GUILD_RES_H_
#define GUILD_RES_H_

struct Guild_Nity
{
	Guild_Nity()
		: level(0)
		, player_num(0)
		, exp(0)
		, vec_systemunlock(0)
		, impeach_condition(0)
		, impeach_cost(0)
		, impeach_costnum(0)
		, impeach_time(0)
		, impeach_playernum(0)
		, impeach_gold(0)
	{}
	uint32 level;
	uint32 player_num;
	uint32 exp;
	Uint32Vec_t vec_systemunlock;
	Uint32Vec_t vec_pic;
	uint32 impeach_condition;
	uint32 impeach_cost;
	uint32 impeach_costnum;
	uint32 impeach_time;
	uint32 impeach_playernum;
	uint32 impeach_gold;

};

#endif