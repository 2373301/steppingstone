
#ifndef DAILY_QUEST_RES_HPP
#define DAILY_QUEST_RES_HPP

struct DailyQuestInfo
{
	DailyQuestInfo()
	: id(0)
	, correct_money(0)
	, correct_seni(0)
	, correct_exp(0)
	, pro(0)
	, senicost(0)
	, rate(0)	
	{}

	uint32 id;
	uint32 correct_money;
	uint32 correct_seni;
	uint32 correct_exp;
	uint32 correct_frame;
	uint32 correct_coin;
	uint32 pro;
	uint32 senicost;
	uint32 rate;
};

#endif