#ifndef MANAGE_PLUM__RES_HPP
#define MANAGE_PLUM__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "PlumRes.h"
class GAMELOGIC_EXOPRT ManagePlumRes
{
public:
	bool loadContent(Document * xml_doc);
	const PlumInfoMap_t & getPlumInfo();
protected:
	bool loadInfo(Element * element);
private:
	PlumInfoMap_t m_plum__res_map;
};
#endif
