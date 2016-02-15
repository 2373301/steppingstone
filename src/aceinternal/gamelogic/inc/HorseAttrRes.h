#ifndef HORSE_ATTR_RES_HPP
#define HORSE_ATTR_RES_HPP
struct HorseAttrInfo
{
	HorseAttrInfo()
	: horse_id(0)
	, lv(0)
	{}
	uint32 horse_id;
	uint32 lv;

    Uint32Vec_t property_id;
    Uint32Vec_t value;
};
#endif
