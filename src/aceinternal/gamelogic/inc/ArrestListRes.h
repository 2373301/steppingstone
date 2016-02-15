#ifndef ARREST_LIST_RES_HPP
#define ARREST_LIST_RES_HPP
struct ArrestListInfo
{
	ArrestListInfo()
	: enemy(0)
	, id(0)
	, level_group(0)
	, probability(0)
	, type(0)
    , color(0)
	{}
	uint32 enemy;
	uint32 id;
	uint32 level_group;
	uint32 probability;
	uint32 type;
    Uint32Vec_t bonus;
    uint32 color;
};
#endif
