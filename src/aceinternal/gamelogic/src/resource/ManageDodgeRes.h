#ifndef MANAGE_DODGE__RES_HPP
#define MANAGE_DODGE__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "DodgeRes.h"
class GAMELOGIC_EXOPRT ManageDodgeRes
{
public:
	bool loadContent(Document * xml_doc);
	const DodgeInfoMap_t & getDodgeInfo();
protected:
	bool loadInfo(Element * element);
private:
	DodgeInfoMap_t m_dodge__res_map;
};
#endif
