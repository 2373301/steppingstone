#ifndef ESCORT_CAR_RES_HPP
#define ESCORT_CAR_RES_HPP
struct EscortCarInfo
{
	EscortCarInfo()
	: id(0)
	,pay(0)
	,probability(0)
	,reward_money(0)
	,reward_shengwang(0)
	,reward_yueli(0)
	{}
	uint32 id;
	uint32 pay;
	uint32 probability;
	uint32 reward_money;
	uint32 reward_shengwang;
	uint32 reward_yueli;
	uint32 is_max;
};
#endif
