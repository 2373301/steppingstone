
#ifndef PRACTICE_RES_HPP
#define PRACTICE_RES_HPP

struct WineTankResInfo
{
	WineTankResInfo()
		: id(0)
		, unlock(0)
	{}

	uint32 id;
	Uint32Vec_t vip_gold_param;
	uint32 unlock;
};

struct GourdResInfo
{
	GourdResInfo()
		:gourd_num(0),
		renew(0),
		a(0),
		b(0)
	{}

	uint32 gourd_num;
	uint32 renew;
	uint32 a;
	uint32 b;

};

struct StomachResInfo
{
	StomachResInfo()
		:belly_num(0),
		renew_time(0),
		renew_coin(0),
		expand(0)
	{}
	
	uint32 belly_num;
	uint32 renew_time;
	uint32 renew_coin;
	uint32 expand;

};

//
struct WineIncreaseResInfo
{
	WineIncreaseResInfo()
		:a(0),
		b(0),
		volume(0),
		renew(0),
		goldblessme_renew(0)
	{}

	uint32 a;
	uint32 b;
	uint32 volume;
	uint32 renew;
	uint32 goldblessme_renew;
	
};

struct WineTankUpgradeResInfo
{

	Uint32Vec_t exp_up;
	Uint32Vec_t need_vip;
	Uint32Vec_t up_goldblessme;
};


#endif