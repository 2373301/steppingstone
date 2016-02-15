#ifndef SOUL_SOUL_RES_HPP
#define SOUL_SOUL_RES_HPP
struct SoulSoulInfo
{
	SoulSoulInfo()
	: color(0)
	, group(0)
	, group_lv(0)
	, id(0)
	, image(0)
	, rate(0)
	, talk_id(0)
	, type(0)
	{}
	uint32 color;
	uint32 group;
	uint32 group_lv;
	uint32 id;
	uint32 image;
	std::string name;
	Uint32Vec_t property_id;
	uint32 rate;
	uint32 talk_id;
	uint32 type;
	Uint32Vec_t value;
};
#endif
