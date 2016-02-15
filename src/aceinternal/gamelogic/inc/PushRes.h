#ifndef PUSH__RES_HPP
#define PUSH__RES_HPP
struct PushInfo
{
	PushInfo()
	: get_id(0)
	, gold_id(0)
	, id(0)
	, push_coin(0)
	, push_id(0)
	, type(0)
	, push_coin2(0)
	, vip(0)
	, push_num(0)
	, time(0)
	, unclock(0)
	, vip_exp(0)
	{}
	uint32 get_id;
	uint32 gold_id;
	uint32 id;
	uint32 push_coin;
	uint32 push_id;
	uint32 type;
	uint32 push_coin2;
	uint32 vip;
	uint32 push_num;
	uint32 time;
	uint32 unclock;
	uint32 vip_exp;
};
#endif
