#ifndef MANAGE_TALENTGROUP__RES_HPP
#define MANAGE_TALENTGROUP__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TalentgroupRes.h"
class GAMELOGIC_EXOPRT ManageTalentgroupRes
{
public:
	bool loadContent(Document * xml_doc);
	const TalentgroupInfoMap_t & getTalentgroupInfo();
protected:
	bool loadInfo(Element * element);
private:
	TalentgroupInfoMap_t m_TalentGroup__res_map;
};
#endif
