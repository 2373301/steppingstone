#ifndef QQSHARE__RES_HPP
#define QQSHARE__RES_HPP
struct QqshareInfo
{
	QqshareInfo()
	: everyday_bonus(0)
	, id(0)
	{}
	Uint32Vec_t bonus;
	Uint32Vec_t day;
	uint32 everyday_bonus;
	uint32 id;
};
#endif
