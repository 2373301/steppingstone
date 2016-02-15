#ifndef MANAGE_FAIL_SCORE_RES_HPP
#define MANAGE_FAIL_SCORE_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "FailScoreRes.h"
class GAMELOGIC_EXOPRT ManageFailScoreRes
{
public:
	bool loadContent(Document * xml_doc);
	const FailScoreInfoMap_t & getFailScoreInfo();
protected:
	bool loadInfo(Element * element);
private:
	FailScoreInfoMap_t m_fail_score_res_map;
};
#endif
