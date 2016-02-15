#ifndef MANAGE_TARGET__RES_HPP
#define MANAGE_TARGET__RES_HPP
#include "typedef.h"
#include "SimpleXML.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "TargetRes.h"
class GAMELOGIC_EXOPRT ManageTargetRes
{
public:
	bool loadContent(Document * xml_doc);
	const TargetInfoMap_t & getTargetInfo();
protected:
	bool loadInfo(Element * element);
private:
	TargetInfoMap_t m_Target__res_map;
};
#endif
