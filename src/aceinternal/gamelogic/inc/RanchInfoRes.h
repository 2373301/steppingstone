#ifndef RANCH_INFO_RES_HPP
#define RANCH_INFO_RES_HPP
struct RanchInfoInfo
{
	RanchInfoInfo()
	: riding_lv(0)
	{}

	Uint32Vec_t ranch_lv;
	uint32 riding_lv;

    Uint32Vec_t property_id;
    Uint32Vec_t value;
};
#endif
