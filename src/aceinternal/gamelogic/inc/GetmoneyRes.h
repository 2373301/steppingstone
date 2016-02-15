#ifndef GETMONEY__RES_HPP
#define GETMONEY__RES_HPP
struct GetmoneyInfo
{
	GetmoneyInfo()
	: num(0)
	{}
	uint32 num;
	Uint32Vec_t rate;
};
#endif
