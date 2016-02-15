#ifndef MANAGE_DODGE_MINI_RES_HPP
#define MANAGE_DODGE_MINI_RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "DodgeMiniRes.h"
class GAMELOGIC_EXOPRT ManageDodgeMiniRes
{
public:
	bool loadContent(Document * xml_doc);
	const DodgeMiniInfoMap_t & getDodgeMiniInfo();
protected:
	bool loadInfo(Element * element);
private:
	DodgeMiniInfoMap_t m_dodge_mini_res_map;
};
#endif
