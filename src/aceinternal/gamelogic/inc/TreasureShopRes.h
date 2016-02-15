#ifndef TREASURE_SHOP_RES_HPP
#define TREASURE_SHOP_RES_HPP
struct TreasureShopInfo
{
	TreasureShopInfo()
	: exp_item(0)
	, label(0)
	, level(0)
	{}
	Uint32Vec_t exp_item;
	uint32 label;
	uint32 level;
};
#endif
