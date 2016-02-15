#ifndef MANAGE_HORSE_ATTR_RES_HPP
#define MANAGE_HORSE_ATTR_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "HorseAttrRes.h"
class GAMELOGIC_EXOPRT ManageHorseAttrRes
{
public:
	bool loadContent(Document * xml_doc);
	const HorseAttrInfoVec_t & getHorseAttrInfo();
protected:
	bool loadInfo(Element * element);
private:
	HorseAttrInfoVec_t m_Horse_Attr_res_vec;
};
#endif
