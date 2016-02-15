#ifndef JIAN_ZHONG_RES_HPP
#define JIAN_ZHONG_RES_HPP
struct JianZhongInfo
{
	JianZhongInfo()
	: battle_buy(0)
	, battle_cd(0)
	, battle_num(0)
	, defend(0)
	, fail_gain(0)
	, gain(0)
	, gain_cd(0)
	, id(0)
	, in_level(0)
	, last_time(0)
	, place(0)
	, safe_cd(0)
	, win_gain(0)
	{}
	uint32 battle_buy;
	uint32 battle_cd;
	uint32 battle_num;
	uint32 defend;
	uint32 fail_gain;
	uint32 gain;
	uint32 gain_cd;
	uint32 id;
	uint32 in_level;
	uint32 last_time;
	uint32 place;
	uint32 safe_cd;
	Uint32Vec_t star_time;
	uint32 win_gain;
};
#endif
