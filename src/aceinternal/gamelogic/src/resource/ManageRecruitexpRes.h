
#ifndef MANAGE_RECRUITEXP_RES_HPP
#define MANAGE_RECRUITEXP_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageRecruitexpRes
{
public:
	~ManageRecruitexpRes()
	{
		cleanMapSecondContent(m_recruitexp_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const RecruitexpResInfoMap_t & getRecruitexpInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	RecruitexpResInfoMap_t m_recruitexp_info_map;
};

#endif