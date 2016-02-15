#ifndef SHL_UNLOCK_RES_HPP
#define SHL_UNLOCK_RES_HPP
struct ShlUnlockInfo
{
	ShlUnlockInfo()
	: colour(0)
	, subtype(0)
	, talk(0)
	, type(0)
	{}
	uint32 colour;
	uint32 subtype;
	uint32 talk;
	uint32 type;
	Uint32Vec_t unlock_id;
};
#endif
