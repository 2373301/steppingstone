#ifndef MANAGE_OPENACT__RES_HPP
#define MANAGE_OPENACT__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "OpenactRes.h"
class GAMELOGIC_EXOPRT ManageOpenactRes
{
public:
	bool loadContent(Document * xml_doc);
	const OpenactInfoMap_t & getOpenactInfo();
protected:
	bool loadInfo(Element * element);
private:
	OpenactInfoMap_t m_OpenAct__res_map;
};
#endif
