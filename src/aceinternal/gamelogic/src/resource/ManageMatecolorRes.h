#ifndef MANAGE_MATECOLOR__RES_HPP
#define MANAGE_MATECOLOR__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MatecolorRes.h"
class GAMELOGIC_EXOPRT ManageMatecolorRes
{
public:
	bool loadContent(Document * xml_doc);
	const MatecolorInfoMap_t & getMatecolorInfo();
protected:
	bool loadInfo(Element * element);
private:
	MatecolorInfoMap_t m_matecolor__res_map;
};
#endif
