#ifndef PAIR_SYSTEM_RES_HPP
#define PAIR_SYSTEM_RES_HPP
struct PairSystemInfo
{
	PairSystemInfo()
	: battle_num(0)
	, battletimes(0)
	, benfu_next_reward(0)
	, cd_time(0)
	, defeatedtimes1(0)
	, defeatedtimes2(0)
	, di_begintime(0)
	, initial_name(0)
	, join_max(0)
	, jointime1(0)
	, jointime2(0)
	, kuafu_begintime(0)
	, kuafu_next_reward(0)
	, leader_chenghao(0)
	, part1begintime(0)
	, part1num(0)
	, part1rundtime(0)
	, part2begintime1(0)
	, part2begintime2(0)
	, part2begintime3(0)
	, part2begintime4(0)
	, part2rundtime(0)
	, partner_chenghao(0)
	, _return(0)
	, team_level(0)
	, team_money(0)
	, name_min(0)
	, name_max(0)
	{}
	uint32 battle_num;
	uint32 battletimes;
	Uint32Vec_t benfu_first_reward1;
	Uint32Vec_t benfu_first_reward2;
	Uint32Vec_t benfu_first_reward3;
	uint32 benfu_next_reward;
	uint32 cd_time;
	uint32 defeatedtimes1;
	uint32 defeatedtimes2;
	uint32 di_begintime;
	uint32 initial_name;
	uint32 join_max;
	uint32 jointime1;
	uint32 jointime2;
	uint32 kuafu_begintime;
	Uint32Vec_t kuafu_first_reward1;
	Uint32Vec_t kuafu_first_reward2;
	Uint32Vec_t kuafu_first_reward3;
	uint32 kuafu_next_reward;
	uint32 leader_chenghao;
	uint32 part1begintime;
	uint32 part1num;
	uint32 part1rundtime;
	uint32 part2begintime1;
	uint32 part2begintime2;
	uint32 part2begintime3;
	uint32 part2begintime4;
	uint32 part2rundtime;
	uint32 partner_chenghao;
	uint32 _return;
	uint32 team_level;
	uint32 team_money;
	uint32 name_min;
	uint32 name_max;
};
#endif
