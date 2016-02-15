#ifndef TARGET__RES_HPP
#define TARGET__RES_HPP
struct TargetInfo
{
	TargetInfo()
	: id(0)
	, target(0)
	, type(0)
	, value(0)
	{}
	uint32 id;
	Uint32Vec_t reward_num;
	Uint32Vec_t spe_reward;
	uint32 target;
	uint32 type;
	uint32 value;
};
#endif
