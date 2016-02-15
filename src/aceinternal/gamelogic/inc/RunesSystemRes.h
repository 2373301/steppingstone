#ifndef RUNES_SYSTEM_RES_HPP
#define RUNES_SYSTEM_RES_HPP
struct RunesSystemInfo
{
	RunesSystemInfo()
	: critical_value1(0)
	, critical_value2(0)
	, critical_value3(0)
	, critical_value4(0)
	, runes1(0)
	, runes2(0)
	, runes3(0)
	, runes4(0)
	, runes5(0)
	, runes_stone_exp(0)
	, unlock_stone_num(0)
	{}
	Uint32Vec_t change_rate;
	Uint32Vec_t critical_rate1;
	Uint32Vec_t critical_rate2;
	uint32 critical_value1;
	uint32 critical_value2;
	uint32 critical_value3;
	uint32 critical_value4;
	uint32 runes1;
	uint32 runes2;
	uint32 runes3;
	uint32 runes4;
	uint32 runes5;
	uint32 runes_stone_exp;
	uint32 unlock_stone_num;
};
#endif
