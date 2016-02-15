#ifndef SOUL_CFG_RES_HPP
#define SOUL_CFG_RES_HPP
struct SoulCfgInfo
{
	SoulCfgInfo()
	: cross_cost(0)
	, cross_costnum(0)
	, cross_freenum(0)
	, hit(0)
	, id(0)
	, raids_cd(0)
	, raids_vip(0)
	, refresh_buy(0)
	, refresh_cd(0)
	, refresh_player(0)
	, reset_cost(0)
	, reset_costnum(0)
	, reset_freenum(0)
	, rob_a(0)
	, rob_b(0)
	, rob_cost(0)
	, rob_costnum(0)
	, rob_freenum(0)
	, search_a(0)
	{}
	Uint32Vec_t buy_cross;
	Uint32Vec_t buy_rob;
	uint32 cross_cost;
	uint32 cross_costnum;
	uint32 cross_freenum;
	uint32 hit;
	uint32 id;
	uint32 raids_cd;
	Uint32Vec_t raids_vip;
	uint32 refresh_buy;
	uint32 refresh_cd;
	uint32 refresh_player;
	Uint32Vec_t reset_buy;
	uint32 reset_cost;
	uint32 reset_costnum;
	uint32 reset_freenum;
	uint32 rob_a;
	uint32 rob_b;
	uint32 rob_cost;
	uint32 rob_costnum;
	uint32 rob_freenum;
	uint32 search_a;
};
#endif
