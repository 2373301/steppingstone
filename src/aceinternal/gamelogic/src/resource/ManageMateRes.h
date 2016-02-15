#ifndef MANAGE_MATE__RES_HPP
#define MANAGE_MATE__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "MateRes.h"
class GAMELOGIC_EXOPRT ManageMateRes
{
public:
	bool loadContent(Document * xml_doc);
	const MateInfoMap_t & getMateInfo();
protected:
	bool loadInfo(Element * element);
private:
	MateInfoMap_t m_mate__res_map;
};
#endif
