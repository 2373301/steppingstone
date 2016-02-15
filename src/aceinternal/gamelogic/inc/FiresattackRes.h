#ifndef FIRESATTACK__RES_HPP
#define FIRESATTACK__RES_HPP
struct FiresattackInfo
{
	FiresattackInfo()
	: boss(0)
	, fail_reward(0)
	, first_reward(0)
	, group(0)
	, id(0)
	, id_name(0)
	, map(0)
	, name(0)
	, pic(0)
	, sub_group(0)
	, sub_id(0)
	, unlock_id(0)
	, win_reward(0)
	{}
	uint32 boss;
	uint32 fail_reward;
	uint32 first_reward;
	uint32 group;
	uint32 id;
	uint32 id_name;
	uint32 map;
	uint32 name;
	uint32 pic;
	uint32 sub_group;
	uint32 sub_id;
	uint32 unlock_id;
	uint32 win_reward;
};
#endif
