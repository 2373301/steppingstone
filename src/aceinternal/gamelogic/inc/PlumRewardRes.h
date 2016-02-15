#ifndef PLUM_REWARD_RES_HPP
#define PLUM_REWARD_RES_HPP
struct PlumRewardInfo
{
	PlumRewardInfo()
	: id(0)
	, level_group(0)
	//, resource1(0)
	//, resource1_num(0)
	//, resource2(0)
	//, resource2_num(0)
	//, resource3(0)
	//, resource3_num(0)
	, _type(0)
	{}
	uint32 id;
	uint32 level_group;

	uint32 resource1;
	//uint32 resource1_num;
	//uint32 resource2;
	//uint32 resource2_num;
	//uint32 resource3;
	//uint32 resource3_num;
	Uint32Vec_t reward_type;
	Uint32Vec_t reward_value;
	uint32 _type;
};
#endif
