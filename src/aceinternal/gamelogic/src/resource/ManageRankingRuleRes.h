#ifndef MANAGE_RANKING_RULE_RES_HPP
#define MANAGE_RANKING_RULE_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RankingRuleRes.h"
class GAMELOGIC_EXOPRT ManageRankingRuleRes
{
public:
	bool loadContent(Document * xml_doc);
	const RankingRuleInfoMap_t & getRankingRuleInfo();
protected:
	bool loadInfo(Element * element);
private:
	RankingRuleInfoMap_t m_Ranking_Rule_res_map;
};
#endif
