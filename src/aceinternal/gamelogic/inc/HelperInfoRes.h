#ifndef HELPER_INFO_RES_HPP
#define HELPER_INFO_RES_HPP
struct HelperInfoInfo
{
	HelperInfoInfo()
	: group(0)
	, help_color(0)
	, id(0)
	, rate(0)
	, refresh_limit(0)
	{}
	uint32 group;
	uint32 help_color;
	uint32 id;
    Uint32Vec_t property_id;
	uint32 rate;
	uint32 refresh_limit;
	Uint32Vec_t value;
};
#endif
