#ifndef MANAGE_SECTION_BONUS_RES_HPP
#define MANAGE_SECTION_BONUS_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "SectionBonusRes.h"
class GAMELOGIC_EXOPRT ManageSectionBonusRes
{
public:
	bool loadContent(Document * xml_doc);
	const SectionBonusInfoVec_t & getSectionBonusInfo();
protected:
	bool loadInfo(Element * element);
private:
	SectionBonusInfoVec_t m_Section_Bonus_res_vec;
};
#endif
