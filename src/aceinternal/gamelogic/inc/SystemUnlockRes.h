
#ifndef SYSTEM_UNLOCK_RES_HPP
#define SYSTEM_UNLOCK_RES_HPP

typedef define_unordered_map<uint32, vector<uint32> > UnlockFunctionMap_t;

struct SystemUnlockRes
{
	SystemUnlockRes()
	{}

	UnlockFunctionMap_t stage_unlock_map;

	UnlockFunctionMap_t quest_unlock_map;

	UnlockFunctionMap_t level_unlock_map;

	UnlockFunctionMap_t level_unlock_map2;
};
#endif