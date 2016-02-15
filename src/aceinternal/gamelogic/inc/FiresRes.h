#ifndef FIRES__RES_HPP
#define FIRES__RES_HPP
struct FiresInfo
{
	FiresInfo()
	: critical_rate1(0)
	, critical_rate2(0)
	, critical_rate3(0)
	, critical_rate4(0)
	, critical_value1(0)
	, critical_value2(0)
	, critical_value3(0)
	, critical_value4(0)
	, patch_exp(0)
	, time(0)
	{}
	Uint32Vec_t copy;
	uint32 critical_rate1;
	uint32 critical_rate2;
	uint32 critical_rate3;
	uint32 critical_rate4;
	uint32 critical_value1;
	uint32 critical_value2;
	uint32 critical_value3;
	uint32 critical_value4;
	Uint32Vec_t level;
	Uint32Vec_t num;
	uint32 patch_exp;
	uint32 time;
	Uint32Vec_t vip;
	Uint32Vec_t yang_entrance;
	Uint32Vec_t yin_entrance;
};
#endif
