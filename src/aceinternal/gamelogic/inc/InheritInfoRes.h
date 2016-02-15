
#ifndef INHERIT_INFO_RES_HPP
#define INHERIT_INFO_RES_HPP

#include "typedef.h"

struct LevelCost 
{
	LevelCost()
	: level(0)
	, money(0)
	, goldblessme_id(0)
	, coin(0)
	{}

	uint32 level;
	uint32 money;
	uint32 goldblessme_id;
	uint32 coin;
};

typedef define_unordered_map<uint32, LevelCost *> LevelCostMap_t;

struct InheritInfoRes
{
	InheritInfoRes()
	{}

	uint32 free_level_pecent;
	uint32 free_edu_pecent;
	uint32 free_training_pecent;
	uint32 free_pellet_pecent;

	uint32 coin_level_pecent;
	uint32 coin_edu_pecent;
	uint32 coin_training_pecent;
	uint32 coin_pellet_pecent;

	LevelCostMap_t level_cost_map;
}
;
#endif