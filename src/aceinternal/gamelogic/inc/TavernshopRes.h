#ifndef TAVERNSHOP__RES_HPP
#define TAVERNSHOP__RES_HPP
struct TavernshopInfo
{
	TavernshopInfo()
	: get_itemid(0)
	,get_itemnum(0)
	,type(0)
	,value(0)
	{}
	uint32 get_itemid;
	uint32 get_itemnum;
	uint32 type;
	uint32 value;
};
#endif
