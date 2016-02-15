#ifndef RANCH_CFG_RES_HPP
#define RANCH_CFG_RES_HPP
struct RanchCfgInfo
{
	RanchCfgInfo()
	: coin_up(0)
	, color_refresh(0)
	, crit(0)
	, forage_item(0)
	, forage_up(0)
	, help_open1(0)
	, help_open2(0)
	, help_open3(0)
	, help_open4(0)
	, pt_item(0)
	, pt_refresh(0)
	{}
	uint32 coin_up;
	uint32 color_refresh;
	uint32 crit;
	Uint32Vec_t crit_color;
	Uint32Vec_t default_open1;
	Uint32Vec_t default_open2;
	Uint32Vec_t default_open3;
	Uint32Vec_t default_open4;
	uint32 forage_item;
	uint32 forage_up;
	uint32 help_open1;
	uint32 help_open2;
	uint32 help_open3;
	uint32 help_open4;
	uint32 pt_item;
	uint32 pt_refresh;
	Uint32Vec_t riding_up;
};
#endif
