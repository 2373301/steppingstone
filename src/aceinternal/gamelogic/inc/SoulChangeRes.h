#ifndef SOUL_CHANGE_RES_HPP
#define SOUL_CHANGE_RES_HPP
struct SoulChangeInfo
{
	SoulChangeInfo()
	: batch(0)
	, buy_soul(0)
	, buy_soulid(0)
	, cost_a(0)
	, cost_b(0)
	, id(0)
	, lock_cost(0)
	, lock_costid(0)
	, open_limit(0)
	{}
	uint32 batch;
	uint32 buy_soul;
	uint32 buy_soulid;
	Uint32Vec_t coin_color;
	Uint32Vec_t coin_group;
	uint32 cost_a;
	uint32 cost_b;
	uint32 id;
	Uint32Vec_t important_soul;
	Uint32Vec_t limit_coina;
	Uint32Vec_t limit_coinb;
	Uint32Vec_t limit_soula;
	Uint32Vec_t limit_soulb;
	uint32 lock_cost;
	uint32 lock_costid;
	Uint32Vec_t nine_open;
	Uint32Vec_t open_limit;
	Uint32Vec_t soul_color;
	Uint32Vec_t soul_group;
};
#endif
