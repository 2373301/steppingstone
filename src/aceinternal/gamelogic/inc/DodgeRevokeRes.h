#ifndef DODGE_REVOKE_RES_HPP
#define DODGE_REVOKE_RES_HPP
struct DodgeRevokeInfo
{
	DodgeRevokeInfo()
	: gold_return_rate(0)
	, money_cost_value(0)
	, money_return_rate(0)
	{}
	uint32 gold_return_rate;
	uint32 money_cost_value;
	uint32 money_return_rate;
};
#endif
