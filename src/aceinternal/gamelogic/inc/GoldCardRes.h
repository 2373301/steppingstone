#ifndef GOLD_CARD_RES_HPP
#define GOLD_CARD_RES_HPP
struct GoldCardInfo
{
	GoldCardInfo()
	: bonus_rewards(0)
	, day(0)
	, first_rewards(0)
	, gold_rewards(0)
	, id(0)
	, level(0)
	, need_gold(0)
	, talk(0)
	, target(0)
	, unlock_gold(0)
	, first_bonus(0)
	{}
	uint32 bonus_rewards;
	uint32 day;
	uint32 first_rewards;
	uint32 gold_rewards;
	uint32 id;
	uint32 level;
	uint32 need_gold;
	Uint32Vec_t p;
	uint32 talk;
	uint32 target;
	uint32 unlock_gold;
	uint32 first_bonus;
	Uint32Vec_t v;
	std::string card_name;
};
#endif
