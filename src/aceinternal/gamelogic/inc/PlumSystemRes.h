#ifndef PLUM_SYSTEM_RES_HPP
#define PLUM_SYSTEM_RES_HPP
struct PlumSystemInfo
{
	PlumSystemInfo()
	: change_times(0)
	, free_times(0)
	{}
	uint32 change_times;
	uint32 free_times;
	Uint32Vec_t library;
};
#endif
