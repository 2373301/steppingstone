
#ifndef MANAGE_GOLD_PARAM_RES_HPP
#define MANAGE_GOLD_PARAM_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageGoldParamRes
{
public:
	~ManageGoldParamRes()
	{
		cleanMapSecondContent(m_gold_param_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const GoldParamResInfoMap_t & getGoldParamInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	GoldParamResInfoMap_t m_gold_param_info_map;
};

#endif