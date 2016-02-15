
#ifndef VIP_RES_HPP
#define VIP_RES_HPP

struct VipInfo
{
	VipInfo()
	: function_id(0)
	, unlock_lv(0)
	, is_free(0)
	, cost(0)
	, option_a(0)
	, option_b(0)
	, utype(0)
	{}

	uint32 function_id;
	uint32 unlock_lv;
	int32  is_free;
	uint32 cost;
	int32  option_a;
	int32  option_b;
	uint32 utype;
};

#endif