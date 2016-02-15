#ifndef GAMBLING__RES_HPP
#define GAMBLING__RES_HPP
struct GamblingInfo
{
	GamblingInfo()
	: losspercent(0)
	, luckydmoney(0)
	, maxmoney2(0)
	, minmoney2(0)
	, opentime(0)
	, returnvalue(0)
	, returnvalue2(0)
	{}
	Uint32Vec_t hit1;
	Uint32Vec_t hit16;
	Uint32Vec_t hit2;
	Uint32Vec_t hit4;
	Uint32Vec_t hit8;
	Uint32Vec_t hitvalue1;
	Uint32Vec_t hitvalue16;
	Uint32Vec_t hitvalue2;
	Uint32Vec_t hitvalue4;
	Uint32Vec_t hitvalue8;
	Uint32Vec_t id;
	Uint32Vec_t leveleffect;
	Uint32Vec_t leveleffect2;
	Uint32Vec_t levelrange;
	Uint32Vec_t levelrange1;
	uint32 losspercent;
	Uint32Vec_t losspercentm;
	Uint32Vec_t losspercenty;
	Uint32Vec_t losspercenty2;
	uint32 luckydmoney;
	Uint32Vec_t luckyeffect;
	Uint32Vec_t luckylevel;
	uint32 maxmoney2;
	uint32 minmoney2;
	Uint32Vec_t money;
	uint32 opentime;
	uint32 returnvalue;
	uint32 returnvalue2;
	Uint32Vec_t value;
};
#endif
