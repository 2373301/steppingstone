#ifndef MANAGE_TALENT__RES_HPP
#define MANAGE_TALENT__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TalentRes.h"
class GAMELOGIC_EXOPRT ManageTalentRes
{
public:
	bool loadContent(Document * xml_doc);
	const TalentInfoMap_t & getTalentInfo();
protected:
	bool loadInfo(Element * element);
private:
	TalentInfoMap_t m_Talent__res_map;
};
#endif
