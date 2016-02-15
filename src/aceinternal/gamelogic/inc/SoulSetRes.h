#ifndef SOUL_SET_RES_HPP
#define SOUL_SET_RES_HPP
struct SoulSetInfo
{
	SoulSetInfo()
	: id(0)
	{}
	Uint32Vec_t default_open;
	uint32 id;
	Uint32Vec_t unlock;
	Uint32Vec_t unlock_cost;
};
#endif
