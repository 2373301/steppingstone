#ifndef MANAGE_GVGREWARDS__RES_HPP
#define MANAGE_GVGREWARDS__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "GvgrewardsRes.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageGvgrewardsRes
{
public:
	~ManageGvgrewardsRes()
	{
		cleanMapSecondContent(m_gvgrewards_res_map);
	}

	bool loadContent(Document * xml_doc);
	const GvgrewardsInfoMap_t & getGvgrewardsInfo();
protected:
	bool loadInfo(Element * element);
private:
	GvgrewardsInfoMap_t m_gvgrewards_res_map;
};
#endif
