
#ifndef MANAGE_WAR_SCORE_RES_PP
#define MANAGE_WAR_SCORE_RES_PP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageWarScoreRes
{
public:
	~ManageWarScoreRes()
	{
		cleanVectorContent(m_war_score_info_vec);
	}
public:
	bool loadContent(Document * xml_doc);

	const WarScoreResInfoVec_t & getWarScoreInfoVec();
protected:
	bool loadInfo(Element * element);

private:

	WarScoreResInfoVec_t m_war_score_info_vec;
};

#endif