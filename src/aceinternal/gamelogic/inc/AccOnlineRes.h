#ifndef ACC_ONLINE_RES_HPP
#define ACC_ONLINE_RES_HPP
struct AccOnlineInfo
{
	AccOnlineInfo()
	: start_time(0)
	{}
	Uint32Vec_t days;
	Uint32Vec_t rewards;
	uint32 start_time;
};
#endif
