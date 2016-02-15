#ifndef SOUL_SUPER_RES_HPP
#define SOUL_SUPER_RES_HPP
struct SoulSuperInfo
{
	SoulSuperInfo()
	: id(0)
	{}
	uint32 id;
	Uint32Vec_t range;
	Uint32Vec_t property_id;
	uint32 type;
};
#endif
