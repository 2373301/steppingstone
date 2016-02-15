
#ifndef MANAGE_RAISERATE_RES_PP
#define MANAGE_RAISERATE_RES_PP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageRaiseRateRes
{
public:
	~ManageRaiseRateRes()
	{
		cleanVectorContent(m_war_score_info_vec);
	}
public:
	bool loadContent(Document * xml_doc);

	const RaiseRateResInfoVec_t & getRaiseRateInfoVec();
protected:
	bool loadInfo(Element * element);

private:

	RaiseRateResInfoVec_t m_war_score_info_vec;
};

#endif