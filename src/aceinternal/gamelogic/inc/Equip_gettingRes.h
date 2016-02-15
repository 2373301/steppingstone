#ifndef EQUIP_GETTING__RES_HPP
#define EQUIP_GETTING__RES_HPP
struct Equip_gettingInfo
{
	Equip_gettingInfo()
	: job(0)
	{}
	Uint32Vec_t equip;
	uint32 job;
	Uint32Vec_t level;
	Uint32Vec_t part;
	Uint32Vec_t part_num;
	Uint32Vec_t rewards;
};
#endif
