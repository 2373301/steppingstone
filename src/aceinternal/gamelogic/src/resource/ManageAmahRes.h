#ifndef MANAGE_AMAH__RES_HPP
#define MANAGE_AMAH__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "AmahRes.h"
class GAMELOGIC_EXOPRT ManageAmahRes
{
public:
	bool loadContent(Document * xml_doc);
	const AmahInfoMap_t & getAmahInfo();
protected:
	bool loadInfo(Element * element);
private:
	AmahInfoMap_t m_amah__res_map;
};
#endif
