
#ifndef MANAGE_RECRUITMENT_RES_HPP
#define MANAGE_RECRUITMENT_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "share_fun.h"

class GAMELOGIC_EXOPRT ManageRecruitmentRes
{
public:
	~ManageRecruitmentRes()
	{
		cleanMapSecondContent(m_recruitment_info_map);
	}
public:
	bool loadContent(Document * xml_doc);

	const RecruitmentResInfoMap_t & getRecruitmentInfoMap();
protected:
	bool loadInfo(Element * element);

private:

	RecruitmentResInfoMap_t m_recruitment_info_map;
};

#endif