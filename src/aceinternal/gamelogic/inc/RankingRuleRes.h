#ifndef RANKING_RULE_RES_HPP
#define RANKING_RULE_RES_HPP
struct RankingRuleInfo
{
	RankingRuleInfo()
	: id(0)
	, list(0)
	, mail_id(0)
	, num(0)
	, update_time(0)
	{}
	uint32 id;
	uint32 list;
	uint32 mail_id;
	uint32 num;
	uint32 update_time;
};
#endif
