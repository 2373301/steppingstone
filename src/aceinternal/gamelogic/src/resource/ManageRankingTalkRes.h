#ifndef MANAGE_RANKING_TALK_RES_HPP
#define MANAGE_RANKING_TALK_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "RankingTalkRes.h"
class GAMELOGIC_EXOPRT ManageRankingTalkRes
{
public:
	bool loadContent(Document * xml_doc);
	const RankingTalkInfoMap_t & getRankingTalkInfo();
protected:
	bool loadInfo(Element * element);
private:
	RankingTalkInfoMap_t m_Ranking_Talk_res_map;
};
#endif
